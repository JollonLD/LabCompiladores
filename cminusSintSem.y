%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "parser_context.h"
    #include "code_generator.h"

    extern int yylex(void);
    /* extern int yyparse(void); */
    extern FILE *yyin;
    extern int yylineno;
    extern char *yytext;

    void yyerror(ParserContext *ctx, const char *s);

#ifdef _WIN32
#define STRTOK_REENTRANT(str, delim, saveptr) strtok_s((str), (delim), (saveptr))
#else
#define STRTOK_REENTRANT(str, delim, saveptr) strtok_r((str), (delim), (saveptr))
#endif

    /* Traduz nomes de tokens para mensagens amigáveis */
    const char* translate_token(const char *token) {
        if (strcmp(token, "SEMI") == 0) return "';'";
        if (strcmp(token, "COMMA") == 0) return "','";
        if (strcmp(token, "LPAREN") == 0) return "'('";
        if (strcmp(token, "RPAREN") == 0) return "')'";
        if (strcmp(token, "LBRACK") == 0) return "'['";
        if (strcmp(token, "RBRACK") == 0) return "']'";
        if (strcmp(token, "LBRACE") == 0) return "'{'";
        if (strcmp(token, "RBRACE") == 0) return "'}'";
        if (strcmp(token, "ASSIGN") == 0) return "'='";
        if (strcmp(token, "PLUS") == 0) return "'+'";
        if (strcmp(token, "MINUS") == 0) return "'-'";
        if (strcmp(token, "TIMES") == 0) return "'*'";
        if (strcmp(token, "DIVIDE") == 0) return "'/'";
        if (strcmp(token, "LT") == 0) return "'<'";
        if (strcmp(token, "LE") == 0) return "'<='";
        if (strcmp(token, "GT") == 0) return "'>'";
        if (strcmp(token, "GE") == 0) return "'>='";
        if (strcmp(token, "EQ") == 0) return "'=='";
        if (strcmp(token, "NE") == 0) return "'!='";
        if (strcmp(token, "IF") == 0) return "'if'";
        if (strcmp(token, "ELSE") == 0) return "'else'";
        if (strcmp(token, "WHILE") == 0) return "'while'";
        if (strcmp(token, "RETURN") == 0) return "'return'";
        if (strcmp(token, "INT") == 0) return "'int'";
        if (strcmp(token, "VOID") == 0) return "'void'";
        if (strcmp(token, "ID") == 0) return "identificador";
        if (strcmp(token, "NUM") == 0) return "numero";
        if (strstr(token, "$end") != NULL) return "fim de arquivo";
        return token;
    }
%}

%expect 1
%error-verbose

%parse-param {ParserContext *ctx}

%code requires {
    #include "parser_context.h"

    /* ===== TIPOS DE DADOS ===== */
    typedef enum {
        TYPE_INT,
        TYPE_VOID,
        TYPE_INT_ARRAY,
        TYPE_ERROR
    } TipoVar;

    typedef enum {
        KIND_VAR,
        KIND_ARRAY,
        KIND_FUNC
    } TipoSimbolo;

    typedef struct Simbolo {
        char *nome;
        TipoVar tipo;
        TipoSimbolo kind;
        int tamanho;        /* para arrays */
        int num_params;     /* para funcoes (mantido por compatibilidade) */
        TipoVar *param_types; /* tipos dos parametros */
        int linha;
        int is_param;           /* 1 se este simbolo é um parâmetro */
        struct Escopo *def_scope; /* escopo associado (para funções: o escopo do corpo) */
        struct Simbolo *prox;
    } Simbolo;

    typedef struct Escopo {
        Simbolo *simbolos;
        struct Escopo *pai;
        struct Escopo *next_all; /* link para lista de todos os escopos */
    } Escopo;

    /* Funcoes da tabela de simbolos */
    void enter_scope_ctx(ParserContext *ctx);
    void leave_scope_ctx(ParserContext *ctx);
    Simbolo* lookup_symbol_ctx(ParserContext *ctx, const char *nome);
    Simbolo* lookup_symbol_current_ctx(ParserContext *ctx, const char *nome);
    void insert_symbol_ctx(ParserContext *ctx, const char *nome, TipoVar tipo, TipoSimbolo kind, int linha);
    void insert_array_ctx(ParserContext *ctx, const char *nome, int tamanho, int linha);
    void insert_function_ctx(ParserContext *ctx, const char *nome, TipoVar tipo_retorno, int linha);
    /* void add_param_to_function(const char *func_nome, TipoVar tipo_param); */
    TipoVar check_expression_type_ctx(ParserContext *ctx, const char *op, TipoVar t1, TipoVar t2, int linha);
    /* void check_return_type(TipoVar tipo_func, TipoVar tipo_exp, int linha); */
    /* void check_function_call(const char *nome, int num_args, int linha); */

    /* libera todos os escopos no final */
    void free_all_scopes_ctx(ParserContext *ctx);

    /* ===== ESTRUTURA DA AST ===== */
    typedef enum {STMTK, EXPK, VARK} NodeKind;
    typedef enum {INTEGERK, VOIDK, IFK, WHILEK, RETURNK, COMPK} StmtKind;
    typedef enum {OPK, CONSTK, IDK, ASSIGNK, CALLK, VECTORK} ExpKind;
    typedef enum {DECLK, ACCESSK} VarAccessK;
    
    #define MAXCHILDREN 3
    
    typedef struct Identifier {
        TipoSimbolo varKind;
        VarAccessK acesso;
        union {
            int val;
            char *name;
        } attr;
    } Identifier;
    
    typedef struct treeNode {
        struct treeNode *child[MAXCHILDREN];
        struct treeNode *sibling;
        int lineno;
        NodeKind nodekind;
        union {
            StmtKind stmt;
            ExpKind exp;
            struct Identifier var;
        } kind;
        int op;                    /* token para operadores */
        TipoVar type;              /* tipo de dado */
    } TreeNode;
    
    /* Funções para criar nós */
    TreeNode* newStmtNode(StmtKind kind);
    TreeNode* newExpNode(ExpKind kind);
    TreeNode* newVarNode(ExpKind kind);
    void printTree(TreeNode *tree, int indent);
}

%union {
    int ival;
    char *id;
    TipoVar tipo;
    struct {
        TipoVar tipo;
        int is_array;
    } var_info;
    TreeNode *node;
}

/* ===== TOKENS ===== */
%token<id> ID
%token<ival> NUM
%token ELSE IF INT RETURN VOID WHILE
%token LE LT GT GE EQ NE
%token ASSIGN SEMI COMMA
%token LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE
%token PLUS MINUS TIMES DIVIDE

