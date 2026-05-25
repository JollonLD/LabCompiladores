/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_CMINUS_TAB_H_INCLUDED
# define YY_YY_CMINUS_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
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

#line 139 "cminus.tab.h"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    NUM = 259,                     /* NUM  */
    ELSE = 260,                    /* ELSE  */
    IF = 261,                      /* IF  */
    INT = 262,                     /* INT  */
    RETURN = 263,                  /* RETURN  */
    VOID = 264,                    /* VOID  */
    WHILE = 265,                   /* WHILE  */
    LE = 266,                      /* LE  */
    LT = 267,                      /* LT  */
    GT = 268,                      /* GT  */
    GE = 269,                      /* GE  */
    EQ = 270,                      /* EQ  */
    NE = 271,                      /* NE  */
    ASSIGN = 272,                  /* ASSIGN  */
    SEMI = 273,                    /* SEMI  */
    COMMA = 274,                   /* COMMA  */
    LPAREN = 275,                  /* LPAREN  */
    RPAREN = 276,                  /* RPAREN  */
    LBRACK = 277,                  /* LBRACK  */
    RBRACK = 278,                  /* RBRACK  */
    LBRACE = 279,                  /* LBRACE  */
    RBRACE = 280,                  /* RBRACE  */
    PLUS = 281,                    /* PLUS  */
    MINUS = 282,                   /* MINUS  */
    TIMES = 283,                   /* TIMES  */
    DIVIDE = 284                   /* DIVIDE  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 151 "cminusSintSem.y"

    int ival;
    char *id;
    TipoVar tipo;
    struct {
        TipoVar tipo;
        int is_array;
    } var_info;
    TreeNode *node;

#line 196 "cminus.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (ParserContext *ctx);


#endif /* !YY_YY_CMINUS_TAB_H_INCLUDED  */
