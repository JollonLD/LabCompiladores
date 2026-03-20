
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* "%code requires" blocks.  */

/* Line 1676 of yacc.c  */
#line 61 "cminusSintSem.y"

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



/* Line 1676 of yacc.c  */
#line 132 "cminus.tab.h"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     ID = 258,
     NUM = 259,
     ELSE = 260,
     IF = 261,
     INT = 262,
     RETURN = 263,
     VOID = 264,
     WHILE = 265,
     LE = 266,
     LT = 267,
     GT = 268,
     GE = 269,
     EQ = 270,
     NE = 271,
     ASSIGN = 272,
     SEMI = 273,
     COMMA = 274,
     LPAREN = 275,
     RPAREN = 276,
     LBRACK = 277,
     RBRACK = 278,
     LBRACE = 279,
     RBRACE = 280,
     PLUS = 281,
     MINUS = 282,
     TIMES = 283,
     DIVIDE = 284
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 151 "cminusSintSem.y"

    int ival;
    char *id;
    TipoVar tipo;
    struct {
        TipoVar tipo;
        int is_array;
    } var_info;
    TreeNode *node;



/* Line 1676 of yacc.c  */
#line 191 "cminus.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