/* ===== PRECEDÊNCIA ===== */
%left PLUS MINUS
%left TIMES DIVIDE
%right ASSIGN
%left LT LE GT GE EQ NE

/* ===== TIPOS NÃO-TERMINAIS ===== */
%type<tipo> type_specifier
%type<ival> relop addop mulop
%type<node> expression simple_expression
%type<node> additive_expression term factor var
%type<node> program declaration_list declaration
%type<node> var_declaration fun_declaration
%type<node> params param_list param
%type<node> compound_stmt compound_stmt_with_scope local_declarations statement_list
%type<node> statement expression_stmt selection_stmt
%type<node> iteration_stmt return_stmt call args arg_list

%start program

%%

/* 1. program → declaration-list */
program : 
    declaration_list 
    { 
        $$ = $1;
        ctx->ast_root = $$;
        printf("Analise sintatica concluida com sucesso!\n"); 
        if (!ctx->has_errors) {
            printf("Analise semantica: Nenhum erro encontrado.\n");
        }
        /* Impressão da AST movida para a main */
    }
    ;

/* 2. declaration-list → declaration-list declaration | declaration */
declaration_list :
    declaration_list declaration
    {
        TreeNode *t = $1;
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        } else {
            $$ = $2;
        }
    }
    | declaration { $$ = $1; }
    ;

/* 3. declaration → var-declaration | fun-declaration */
declaration :
    var_declaration { $$ = $1; }
    | fun_declaration { $$ = $1; }
    ;

/* 4. var-declaration → type-specifier ID ; | type-specifier ID [ NUM ] ; */
var_declaration :
    type_specifier ID SEMI
    {
        /* Análise semântica */
        insert_symbol_ctx(ctx, $2, $1, KIND_VAR, yylineno);
        
        /* Construção da AST - padrão correto */
        $$ = newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK);
        $$->type = $1;
        
        /* Criar nó ID e anexar como filho */
        $$->child[0] = newVarNode(IDK);
        $$->child[0]->kind.var.attr.name = strdup($2);
        $$->child[0]->kind.var.varKind = KIND_VAR;
        $$->child[0]->kind.var.acesso = DECLK;
        $$->child[0]->type = $1;
        $$->child[0]->lineno = yylineno;
        
        free($2);
    }
    | type_specifier ID LBRACK NUM RBRACK SEMI
    {
        /* Análise semântica */
        if ($1 == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: array '%s' nao pode ser void - LINHA: %d\n",
                    $2, yylineno);
            ctx->has_errors = 1;
        } else {
            insert_array_ctx(ctx, $2, $4, yylineno);
        }
        
        /* Construção da AST - padrão correto */
        $$ = newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK);
        $$->type = $1;
        
        /* Criar nó ID array e anexar como filho */
        $$->child[0] = newVarNode(VECTORK);
        $$->child[0]->kind.var.attr.name = strdup($2);
        $$->child[0]->kind.var.varKind = KIND_ARRAY;
        $$->child[0]->kind.var.acesso = DECLK;
        $$->child[0]->type = $1;
        $$->child[0]->lineno = yylineno;
        
        /* Adicionar tamanho do array como filho do nó array */
        $$->child[0]->child[0] = newExpNode(CONSTK);
        $$->child[0]->child[0]->kind.var.attr.val = $4;
        $$->child[0]->child[0]->type = TYPE_INT;
        $$->child[0]->child[0]->lineno = yylineno;
        
        free($2);
    }
    ;

/* 5. type-specifier → int | void */
type_specifier :
    INT  { 
        $$ = TYPE_INT;
    }
    | VOID { 
        $$ = TYPE_VOID;
    }
    ;

/* 6. fun-declaration → type-specifier ID ( params ) compound-stmt */
fun_declaration :
    type_specifier ID 
    {
        /* Análise semântica */
        insert_function_ctx(ctx, $2, $1, yylineno);
        Simbolo *func_sym = lookup_symbol_current_ctx(ctx, $2);
        enter_scope_ctx(ctx);
        if (func_sym) {
            func_sym->def_scope = ctx->escopo_atual;
        }
    }
    LPAREN params RPAREN compound_stmt
    {
        leave_scope_ctx(ctx);
        
        /* Construção da AST - padrão correto */
        $$ = newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK);
        $$->type = $1;
        
        /* Criar nó ID função e anexar como filho */
        $$->child[0] = newVarNode(IDK);
        $$->child[0]->kind.var.attr.name = strdup($2);
        $$->child[0]->kind.var.varKind = KIND_FUNC;
        $$->child[0]->kind.var.acesso = DECLK;
        $$->child[0]->type = $1;
        $$->child[0]->lineno = yylineno;
        
        /* Adicionar parâmetros e corpo como filhos do nó função */
        $$->child[0]->child[0] = $5;  /* parâmetros */
        $$->child[0]->child[1] = $7;  /* corpo */
        
        free($2);
    }
    ;

/* 7. params → param-list | void */
params :
    param_list { $$ = $1; }
    | VOID { $$ = NULL; }
    | /* empty */ { $$ = NULL; }
    ;

/* 8. param-list → param-list , param | param */
param_list :
    param_list COMMA param
    {
        TreeNode *t = $1;
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = $3;
            $$ = $1;
        } else {
            $$ = $3;
        }
    }
    | param { $$ = $1; }
    ;

/* 9. param → type-specifier ID | type-specifier ID [ ] */
param :
    type_specifier ID
    {
        /* Análise semântica */
        if ($1 == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: parametro '%s' nao pode ser void - LINHA: %d\n",
                    $2, yylineno);
            ctx->has_errors = 1;
        } else {
            insert_symbol_ctx(ctx, $2, $1, KIND_VAR, yylineno);
            Simbolo *p = lookup_symbol_current_ctx(ctx, $2);
            if (p) p->is_param = 1;
        }
        
        /* Construção da AST - padrão correto */
        $$ = newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK);
        $$->type = $1;
        
        $$->child[0] = newVarNode(IDK);
        $$->child[0]->kind.var.attr.name = strdup($2);
        $$->child[0]->kind.var.varKind = KIND_VAR;
        $$->child[0]->kind.var.acesso = DECLK;
        $$->child[0]->type = $1;
        $$->child[0]->lineno = yylineno;
        
        free($2);
    }
    | type_specifier ID LBRACK RBRACK
    {
        /* Análise semântica */
        if ($1 == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: parametro array '%s' nao pode ser void - LINHA: %d\n",
                    $2, yylineno);
            ctx->has_errors = 1;
        } else {
            insert_symbol_ctx(ctx, $2, TYPE_INT_ARRAY, KIND_ARRAY, yylineno);
            Simbolo *p = lookup_symbol_current_ctx(ctx, $2);
            if (p) p->is_param = 1;
        }
        
        /* Construção da AST - padrão correto */
        $$ = newStmtNode($1 == TYPE_INT ? INTEGERK : VOIDK);
        $$->type = $1;
        
        $$->child[0] = newVarNode(VECTORK);
        $$->child[0]->kind.var.attr.name = strdup($2);
        $$->child[0]->kind.var.varKind = KIND_ARRAY;
        $$->child[0]->kind.var.acesso = DECLK;
        $$->child[0]->type = TYPE_INT_ARRAY;
        $$->child[0]->lineno = yylineno;
        
        free($2);
    }
    ;

/* 10. compound-stmt → { local-declarations statement-list } */
compound_stmt :
    LBRACE local_declarations statement_list RBRACE
    {
        $$ = newStmtNode(COMPK);
        $$->child[0] = $2;  /* declarações locais */
        $$->child[1] = $3;  /* lista de statements */
        $$->lineno = yylineno;
    }
    ;
/* 11. local-declarations → local-declarations var-declaration | empty */
local_declarations :
    local_declarations var_declaration
    {
        TreeNode *t = $1;
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        } else {
            $$ = $2;
        }
    }
    | /* empty */ { $$ = NULL; }
    ;

/* 12. statement-list → statement-list statement | empty */
statement_list :
    statement_list statement
    {
        TreeNode *t = $1;
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = $2;
            $$ = $1;
        } else {
            $$ = $2;
        }
    }
    | /* empty */ { $$ = NULL; }
    ;

/* 13. statement → expression-stmt | compound-stmt | selection-stmt | iteration-stmt | return-stmt */
statement :
    expression_stmt { $$ = $1; }
    | compound_stmt_with_scope { $$ = $1; }
    | selection_stmt { $$ = $1; }
    | iteration_stmt { $$ = $1; }
    | return_stmt { $$ = $1; }
    ;

compound_stmt_with_scope :
    { enter_scope_ctx(ctx); } 
    compound_stmt 
    { 
        leave_scope_ctx(ctx);
        $$ = $2;
    }
    ;

/* 14. expression-stmt → expression ; | ; */
expression_stmt :
    expression SEMI { $$ = $1; }
    | SEMI { $$ = NULL; }
    ;

/* 15. selection-stmt → if ( expression ) statement | if ( expression ) statement else statement */
selection_stmt :
    IF LPAREN expression RPAREN statement
    {
        /* Análise semântica */
        if ($3 && $3->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do IF deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }

        /* Construção da AST */
        $$ = newStmtNode(IFK);
        $$->child[0] = $3;    /* condição */
        $$->child[1] = $5;    /* then */
        $$->child[2] = NULL;  /* else */
        $$->lineno = yylineno;
    }
    | IF LPAREN expression RPAREN statement ELSE statement
    {
        /* Análise semântica */
        if ($3 && $3->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do IF deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }

        /* Construção da AST */
        $$ = newStmtNode(IFK);
        $$->child[0] = $3;    /* condição */
        $$->child[1] = $5;    /* then */
        $$->child[2] = $7;    /* else */
        $$->lineno = yylineno;
    }
    ;

/* 16. iteration-stmt → while ( expression ) statement */
iteration_stmt :
    WHILE LPAREN expression RPAREN statement
    {
        /* Análise semântica */
        if ($3 && $3->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do WHILE deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }
        
        /* Construção da AST */
        $$ = newStmtNode(WHILEK);
        $$->child[0] = $3;    /* condição */
        $$->child[1] = $5;    /* corpo */
        $$->lineno = yylineno;
    }
    ;

/* 17. return-stmt → return ; | return expression ; */
return_stmt :
    RETURN SEMI
    {
        $$ = newStmtNode(RETURNK);
        $$->child[0] = NULL;  /* sem expressão */
        $$->lineno = yylineno;
    }
    | RETURN expression SEMI
    {
        $$ = newStmtNode(RETURNK);
        $$->child[0] = $2;    /* expressão */
        $$->lineno = yylineno;
    }
    ;

/* 18. expression → var = expression | simple-expression */
expression :
    var ASSIGN expression
    {
        /* Análise semântica */
        TipoVar var_type = $1 ? $1->type : TYPE_ERROR;
        TipoVar exp_type = $3 ? $3->type : TYPE_ERROR;
        
        if ($1 && $1->nodekind == VARK && $1->kind.var.varKind == KIND_ARRAY &&
            $1->child[0] == NULL) {
            fprintf(stderr, "ERRO SEMANTICO: nao e possivel atribuir a array completo - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        } else if (var_type != exp_type && exp_type != TYPE_ERROR && var_type != TYPE_ERROR) {
            fprintf(stderr, "ERRO SEMANTICO: tipos incompativeis na atribuicao - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }
        
        /* Construção da AST */
        $$ = newExpNode(ASSIGNK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->type = var_type;
        $$->lineno = yylineno;
    }
    | simple_expression { $$ = $1; }
    ;

/* 19. var → ID | ID [ expression ] */
var :
    ID
    {
        Simbolo *s = lookup_symbol_ctx(ctx, $1);
        TipoVar tipo = TYPE_ERROR;
        TipoSimbolo kind = KIND_VAR;
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada - LINHA: %d\n", $1, yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = s->tipo;
            kind = s->kind;
        }
        
        /* Cria nó de variável */
        if (kind == KIND_ARRAY) {
            $$ = newVarNode(VECTORK);
            $$->type = TYPE_INT_ARRAY;
        } else {
            $$ = newVarNode(IDK);
            $$->type = tipo;
        }
        $$->kind.var.attr.name = strdup($1);
        $$->kind.var.varKind = kind;
        $$->kind.var.acesso = ACCESSK;
        $$->lineno = yylineno;
        free($1);
    }
    | ID LBRACK expression RBRACK
    {
        Simbolo *s = lookup_symbol_ctx(ctx, $1);
        TipoVar tipo = TYPE_ERROR;
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada - LINHA: %d\n", $1, yylineno);
            ctx->has_errors = 1;
        } else if (s->kind != KIND_ARRAY) {
            fprintf(stderr, "ERRO SEMANTICO: '%s' nao e um array - LINHA: %d\n", $1, yylineno);
            ctx->has_errors = 1;
        } else if ($3 && $3->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: indice de array deve ser inteiro - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = TYPE_INT;
        }
        
        /* Cria nó de acesso a array */
        $$ = newVarNode(VECTORK);
        $$->kind.var.attr.name = strdup($1);
        $$->kind.var.varKind = KIND_ARRAY;
        $$->kind.var.acesso = ACCESSK;
        $$->child[0] = $3;  /* índice */
        $$->type = tipo;
        $$->lineno = yylineno;
        free($1);
    }
    ;

/* 20. simple-expression → additive-expression relop additive-expression | additive-expression */
simple_expression :
    additive_expression relop additive_expression
    {
        /* Análise semântica */
        TipoVar t1 = $1 ? $1->type : TYPE_ERROR;
        TipoVar t2 = $3 ? $3->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "relacional", t1, t2, yylineno);
        
        /* Construção da AST */
        $$ = newExpNode(OPK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->op = $2;  /* operador relacional */
        $$->type = result;
        $$->lineno = yylineno;
    }
    | additive_expression { $$ = $1; }
    ;

/* 21. relop → <= | < | > | >= | == | != */
relop : LE { $$ = LE; } 
      | LT { $$ = LT; } 
      | GT { $$ = GT; } 
      | GE { $$ = GE; } 
      | EQ { $$ = EQ; } 
      | NE { $$ = NE; }
      ;

/* 22. additive-expression → additive-expression addop term | term */
additive_expression :
    additive_expression addop term
    {
        /* Análise semântica */
        TipoVar t1 = $1 ? $1->type : TYPE_ERROR;
        TipoVar t2 = $3 ? $3->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "aditivo", t1, t2, yylineno);
        
        /* Construção da AST */
        $$ = newExpNode(OPK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->op = $2;  /* operador aditivo */
        $$->type = result;
        $$->lineno = yylineno;
    }
    | term { $$ = $1; }
    ;

/* 23. addop → + | - */
addop : PLUS { $$ = PLUS; } 
      | MINUS { $$ = MINUS; }
      ;

/* 24. term → term mulop factor | factor */
term :
    term mulop factor
    {
        /* Análise semântica */
        TipoVar t1 = $1 ? $1->type : TYPE_ERROR;
        TipoVar t2 = $3 ? $3->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "multiplicativo", t1, t2, yylineno);
        
        /* Construção da AST */
        $$ = newExpNode(OPK);
        $$->child[0] = $1;
        $$->child[1] = $3;
        $$->op = $2;  /* operador multiplicativo */
        $$->type = result;
        $$->lineno = yylineno;
    }
    | factor { $$ = $1; }
    ;

/* 25. mulop → * | / */
mulop : TIMES { $$ = TIMES; } 
      | DIVIDE { $$ = DIVIDE; }
      ;

/* 26. factor → ( expression ) | var | call | NUM */
factor :
    LPAREN expression RPAREN { $$ = $2; }
    | var { $$ = $1; }
    | call { $$ = $1; }
    | NUM 
    { 
        $$ = newExpNode(CONSTK);
        $$->kind.var.attr.val = $1;
        $$->type = TYPE_INT;
        $$->lineno = yylineno;
    }
    ;

/* 27. call → ID ( args ) */
call :
    ID LPAREN args RPAREN
    {
        Simbolo *s = lookup_symbol_ctx(ctx, $1);
        TipoVar tipo = TYPE_INT;  /* padrão */
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: funcao '%s' nao declarada - LINHA: %d\n", $1, yylineno);
            ctx->has_errors = 1;
        } else if (s->kind != KIND_FUNC) {
            fprintf(stderr, "ERRO SEMANTICO: '%s' nao e uma funcao - LINHA: %d\n", $1, yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = s->tipo;  /* tipo de retorno da função */
        }
        
        /* Cria nó de chamada de função */
        $$ = newExpNode(CALLK);
        $$->kind.var.attr.name = strdup($1);
        $$->child[0] = $3;  /* argumentos */
        $$->type = tipo;
        $$->lineno = yylineno;
        free($1);
    }
    ;

/* 28. args → arg-list | empty */
args :
    arg_list { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

/* 29. arg-list → arg-list , expression | expression */
arg_list :
    arg_list COMMA expression
    {
        TreeNode *t = $1;
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = $3;
            $$ = $1;
        } else {
            $$ = $3;
        }
    }
    | expression { $$ = $1; }
    ;

%%

/* ===== FUNÇÕES PARA GRAPHVIZ ===== */

/* Estrutura para contexto de impressão DOT */
typedef struct {
    int node_counter;
    FILE *fp;
} DotContext;

/* Função auxiliar para escapar strings */
static char *escape_label(const char *str) {
    if (!str) return strdup("");

    int len = strlen(str);
    char *escaped = (char *)malloc(len * 2 + 1);
    
    int j = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] == '"' || str[i] == '\\') {
            escaped[j++] = '\\';
        }

        escaped[j++] = str[i];
    }

    escaped[j] = '\0';

    return escaped;
}

/* Função para obter símbolo do operador */
static const char* get_op_symbol(int op) {
    switch (op) {
        case PLUS: return "+";
        case MINUS: return "-";
        case TIMES: return "*";
        case DIVIDE: return "/";
        case LT: return "<";
        case LE: return "<=";
        case GT: return ">";
        case GE: return ">=";
        case EQ: return "==";
        case NE: return "!=";
        default: return "?";
    }
}
/* Função para pegar qual o Tipo para Declaração de Var, Array e Function */
static const char* get_type_string(TipoVar type) {
    switch (type) {
        case TYPE_INT:       return "int";
        case TYPE_VOID:      return "void";
        case TYPE_INT_ARRAY: return "int[]";
        case TYPE_ERROR:     return "error";
        default:             return "?";
    }
}

/* Função recursiva para gerar GraphViz DOT */
static int printTreeDOT_simplified(DotContext *ctx, TreeNode *tree, int parent_id) {
    if (tree == NULL) return -1;
    
    int current_id = ctx->node_counter++;
    FILE *fp = ctx->fp;
    
    /* Define cor e forma do nó */
    const char *shape = "box";
    const char *color = "lightblue";
    char label[256] = "";
    
    if (tree->nodekind == STMTK) {
        switch(tree->kind.stmt) {
            case INTEGERK:
                snprintf(label, sizeof(label), "int");
                color = "lightblue";
                shape = "box";
                break;

            case VOIDK:
                snprintf(label, sizeof(label), "void");
                color = "lightblue";
                shape = "box";
                break;

            case IFK:
                snprintf(label, sizeof(label), "IF");
                color = "orange";
                shape = "diamond";
                break;

            case WHILEK:
                snprintf(label, sizeof(label), "WHILE");
                color = "orange";
                shape = "diamond";
                break;

            case RETURNK:
                snprintf(label, sizeof(label), "RETURN");
                color = "pink";
                break;

            case COMPK:
                snprintf(label, sizeof(label), "{}");
                color = "lightcoral";
                shape = "box";
                break;
        }
    } 
    else if (tree->nodekind == VARK) {
        char *esc_name = escape_label(tree->kind.var.attr.name);
        
        if (tree->kind.var.varKind == KIND_FUNC) {
            snprintf(label, sizeof(label), "Function\\n%s", esc_name);
            color = "gold";
            shape = "ellipse";
        } 
        else if (tree->kind.var.varKind == KIND_ARRAY) {
            if (tree->kind.var.acesso == DECLK) {
                snprintf(label, sizeof(label), "Array\\n%s", esc_name);
                color = "lightgreen";
            } 
            else {
                snprintf(label, sizeof(label), "%s[]", esc_name);
                color = "lightyellow";
            }
        } 
        else {
            if (tree->kind.var.acesso == DECLK) {
                snprintf(label, sizeof(label), "Var\\n%s", esc_name);
                color = "lightgreen";
            } 
            else {
                snprintf(label, sizeof(label), "%s", esc_name);
                color = "lightyellow";
            }
        }

        free(esc_name);
    } 
    else if (tree->nodekind == EXPK) {
        switch (tree->kind.exp) {
            case OPK:
                snprintf(label, sizeof(label), "%s", get_op_symbol(tree->op));
                color = "lavender";
                shape = "circle";
                break;

            case CONSTK:
                snprintf(label, sizeof(label), "%d", tree->kind.var.attr.val);
                color = "lightpink";
                shape = "circle";
                break;

            case IDK: {
                char *esc_name = escape_label(tree->kind.var.attr.name);
                snprintf(label, sizeof(label), "%s", esc_name);
                free(esc_name);
                color = "lightyellow";
            }
            break;

            case ASSIGNK:
                snprintf(label, sizeof(label), "=");
                color = "khaki";
                shape = "circle";
                break;

            case CALLK: {
                char *esc_name = escape_label(tree->kind.var.attr.name);
                snprintf(label, sizeof(label), "%s()", esc_name);
                free(esc_name);
                color = "peachpuff";
            }
            break;

            case VECTORK: {
                char *esc_name = escape_label(tree->kind.var.attr.name);
                snprintf(label, sizeof(label), "%s[]", esc_name);
                free(esc_name);
                color = "lightcyan";
            }
            break;
        }
    }

    fprintf(fp, "  node%d [label=\"%s\", shape=%s, style=\"filled\", fillcolor=%s];\n", current_id, label, shape, color);
    
    /* Liga ao pai */
    if (parent_id >= 0) {
        fprintf(fp, "  node%d -> node%d [weight=10];\n", parent_id, current_id);
    }

    /* Visita irmãos */
    if (tree->sibling != NULL) {
        int sib_id = printTreeDOT_simplified(ctx, tree->sibling, -1);
        
        /* força os dois no mesmo nível */
        fprintf(fp, "  { rank=same; node%d; node%d; }\n", current_id, sib_id);
        
        /* aresta de irmão sem influenciar hierarquia vertical */
        fprintf(fp, "  node%d -> node%d [style=dashed, color=gray, constraint=false, minlen=3];\n", current_id, sib_id);
        
        /* ancora o irmão no mesmo pai para estabilizar lado a lado */
        if (parent_id >= 0) {
            fprintf(fp, "  node%d -> node%d [style=invis, weight=20];\n", parent_id, sib_id);
        }
    }

    /* Processa filhos */
    for (int i = 0; i < MAXCHILDREN; i++) {
        TreeNode *filho = tree->child[i];
        
        if (filho != NULL) {
            /* cria primeiro filho ligado ao pai */
            printTreeDOT_simplified(ctx, filho, current_id); 
        }
    }


    return current_id;
}

/* Função principal para gerar arquivo DOT */
void printTreeDOT(TreeNode *tree, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Erro: nao foi possivel criar arquivo '%s'\n", filename);
        return;
    }
    
    /* Cria contexto */
    DotContext ctx = {0, fp};
    
    /* Cabeçalho */
    fprintf(fp, "digraph AST {\n");
    fprintf(fp, "  rankdir=TB;\n");
    fprintf(fp, "  node [fontname=\"Arial\", fontsize=14, fontcolor=\"#333333\"];\n");
    fprintf(fp, "  edge [fontname=\"Arial\", fontsize=10, color=\"#666666\"];\n");
    fprintf(fp, "  bgcolor=\"white\";\n");
    fprintf(fp, "  \n");
    fprintf(fp, "  // AST Simplificada (Abstract Syntax Tree)\n");
    fprintf(fp, "  \n");


    printTreeDOT_simplified(&ctx, tree, -1);

    // Verificar irmao do nó raiz
    /* printTreeSimplified(tree->sibling, 0); */

    /* Gera a árvore */
    /* printTreeDOT_simplified(&ctx, tree, -1); */

    /* Rodapé */
    fprintf(fp, "}\n");
    fclose(fp);

    printf("\nArquivo GraphViz gerado: %s\n", filename);
    printf("Para gerar a imagem, execute:\n");
    printf("   dot -Tpng %s -o ast.png\n", filename);
    printf("   dot -Tsvg %s -o ast.svg (recomendado)\n\n", filename);
}

/* Impressão textual (opcional) */
void printTreeSimplified(TreeNode *tree, int indent) {
    if (tree == NULL) return;

    for (int i = 0; i < indent; i++) printf("\t");

    if (tree->nodekind == STMTK) {
        switch (tree->kind.stmt) {
            case INTEGERK:

            case VOIDK:
                /* Pula nós de tipo */
                if (tree->child[0]) {
                    printTreeSimplified(tree->child[0], indent);
                    printTreeSimplified(tree->sibling, indent);
                    return;
                }
                break;
                
            case IFK: 
                printf("IF\n"); 
                break;

            case WHILEK: 
                printf("WHILE\n"); 
                break;

            case RETURNK: 
                printf("RETURN\n"); 
                break;

            case COMPK:
                /* Pula compound, vai direto aos filhos */
                for (int i = 0; i < MAXCHILDREN; i++) {
                    printTreeSimplified(tree->child[i], indent);
                }
                printTreeSimplified(tree->sibling, indent);
                return;
        }
    } 
    else if (tree->nodekind == VARK) {
        if (tree->kind.var.varKind == KIND_FUNC) {
            printf("Function: %s\n", tree->kind.var.attr.name);
        } 
        else if (tree->kind.var.varKind == KIND_ARRAY) {
            if (tree->kind.var.acesso == DECLK) {
                printf("Array: %s\n", tree->kind.var.attr.name);
            } 
            else {
                printf("%s[]\n", tree->kind.var.attr.name);
            }
        } 
        else {
            if (tree->kind.var.acesso == DECLK) {
                printf("Var: %s\n", tree->kind.var.attr.name);
            } 
            else {
                printf("%s\n", tree->kind.var.attr.name);
            }
        }
    } 
    else if (tree->nodekind == EXPK) {
        switch (tree->kind.exp) {
            case OPK:
                printf("%s\n", get_op_symbol(tree->op));
                break;

            case CONSTK: 
                printf("%d\n", tree->kind.var.attr.val); 
                break;

            case IDK: 
                printf("%s\n", tree->kind.var.attr.name); 
                break;

            case ASSIGNK: 
                printf("=\n"); 
                break;

            case CALLK: 
                printf("%s()\n", tree->kind.var.attr.name); 
                break;

            case VECTORK: 
                printf("%s[]\n", tree->kind.var.attr.name);
                break;
        }
    }
    
    /* Imprime filhos */
    for (int i = 0; i < MAXCHILDREN; i++)
        printTreeSimplified(tree->child[i], indent + 1);
    
    /* Imprime irmãos */
    printTreeSimplified(tree->sibling, indent);
}

/* ===== IMPLEMENTAÇÃO DA TABELA DE SÍMBOLOS ===== */

void enter_scope_ctx(ParserContext *ctx) {
    Escopo *novo = (Escopo*) malloc(sizeof(Escopo));
    novo->simbolos = NULL;
    novo->pai = ctx->escopo_atual;
    novo->next_all = ctx->lista_escopos;
    ctx->lista_escopos = novo;
    ctx->escopo_atual = novo;
}

void leave_scope_ctx(ParserContext *ctx) {
    if (!ctx->escopo_atual) return;
    ctx->escopo_atual = ctx->escopo_atual->pai;
}

Simbolo* lookup_symbol_current_ctx(ParserContext *ctx, const char *nome) {
    if (!ctx->escopo_atual) return NULL;
    Simbolo *s = ctx->escopo_atual->simbolos;
    while (s) {
        if (strcmp(s->nome, nome) == 0) return s;
        s = s->prox;
    }
    return NULL;
}

Simbolo* lookup_symbol_ctx(ParserContext *ctx, const char *nome) {
    Escopo *e = ctx->escopo_atual;
    while (e) {
        Simbolo *s = e->simbolos;
        while (s) {
            if (strcmp(s->nome, nome) == 0) return s;
            s = s->prox;
        }
        e = e->pai;
    }
    return NULL;
}

void insert_symbol_ctx(ParserContext *ctx, const char *nome, TipoVar tipo, TipoSimbolo kind, int linha) {
    if (!ctx->escopo_atual) {
        fprintf(stderr, "Erro interno: nenhum escopo ativo\n");
        return;
    }
    
    if (lookup_symbol_current_ctx(ctx, nome)) {
        fprintf(stderr, "ERRO SEMANTICO: identificador '%s' ja declarado neste escopo - LINHA: %d\n", nome, linha);
        ctx->has_errors = 1;
        return;
    }
    
    Simbolo *s = (Simbolo*) malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = tipo;
    s->kind = kind;
    s->tamanho = 0;
    s->num_params = 0;
    s->param_types = NULL;
    s->linha = linha;
    s->prox = ctx->escopo_atual->simbolos;
    s->is_param = 0;
    s->def_scope = NULL;
    ctx->escopo_atual->simbolos = s;
}

void insert_array_ctx(ParserContext *ctx, const char *nome, int tamanho, int linha) {
    if (!ctx->escopo_atual) return;
    
    if (lookup_symbol_current_ctx(ctx, nome)) {
        fprintf(stderr, "ERRO SEMANTICO: identificador '%s' ja declarado neste escopo - LINHA: %d\n", nome, linha);
        ctx->has_errors = 1;
        return;
    }
    
    Simbolo *s = (Simbolo*) malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = TYPE_INT_ARRAY;
    s->kind = KIND_ARRAY;
    s->tamanho = tamanho;
    s->num_params = 0;
    s->param_types = NULL;
    s->linha = linha;
    s->prox = ctx->escopo_atual->simbolos;
    ctx->escopo_atual->simbolos = s;
}

void insert_function_ctx(ParserContext *ctx, const char *nome, TipoVar tipo_retorno, int linha) {
    if (!ctx->escopo_atual) return;
    
    if (lookup_symbol_current_ctx(ctx, nome)) {
        fprintf(stderr, "ERRO SEMANTICO: identificador '%s' ja declarado neste escopo - LINHA: %d\n", nome, linha);
        ctx->has_errors = 1;
        return;
    }
    
    Simbolo *s = (Simbolo*) malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = tipo_retorno;
    s->kind = KIND_FUNC;
    s->tamanho = 0;
    s->num_params = 0;
    s->param_types = NULL;
    s->linha = linha;
    s->prox = ctx->escopo_atual->simbolos;
    ctx->escopo_atual->simbolos = s;
}

TipoVar check_expression_type_ctx(ParserContext *ctx, const char *op, TipoVar t1, TipoVar t2, int linha) {
    if (t1 == TYPE_ERROR || t2 == TYPE_ERROR) {
        return TYPE_ERROR;
    }
    
    if (t1 == TYPE_VOID || t2 == TYPE_VOID) {
        fprintf(stderr, "ERRO SEMANTICO: operacao %s com tipo void - LINHA: %d\n", op, linha);
        ctx->has_errors = 1;
        return TYPE_ERROR;
    }

    if (t1 != TYPE_INT || t2 != TYPE_INT) {
        fprintf(stderr, "ERRO SEMANTICO: operacao %s requer operandos inteiros - LINHA: %d\n", op, linha);
        ctx->has_errors = 1;
        return TYPE_ERROR;
    }
    
    return TYPE_INT;
}

void ExibirTabelaSimbolos_ctx(ParserContext *ctx) {
    printf("\n");
    printf("--------------------------------------------------------------------------------------\n");
    printf("|                      TABELA DE SIMBOLOS DO COMPILADOR C-MINUS                      |\n");
    printf("--------------------------------------------------------------------------------------\n");
    
    printf("\n%-20s | %-10s | %-8s | %-10s | %-8s | %-6s | %s\n", 
           "NOME", "TIPO", "CATEGORIA", "ESCOPO", "TAMANHO", "PARAMS", "LINHA");
    printf("%-20s-%-10s-%-8s-%-10s-%-8s-%-6s---------\n",
           "--------------------", "----------", "--------", "----------", "--------", "------");
    
    /* Conta e armazena ponteiros para todos os escopos */
    int count = 0;
    Escopo *e = ctx->lista_escopos;
    while (e) { count++; e = e->next_all; }
    if (count == 0) {
        printf("\n");
        return;
    }
    
    Escopo **arr = (Escopo**) malloc(count * sizeof(Escopo*));
    int i = 0;
    e = ctx->lista_escopos;
    while (e) { arr[i++] = e; e = e->next_all; }

    /* printf("%-6d | ", computed_params); */
    
    /* Imprime do escopo mais antigo (global) para o mais interno,
       atribuindo escopo_num começando em 0 */
    int escopo_num = 0;
    for (int idx = count - 1; idx >= 0; idx--) {
        Simbolo *s = arr[idx]->simbolos;
        while (s) {
            printf("%-20s | ", s->nome);
            
            /* Tipo */
            switch (s->tipo) {
                case TYPE_INT:        printf("%-10s | ", "int"); break;
                case TYPE_VOID:       printf("%-10s | ", "void"); break;
                case TYPE_INT_ARRAY:  printf("%-10s | ", "int[]"); break;
                case TYPE_ERROR:      printf("%-10s | ", "ERROR"); break;
            }
            
            /* Tipo de Símbolo */
            switch (s->kind) {
                case KIND_VAR:   printf("%-8s | ", "VAR"); break;
                case KIND_ARRAY: printf("%-8s | ", "ARRAY"); break;
                case KIND_FUNC:  printf("%-8s | ", "FUNC"); break;
            }
            
            /* Escopo */
            printf("%-10d | ", escopo_num);
            
            /* Tamanho ou Params */
            if (s->kind == KIND_ARRAY) {
                printf("%-8d | ", s->tamanho);
            } else if (s->kind == KIND_FUNC) {
                printf("%-8s | ", "-");
            } else {
                printf("%-8s | ", "-");
            }
            
            /* Número de parâmetros */
            if (s->kind == KIND_FUNC) {
                int computed_params = 0;
                if (s->def_scope) {
                    Simbolo *ps = s->def_scope->simbolos;
                    while (ps) {
                        if (ps->is_param) computed_params++;
                        ps = ps->prox;
                    }
                }
                printf("%-6d | ", computed_params);
            } else {
                printf("%-6s | ", "-");
            }
            
            printf("%d\n", s->linha);
            s = s->prox;
        }
        escopo_num++;
    }
    
    free(arr);
    printf("\n");
}

void free_all_scopes_ctx(ParserContext *ctx) {
    Escopo *e = ctx->lista_escopos;
    while (e) {
        Simbolo *s = e->simbolos;
        while (s) {
            Simbolo *tmp = s;
            s = s->prox;
            free(tmp->nome);
            if (tmp->param_types) free(tmp->param_types);
            free(tmp);
        }
        Escopo *tmp_e = e;
        e = e->next_all;
        free(tmp_e);
    }
    ctx->lista_escopos = NULL;
    ctx->escopo_atual = NULL;
}

/* ===== IMPLEMENTAÇÃO DAS FUNÇÕES DA AST ===== */

/* Cria nó de statement */
TreeNode* newStmtNode(StmtKind kind) {
    TreeNode *t = (TreeNode*) malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para TreeNode\n");
        exit(1);
    }
    for (int i = 0; i < MAXCHILDREN; i++)
        t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = STMTK;
    t->kind.stmt = kind;
    t->lineno = yylineno;
    t->type = TYPE_VOID;
    t->op = 0;
    return t;
}

/* Cria nó de expressão */
TreeNode* newExpNode(ExpKind kind) {
    TreeNode *t = (TreeNode*) malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para TreeNode\n");
        exit(1);
    }
    for (int i = 0; i < MAXCHILDREN; i++)
        t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = EXPK;
    t->kind.exp = kind;
    t->lineno = yylineno;
    t->type = TYPE_INT;
    t->op = 0;
    t->kind.var.attr.name = NULL;
    return t;
}

/* Cria nó de variável/identificador */
TreeNode* newVarNode(ExpKind kind) {
    TreeNode *t = (TreeNode*) malloc(sizeof(TreeNode));
    if (t == NULL) {
        fprintf(stderr, "Erro: falha ao alocar memória para TreeNode\n");
        exit(1);
    }
    for (int i = 0; i < MAXCHILDREN; i++)
        t->child[i] = NULL;
    t->sibling = NULL;
    t->nodekind = VARK;
    t->kind.var.acesso = DECLK;
    t->kind.var.varKind = KIND_VAR;
    t->kind.var.attr.name = NULL;
    t->lineno = yylineno;
    t->type = TYPE_INT;
    t->op = 0;
    return t;
}

/* Imprime a árvore sintática */
void printTree(TreeNode *tree, int indent) {
    if (tree == NULL) return;
    
    /* Imprime indentação */
    for (int i = 0; i < indent; i++)
        printf("  ");
    
    /* Imprime tipo do nó */
    if (tree->nodekind == STMTK) {
        switch (tree->kind.stmt) {
            case INTEGERK:
                printf("[int]");
                if (tree->child[0]) {
                    if (tree->child[0]->nodekind == VARK) {
                        if (tree->child[0]->kind.var.varKind == KIND_FUNC) {
                            printf(" Function");
                        } else if (tree->child[0]->kind.var.varKind == KIND_ARRAY) {
                            printf(" Array");
                        } else {
                            printf(" Var");
                        }
                    }
                }
                printf("\n");
                break;
            case VOIDK:
                printf("[void]");
                if (tree->child[0] && tree->child[0]->nodekind == VARK && 
                    tree->child[0]->kind.var.varKind == KIND_FUNC) {
                    printf(" Function");
                }
                printf("\n");
                break;
            case IFK: printf("If\n"); break;
            case WHILEK: printf("While\n"); break;
            case RETURNK: printf("Return\n"); break;
            case COMPK: printf("Compound Statement\n"); break;
        }
    } else if (tree->nodekind == VARK) {
        printf("[%s]", tree->kind.var.attr.name);
        if (tree->kind.var.varKind == KIND_FUNC) {
            printf(" (function)");
        } else if (tree->kind.var.varKind == KIND_ARRAY) {
            printf(" (array)");
        } else if (tree->kind.var.varKind == KIND_VAR) {
            if (tree->kind.var.acesso == DECLK) {
                printf(" (declaration)");
            } else {
                printf(" (access)");
            }
        }
        printf("\n");
    } else if (tree->nodekind == EXPK) {
        switch (tree->kind.exp) {
            case OPK:
                printf("Op: ");
                switch (tree->op) {
                    case PLUS: printf("+"); break;
                    case MINUS: printf("-"); break;
                    case TIMES: printf("*"); break;
                    case DIVIDE: printf("/"); break;
                    case LT: printf("<"); break;
                    case LE: printf("<="); break;
                    case GT: printf(">"); break;
                    case GE: printf(">="); break;
                    case EQ: printf("=="); break;
                    case NE: printf("!="); break;
                    default: printf("?"); break;
                }
                printf("\n");
                break;
            case CONSTK: printf("Const: %d\n", tree->kind.var.attr.val); break;
            case IDK: printf("Id: %s\n", tree->kind.var.attr.name); break;
            case ASSIGNK: printf("Assign (=)\n"); break;
            case CALLK: printf("Call: %s()\n", tree->kind.var.attr.name); break;
            case VECTORK: printf("Vector: %s", tree->kind.var.attr.name); 
                if (tree->child[0]) printf(" [indexed]");
                printf("\n");
                break;
        }
    }
    
    /* Imprime filhos */
    for (int i = 0; i < MAXCHILDREN; i++)
        printTree(tree->child[i], indent + 1);
    
    /* Imprime irmãos */
    printTree(tree->sibling, indent);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        yyin = fopen(argv[1], "r");
        if (!yyin) {
            fprintf(stderr, "Erro: nao foi possivel abrir arquivo '%s'\n", argv[1]);
            return 1;
        }
    } else {
        yyin = stdin;
    }

    /* Cria contexto do parser */
    ParserContext *ctx = parser_context_create();

    if (!ctx) {
        fprintf(stderr, "Erro: falha ao criar o contexto do parser\n");
        if (yyin != stdin) fclose(yyin);
        return 1;
    }
    
    /* Cria escopo global */
    enter_scope_ctx(ctx);
    
    /* Adiciona funções built-in input e output */
    insert_symbol_ctx(ctx, "input", TYPE_INT, KIND_FUNC, 0);
    insert_symbol_ctx(ctx, "output", TYPE_VOID, KIND_FUNC, 0);
    
    yyparse(ctx);
    
    ExibirTabelaSimbolos_ctx(ctx);

    /* Se não houver erros, imprime a AST */
    if (!ctx->has_errors && ctx->ast_root) {
        printf("\n");
        printf("================================================================================\n");
        printf("                      ARVORE SINTATICA ABSTRATA (AST)\n");
        printf("================================================================================\n");
        /* printTree(ctx->ast_root, 0);
        printf("================================================================================\n"); */

        printTreeDOT(ctx->ast_root, "ast.dot");

        /* Gera código intermediário (três endereços) */
        codeGen(ctx->ast_root);
    }

    /* Libera todos os escopos e simbolos */
    free_all_scopes_ctx(ctx);

    int return_code = ctx->has_errors ? 1 : 0;

    parser_context_destroy(ctx);
    
    if (yyin != stdin) fclose(yyin);

    return return_code;
}

void yyerror(ParserContext *ctx, const char *s) {
    /* Processa a mensagem de erro do Bison para extrair informações úteis */
    if (strstr(s, "unexpected") != NULL) {
        char *msg = strdup(s);
        char *unexpected_pos = strstr(msg, "unexpected ");
        char *expecting_pos = strstr(msg, "expecting ");

        if (unexpected_pos && expecting_pos) {
            /* Formato: "syntax error, unexpected TOKEN, expecting TOKEN" */
            char unexpected_raw[64] = "";
            char expected_raw[512] = "";

            sscanf(unexpected_pos, "unexpected %[^,]", unexpected_raw);

            /* Extrai tudo após "expecting " até o final ou vírgula */
            char *exp_start = expecting_pos + strlen("expecting ");
            int len = 0;
            while (exp_start[len] && exp_start[len] != '\n' && len < 511) {
                expected_raw[len] = exp_start[len];
                len++;
            }
            expected_raw[len] = '\0';

            /* Traduz o token inesperado */
            const char *unexpected_translated = translate_token(unexpected_raw);

            /* Detecta casos especiais */
            if (strstr(unexpected_raw, "$end") != NULL || strcmp(unexpected_raw, "$end") == 0) {
                /* Fim de arquivo inesperado - geralmente significa algo não foi fechado */
                fprintf(stderr, "ERRO SINTATICO: fim de arquivo inesperado, possivelmente falta fechar '}', ')' ou ']' - LINHA: %d\n", yylineno);
            }
            /* Caso especial: ELSE inesperado geralmente significa else sem if */
            else if (strcmp(unexpected_raw, "ELSE") == 0) {
                fprintf(stderr, "ERRO SINTATICO: 'else' sem 'if' correspondente - LINHA: %d\n", yylineno);
            }
            /* Caso especial: quando Bison reporta apenas LBRACE mas o contexto é mais amplo */
            else if (strcmp(expected_raw, "LBRACE") == 0 &&
                     (strcmp(unexpected_raw, "RBRACE") == 0 ||
                      strcmp(unexpected_raw, "ELSE") == 0 ||
                      strcmp(unexpected_raw, "RETURN") == 0)) {
                /* Contexto sugere que estamos esperando um statement */
                fprintf(stderr, "ERRO SINTATICO: token inesperado %s, esperado um comando (if, while, return, identificador, '{', ou ';') - LINHA: %d\n",
                        unexpected_translated, yylineno);
            }
            else {
                /* Processa múltiplos tokens esperados (separados por "or") */
                char expected_translated[1024] = "";
                char *token_copy = strdup(expected_raw);
                char *saveptr = NULL;
                char *token = STRTOK_REENTRANT(token_copy, " ", &saveptr);
                int first = 1;
                int count = 0;

                while (token != NULL && count < 20) {
                    if (strcmp(token, "or") != 0) {
                        if (!first) {
                            strcat(expected_translated, " ou ");
                        }
                        const char *translated = translate_token(token);
                        strcat(expected_translated, translated);
                        first = 0;
                        count++;
                    }
                    token = STRTOK_REENTRANT(NULL, " ", &saveptr);
                }

                fprintf(stderr, "ERRO SINTATICO: token inesperado %s, esperado %s - LINHA: %d\n",
                        unexpected_translated, expected_translated, yylineno);
                free(token_copy);
            }
        } else if (unexpected_pos) {
            /* Apenas token inesperado */
            char unexpected_raw[64] = "";
            sscanf(unexpected_pos, "unexpected %s", unexpected_raw);
            const char *unexpected_translated = translate_token(unexpected_raw);
            fprintf(stderr, "ERRO SINTATICO: token inesperado %s - LINHA: %d\n",
                    unexpected_translated, yylineno);
        } else {
            fprintf(stderr, "ERRO SINTATICO: %s - LINHA: %d\n", s, yylineno);
        }

        free(msg);
    } else {
        /* Mensagem genérica */
        fprintf(stderr, "ERRO SINTATICO: %s - LINHA: %d\n", s, yylineno);
    }

    ctx->has_errors = 1;
}