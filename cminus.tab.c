
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "cminusSintSem.y"

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


/* Line 189 of yacc.c  */
#line 129 "cminus.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 209 of yacc.c  */
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



/* Line 209 of yacc.c  */
#line 245 "cminus.tab.c"

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

/* Line 214 of yacc.c  */
#line 151 "cminusSintSem.y"

    int ival;
    char *id;
    TipoVar tipo;
    struct {
        TipoVar tipo;
        int is_array;
    } var_info;
    TreeNode *node;



/* Line 214 of yacc.c  */
#line 304 "cminus.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 316 "cminus.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   96

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  67
/* YYNRULES -- Number of states.  */
#define YYNSTATES  105

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   284

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint8 yyprhs[] =
{
       0,     0,     3,     5,     8,    10,    12,    14,    18,    25,
      27,    29,    30,    38,    40,    42,    43,    47,    49,    52,
      57,    62,    65,    66,    69,    70,    72,    74,    76,    78,
      80,    81,    84,    87,    89,    95,   103,   109,   112,   116,
     120,   122,   124,   129,   133,   135,   137,   139,   141,   143,
     145,   147,   151,   153,   155,   157,   161,   163,   165,   167,
     171,   173,   175,   177,   182,   184,   185,   189
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      31,     0,    -1,    32,    -1,    32,    33,    -1,    33,    -1,
      34,    -1,    36,    -1,    35,     3,    18,    -1,    35,     3,
      22,     4,    23,    18,    -1,     7,    -1,     9,    -1,    -1,
      35,     3,    37,    20,    38,    21,    41,    -1,    39,    -1,
       9,    -1,    -1,    39,    19,    40,    -1,    40,    -1,    35,
       3,    -1,    35,     3,    22,    23,    -1,    24,    42,    43,
      25,    -1,    42,    34,    -1,    -1,    43,    44,    -1,    -1,
      47,    -1,    45,    -1,    48,    -1,    49,    -1,    50,    -1,
      -1,    46,    41,    -1,    51,    18,    -1,    18,    -1,     6,
      20,    51,    21,    44,    -1,     6,    20,    51,    21,    44,
       5,    44,    -1,    10,    20,    51,    21,    44,    -1,     8,
      18,    -1,     8,    51,    18,    -1,    52,    17,    51,    -1,
      53,    -1,     3,    -1,     3,    22,    51,    23,    -1,    55,
      54,    55,    -1,    55,    -1,    11,    -1,    12,    -1,    13,
      -1,    14,    -1,    15,    -1,    16,    -1,    55,    56,    57,
      -1,    57,    -1,    26,    -1,    27,    -1,    57,    58,    59,
      -1,    59,    -1,    28,    -1,    29,    -1,    20,    51,    21,
      -1,    52,    -1,    60,    -1,     4,    -1,     3,    20,    61,
      21,    -1,    62,    -1,    -1,    62,    19,    51,    -1,    51,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   195,   195,   209,   221,   226,   227,   232,   251,   286,
     289,   297,   296,   332,   333,   334,   339,   351,   356,   382,
     412,   422,   434,   439,   451,   456,   457,   458,   459,   460,
     464,   464,   474,   475,   480,   495,   514,   532,   538,   548,
     570,   575,   603,   635,   650,   654,   655,   656,   657,   658,
     659,   664,   679,   683,   684,   689,   704,   708,   709,   714,
     715,   716,   717,   728,   755,   756,   761,   773
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "ID", "NUM", "ELSE", "IF", "INT",
  "RETURN", "VOID", "WHILE", "LE", "LT", "GT", "GE", "EQ", "NE", "ASSIGN",
  "SEMI", "COMMA", "LPAREN", "RPAREN", "LBRACK", "RBRACK", "LBRACE",
  "RBRACE", "PLUS", "MINUS", "TIMES", "DIVIDE", "$accept", "program",
  "declaration_list", "declaration", "var_declaration", "type_specifier",
  "fun_declaration", "$@1", "params", "param_list", "param",
  "compound_stmt", "local_declarations", "statement_list", "statement",
  "compound_stmt_with_scope", "$@2", "expression_stmt", "selection_stmt",
  "iteration_stmt", "return_stmt", "expression", "var",
  "simple_expression", "relop", "additive_expression", "addop", "term",
  "mulop", "factor", "call", "args", "arg_list", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    30,    31,    32,    32,    33,    33,    34,    34,    35,
      35,    37,    36,    38,    38,    38,    39,    39,    40,    40,
      41,    42,    42,    43,    43,    44,    44,    44,    44,    44,
      46,    45,    47,    47,    48,    48,    49,    50,    50,    51,
      51,    52,    52,    53,    53,    54,    54,    54,    54,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    59,
      59,    59,    59,    60,    61,    61,    62,    62
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     6,     1,
       1,     0,     7,     1,     1,     0,     3,     1,     2,     4,
       4,     2,     0,     2,     0,     1,     1,     1,     1,     1,
       0,     2,     2,     1,     5,     7,     5,     2,     3,     3,
       1,     1,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     4,     1,     0,     3,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     9,    10,     0,     2,     4,     5,     0,     6,     1,
       3,    11,     7,     0,     0,     0,    15,     0,    10,     0,
       0,    13,    17,     8,    18,     0,     0,     0,    22,    12,
      16,    19,    24,    21,     0,    30,     0,    41,    62,     0,
       0,     0,    33,     0,    20,    23,    26,     0,    25,    27,
      28,    29,     0,    60,    40,    44,    52,    56,    61,    65,
       0,     0,    37,     0,     0,     0,    31,    32,     0,    45,
      46,    47,    48,    49,    50,    53,    54,     0,     0,    57,
      58,     0,    67,     0,    64,     0,     0,    38,     0,    59,
      39,    60,    43,    51,    55,    63,     0,    42,    30,    30,
      66,    34,    36,    30,    35
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     3,     4,     5,     6,     7,     8,    14,    20,    21,
      22,    29,    32,    35,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    77,    55,    78,    56,    81,    57,
      58,    83,    84
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -57
static const yytype_int8 yypact[] =
{
       5,   -57,   -57,    22,     5,   -57,   -57,     4,   -57,   -57,
     -57,   -13,   -57,    29,     6,    17,    52,    27,    44,    63,
      46,    49,   -57,   -57,    47,    48,     5,    50,   -57,   -57,
     -57,   -57,     5,   -57,    67,    -2,   -13,    40,   -57,    51,
      26,    54,   -57,    28,   -57,   -57,   -57,    48,   -57,   -57,
     -57,   -57,    57,    59,   -57,    23,    24,   -57,   -57,    28,
      28,    28,   -57,    60,    28,    56,   -57,   -57,    28,   -57,
     -57,   -57,   -57,   -57,   -57,   -57,   -57,    28,    28,   -57,
     -57,    28,   -57,    58,    61,    62,    65,   -57,    66,   -57,
     -57,   -57,    37,    24,   -57,   -57,    28,   -57,     7,     7,
     -57,    76,   -57,     7,   -57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,   -57,    78,    64,    25,   -57,   -57,   -57,   -57,
      68,    36,   -57,   -57,   -56,   -57,   -57,   -57,   -57,   -57,
     -57,   -40,   -23,   -57,   -57,    11,   -57,    12,   -57,     3,
     -57,   -57,   -57
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -15
static const yytype_int8 yytable[] =
{
      63,    37,    38,    65,    39,    12,    40,    11,    41,    13,
      37,    38,     1,    39,     2,    40,    42,    41,    43,    82,
      85,    86,     9,    44,    88,    42,    16,    43,    90,    37,
      38,    37,    38,    15,    69,    70,    71,    72,    73,    74,
      17,    19,   101,   102,    62,    23,    43,   104,    43,    75,
      76,    19,    79,    80,    91,    91,   100,    34,    91,     1,
      59,    18,    60,    75,    76,   -14,    24,    25,    26,    27,
      36,    61,    28,    31,    64,    67,    68,    89,    87,    95,
      96,   103,    10,    66,    94,    97,    98,    99,    92,     0,
      93,     0,     0,     0,    30,     0,    33
};

static const yytype_int8 yycheck[] =
{
      40,     3,     4,    43,     6,    18,     8,     3,    10,    22,
       3,     4,     7,     6,     9,     8,    18,    10,    20,    59,
      60,    61,     0,    25,    64,    18,    20,    20,    68,     3,
       4,     3,     4,     4,    11,    12,    13,    14,    15,    16,
      23,    16,    98,    99,    18,    18,    20,   103,    20,    26,
      27,    26,    28,    29,    77,    78,    96,    32,    81,     7,
      20,     9,    22,    26,    27,    21,     3,    21,    19,    22,
       3,    20,    24,    23,    20,    18,    17,    21,    18,    21,
      19,     5,     4,    47,    81,    23,    21,    21,    77,    -1,
      78,    -1,    -1,    -1,    26,    -1,    32
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     7,     9,    31,    32,    33,    34,    35,    36,     0,
      33,     3,    18,    22,    37,     4,    20,    23,     9,    35,
      38,    39,    40,    18,     3,    21,    19,    22,    24,    41,
      40,    23,    42,    34,    35,    43,     3,     3,     4,     6,
       8,    10,    18,    20,    25,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    55,    57,    59,    60,    20,
      22,    20,    18,    51,    20,    51,    41,    18,    17,    11,
      12,    13,    14,    15,    16,    26,    27,    54,    56,    28,
      29,    58,    51,    61,    62,    51,    51,    18,    51,    21,
      51,    52,    55,    57,    59,    21,    19,    23,    21,    21,
      51,    44,    44,     5,    44
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (ctx, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, ctx); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, ParserContext *ctx)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, ctx)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    ParserContext *ctx;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (ctx);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, ParserContext *ctx)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, ctx)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    ParserContext *ctx;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, ctx);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, ParserContext *ctx)
#else
static void
yy_reduce_print (yyvsp, yyrule, ctx)
    YYSTYPE *yyvsp;
    int yyrule;
    ParserContext *ctx;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, ctx); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, ParserContext *ctx)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, ctx)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    ParserContext *ctx;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (ctx);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (ParserContext *ctx);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (ParserContext *ctx)
#else
int
yyparse (ctx)
    ParserContext *ctx;
#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1455 of yacc.c  */
#line 196 "cminusSintSem.y"
    { 
        (yyval.node) = (yyvsp[(1) - (1)].node);
        ctx->ast_root = (yyval.node);
        printf("Analise sintatica concluida com sucesso!\n"); 
        if (!ctx->has_errors) {
            printf("Analise semantica: Nenhum erro encontrado.\n");
        }
        /* Impressão da AST movida para a main */
    ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 210 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[(1) - (2)].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[(2) - (2)].node);
            (yyval.node) = (yyvsp[(1) - (2)].node);
        } else {
            (yyval.node) = (yyvsp[(2) - (2)].node);
        }
    ;}
    break;

  case 4:

/* Line 1455 of yacc.c  */
#line 221 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 5:

/* Line 1455 of yacc.c  */
#line 226 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 6:

/* Line 1455 of yacc.c  */
#line 227 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 7:

/* Line 1455 of yacc.c  */
#line 233 "cminusSintSem.y"
    {
        /* Análise semântica */
        insert_symbol_ctx(ctx, (yyvsp[(2) - (3)].id), (yyvsp[(1) - (3)].tipo), KIND_VAR, yylineno);
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[(1) - (3)].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[(1) - (3)].tipo);
        
        /* Criar nó ID e anexar como filho */
        (yyval.node)->child[0] = newVarNode(IDK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[(2) - (3)].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_VAR;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[(1) - (3)].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        free((yyvsp[(2) - (3)].id));
    ;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 252 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[(1) - (6)].tipo) == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: array '%s' nao pode ser void - LINHA: %d\n",
                    (yyvsp[(2) - (6)].id), yylineno);
            ctx->has_errors = 1;
        } else {
            insert_array_ctx(ctx, (yyvsp[(2) - (6)].id), (yyvsp[(4) - (6)].ival), yylineno);
        }
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[(1) - (6)].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[(1) - (6)].tipo);
        
        /* Criar nó ID array e anexar como filho */
        (yyval.node)->child[0] = newVarNode(VECTORK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[(2) - (6)].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_ARRAY;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[(1) - (6)].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        /* Adicionar tamanho do array como filho do nó array */
        (yyval.node)->child[0]->child[0] = newExpNode(CONSTK);
        (yyval.node)->child[0]->child[0]->kind.var.attr.val = (yyvsp[(4) - (6)].ival);
        (yyval.node)->child[0]->child[0]->type = TYPE_INT;
        (yyval.node)->child[0]->child[0]->lineno = yylineno;
        
        free((yyvsp[(2) - (6)].id));
    ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 286 "cminusSintSem.y"
    { 
        (yyval.tipo) = TYPE_INT;
    ;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 289 "cminusSintSem.y"
    { 
        (yyval.tipo) = TYPE_VOID;
    ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 297 "cminusSintSem.y"
    {
        /* Análise semântica */
        insert_function_ctx(ctx, (yyvsp[(2) - (2)].id), (yyvsp[(1) - (2)].tipo), yylineno);
        Simbolo *func_sym = lookup_symbol_current_ctx(ctx, (yyvsp[(2) - (2)].id));
        enter_scope_ctx(ctx);
        if (func_sym) {
            func_sym->def_scope = ctx->escopo_atual;
        }
    ;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 307 "cminusSintSem.y"
    {
        leave_scope_ctx(ctx);
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[(1) - (7)].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[(1) - (7)].tipo);
        
        /* Criar nó ID função e anexar como filho */
        (yyval.node)->child[0] = newVarNode(IDK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[(2) - (7)].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_FUNC;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[(1) - (7)].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        /* Adicionar parâmetros e corpo como filhos do nó função */
        (yyval.node)->child[0]->child[0] = (yyvsp[(5) - (7)].node);  /* parâmetros */
        (yyval.node)->child[0]->child[1] = (yyvsp[(7) - (7)].node);  /* corpo */
        
        free((yyvsp[(2) - (7)].id));
    ;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 332 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 333 "cminusSintSem.y"
    { (yyval.node) = NULL; ;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 334 "cminusSintSem.y"
    { (yyval.node) = NULL; ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 340 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[(1) - (3)].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[(3) - (3)].node);
            (yyval.node) = (yyvsp[(1) - (3)].node);
        } else {
            (yyval.node) = (yyvsp[(3) - (3)].node);
        }
    ;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 351 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 357 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[(1) - (2)].tipo) == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: parametro '%s' nao pode ser void - LINHA: %d\n",
                    (yyvsp[(2) - (2)].id), yylineno);
            ctx->has_errors = 1;
        } else {
            insert_symbol_ctx(ctx, (yyvsp[(2) - (2)].id), (yyvsp[(1) - (2)].tipo), KIND_VAR, yylineno);
            Simbolo *p = lookup_symbol_current_ctx(ctx, (yyvsp[(2) - (2)].id));
            if (p) p->is_param = 1;
        }
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[(1) - (2)].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[(1) - (2)].tipo);
        
        (yyval.node)->child[0] = newVarNode(IDK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[(2) - (2)].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_VAR;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[(1) - (2)].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        free((yyvsp[(2) - (2)].id));
    ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 383 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[(1) - (4)].tipo) == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: parametro array '%s' nao pode ser void - LINHA: %d\n",
                    (yyvsp[(2) - (4)].id), yylineno);
            ctx->has_errors = 1;
        } else {
            insert_symbol_ctx(ctx, (yyvsp[(2) - (4)].id), TYPE_INT_ARRAY, KIND_ARRAY, yylineno);
            Simbolo *p = lookup_symbol_current_ctx(ctx, (yyvsp[(2) - (4)].id));
            if (p) p->is_param = 1;
        }
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[(1) - (4)].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[(1) - (4)].tipo);
        
        (yyval.node)->child[0] = newVarNode(VECTORK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[(2) - (4)].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_ARRAY;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = TYPE_INT_ARRAY;
        (yyval.node)->child[0]->lineno = yylineno;
        
        free((yyvsp[(2) - (4)].id));
    ;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 413 "cminusSintSem.y"
    {
        (yyval.node) = newStmtNode(COMPK);
        (yyval.node)->child[0] = (yyvsp[(2) - (4)].node);  /* declarações locais */
        (yyval.node)->child[1] = (yyvsp[(3) - (4)].node);  /* lista de statements */
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 423 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[(1) - (2)].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[(2) - (2)].node);
            (yyval.node) = (yyvsp[(1) - (2)].node);
        } else {
            (yyval.node) = (yyvsp[(2) - (2)].node);
        }
    ;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 434 "cminusSintSem.y"
    { (yyval.node) = NULL; ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 440 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[(1) - (2)].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[(2) - (2)].node);
            (yyval.node) = (yyvsp[(1) - (2)].node);
        } else {
            (yyval.node) = (yyvsp[(2) - (2)].node);
        }
    ;}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 451 "cminusSintSem.y"
    { (yyval.node) = NULL; ;}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 456 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 457 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 458 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 459 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 460 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 464 "cminusSintSem.y"
    { enter_scope_ctx(ctx); ;}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 466 "cminusSintSem.y"
    { 
        leave_scope_ctx(ctx);
        (yyval.node) = (yyvsp[(2) - (2)].node);
    ;}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 474 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (2)].node); ;}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 475 "cminusSintSem.y"
    { (yyval.node) = NULL; ;}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 481 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[(3) - (5)].node) && (yyvsp[(3) - (5)].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do IF deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }

        /* Construção da AST */
        (yyval.node) = newStmtNode(IFK);
        (yyval.node)->child[0] = (yyvsp[(3) - (5)].node);    /* condição */
        (yyval.node)->child[1] = (yyvsp[(5) - (5)].node);    /* then */
        (yyval.node)->child[2] = NULL;  /* else */
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 496 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[(3) - (7)].node) && (yyvsp[(3) - (7)].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do IF deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }

        /* Construção da AST */
        (yyval.node) = newStmtNode(IFK);
        (yyval.node)->child[0] = (yyvsp[(3) - (7)].node);    /* condição */
        (yyval.node)->child[1] = (yyvsp[(5) - (7)].node);    /* then */
        (yyval.node)->child[2] = (yyvsp[(7) - (7)].node);    /* else */
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 515 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[(3) - (5)].node) && (yyvsp[(3) - (5)].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do WHILE deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }
        
        /* Construção da AST */
        (yyval.node) = newStmtNode(WHILEK);
        (yyval.node)->child[0] = (yyvsp[(3) - (5)].node);    /* condição */
        (yyval.node)->child[1] = (yyvsp[(5) - (5)].node);    /* corpo */
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 533 "cminusSintSem.y"
    {
        (yyval.node) = newStmtNode(RETURNK);
        (yyval.node)->child[0] = NULL;  /* sem expressão */
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 539 "cminusSintSem.y"
    {
        (yyval.node) = newStmtNode(RETURNK);
        (yyval.node)->child[0] = (yyvsp[(2) - (3)].node);    /* expressão */
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 549 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar var_type = (yyvsp[(1) - (3)].node) ? (yyvsp[(1) - (3)].node)->type : TYPE_ERROR;
        TipoVar exp_type = (yyvsp[(3) - (3)].node) ? (yyvsp[(3) - (3)].node)->type : TYPE_ERROR;
        
        if ((yyvsp[(1) - (3)].node) && (yyvsp[(1) - (3)].node)->nodekind == VARK && (yyvsp[(1) - (3)].node)->kind.var.varKind == KIND_ARRAY &&
            (yyvsp[(1) - (3)].node)->child[0] == NULL) {
            fprintf(stderr, "ERRO SEMANTICO: nao e possivel atribuir a array completo - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        } else if (var_type != exp_type && exp_type != TYPE_ERROR && var_type != TYPE_ERROR) {
            fprintf(stderr, "ERRO SEMANTICO: tipos incompativeis na atribuicao - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }
        
        /* Construção da AST */
        (yyval.node) = newExpNode(ASSIGNK);
        (yyval.node)->child[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->child[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->type = var_type;
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 570 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 576 "cminusSintSem.y"
    {
        Simbolo *s = lookup_symbol_ctx(ctx, (yyvsp[(1) - (1)].id));
        TipoVar tipo = TYPE_ERROR;
        TipoSimbolo kind = KIND_VAR;
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada - LINHA: %d\n", (yyvsp[(1) - (1)].id), yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = s->tipo;
            kind = s->kind;
        }
        
        /* Cria nó de variável */
        if (kind == KIND_ARRAY) {
            (yyval.node) = newVarNode(VECTORK);
            (yyval.node)->type = TYPE_INT_ARRAY;
        } else {
            (yyval.node) = newVarNode(IDK);
            (yyval.node)->type = tipo;
        }
        (yyval.node)->kind.var.attr.name = strdup((yyvsp[(1) - (1)].id));
        (yyval.node)->kind.var.varKind = kind;
        (yyval.node)->kind.var.acesso = ACCESSK;
        (yyval.node)->lineno = yylineno;
        free((yyvsp[(1) - (1)].id));
    ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 604 "cminusSintSem.y"
    {
        Simbolo *s = lookup_symbol_ctx(ctx, (yyvsp[(1) - (4)].id));
        TipoVar tipo = TYPE_ERROR;
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada - LINHA: %d\n", (yyvsp[(1) - (4)].id), yylineno);
            ctx->has_errors = 1;
        } else if (s->kind != KIND_ARRAY) {
            fprintf(stderr, "ERRO SEMANTICO: '%s' nao e um array - LINHA: %d\n", (yyvsp[(1) - (4)].id), yylineno);
            ctx->has_errors = 1;
        } else if ((yyvsp[(3) - (4)].node) && (yyvsp[(3) - (4)].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: indice de array deve ser inteiro - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = TYPE_INT;
        }
        
        /* Cria nó de acesso a array */
        (yyval.node) = newVarNode(VECTORK);
        (yyval.node)->kind.var.attr.name = strdup((yyvsp[(1) - (4)].id));
        (yyval.node)->kind.var.varKind = KIND_ARRAY;
        (yyval.node)->kind.var.acesso = ACCESSK;
        (yyval.node)->child[0] = (yyvsp[(3) - (4)].node);  /* índice */
        (yyval.node)->type = tipo;
        (yyval.node)->lineno = yylineno;
        free((yyvsp[(1) - (4)].id));
    ;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 636 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar t1 = (yyvsp[(1) - (3)].node) ? (yyvsp[(1) - (3)].node)->type : TYPE_ERROR;
        TipoVar t2 = (yyvsp[(3) - (3)].node) ? (yyvsp[(3) - (3)].node)->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "relacional", t1, t2, yylineno);
        
        /* Construção da AST */
        (yyval.node) = newExpNode(OPK);
        (yyval.node)->child[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->child[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->op = (yyvsp[(2) - (3)].ival);  /* operador relacional */
        (yyval.node)->type = result;
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 650 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 654 "cminusSintSem.y"
    { (yyval.ival) = LE; ;}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 655 "cminusSintSem.y"
    { (yyval.ival) = LT; ;}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 656 "cminusSintSem.y"
    { (yyval.ival) = GT; ;}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 657 "cminusSintSem.y"
    { (yyval.ival) = GE; ;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 658 "cminusSintSem.y"
    { (yyval.ival) = EQ; ;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 659 "cminusSintSem.y"
    { (yyval.ival) = NE; ;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 665 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar t1 = (yyvsp[(1) - (3)].node) ? (yyvsp[(1) - (3)].node)->type : TYPE_ERROR;
        TipoVar t2 = (yyvsp[(3) - (3)].node) ? (yyvsp[(3) - (3)].node)->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "aditivo", t1, t2, yylineno);
        
        /* Construção da AST */
        (yyval.node) = newExpNode(OPK);
        (yyval.node)->child[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->child[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->op = (yyvsp[(2) - (3)].ival);  /* operador aditivo */
        (yyval.node)->type = result;
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 679 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 683 "cminusSintSem.y"
    { (yyval.ival) = PLUS; ;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 684 "cminusSintSem.y"
    { (yyval.ival) = MINUS; ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 690 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar t1 = (yyvsp[(1) - (3)].node) ? (yyvsp[(1) - (3)].node)->type : TYPE_ERROR;
        TipoVar t2 = (yyvsp[(3) - (3)].node) ? (yyvsp[(3) - (3)].node)->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "multiplicativo", t1, t2, yylineno);
        
        /* Construção da AST */
        (yyval.node) = newExpNode(OPK);
        (yyval.node)->child[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->child[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->op = (yyvsp[(2) - (3)].ival);  /* operador multiplicativo */
        (yyval.node)->type = result;
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 704 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 708 "cminusSintSem.y"
    { (yyval.ival) = TIMES; ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 709 "cminusSintSem.y"
    { (yyval.ival) = DIVIDE; ;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 714 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 715 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 716 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 718 "cminusSintSem.y"
    { 
        (yyval.node) = newExpNode(CONSTK);
        (yyval.node)->kind.var.attr.val = (yyvsp[(1) - (1)].ival);
        (yyval.node)->type = TYPE_INT;
        (yyval.node)->lineno = yylineno;
    ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 729 "cminusSintSem.y"
    {
        Simbolo *s = lookup_symbol_ctx(ctx, (yyvsp[(1) - (4)].id));
        TipoVar tipo = TYPE_INT;  /* padrão */
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: funcao '%s' nao declarada - LINHA: %d\n", (yyvsp[(1) - (4)].id), yylineno);
            ctx->has_errors = 1;
        } else if (s->kind != KIND_FUNC) {
            fprintf(stderr, "ERRO SEMANTICO: '%s' nao e uma funcao - LINHA: %d\n", (yyvsp[(1) - (4)].id), yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = s->tipo;  /* tipo de retorno da função */
        }
        
        /* Cria nó de chamada de função */
        (yyval.node) = newExpNode(CALLK);
        (yyval.node)->kind.var.attr.name = strdup((yyvsp[(1) - (4)].id));
        (yyval.node)->child[0] = (yyvsp[(3) - (4)].node);  /* argumentos */
        (yyval.node)->type = tipo;
        (yyval.node)->lineno = yylineno;
        free((yyvsp[(1) - (4)].id));
    ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 755 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 756 "cminusSintSem.y"
    { (yyval.node) = NULL; ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 762 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[(1) - (3)].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[(3) - (3)].node);
            (yyval.node) = (yyvsp[(1) - (3)].node);
        } else {
            (yyval.node) = (yyvsp[(3) - (3)].node);
        }
    ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 773 "cminusSintSem.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;



/* Line 1455 of yacc.c  */
#line 2447 "cminus.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (ctx, YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (ctx, yymsg);
	  }
	else
	  {
	    yyerror (ctx, YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval, ctx);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp, ctx);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (ctx, YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval, ctx);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, ctx);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 776 "cminusSintSem.y"


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

static int printNodeDOT_simplified(DotContext *ctx, TreeNode *tree) {
    if (tree == NULL) return -1;

    int current_id = ctx->node_counter++;
    FILE *fp = ctx->fp;

    /* Define cor e forma do no */
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
    } else if (tree->nodekind == VARK) {
        char *esc_name = escape_label(tree->kind.var.attr.name);
        if (tree->kind.var.varKind == KIND_FUNC) {
            snprintf(label, sizeof(label), "Function\\n%s", esc_name);
            color = "gold";
            shape = "ellipse";
        } else if (tree->kind.var.varKind == KIND_ARRAY) {
            if (tree->kind.var.acesso == DECLK) {
                snprintf(label, sizeof(label), "Array\\n%s", esc_name);
                color = "lightgreen";
            } else {
                snprintf(label, sizeof(label), "%s[]", esc_name);
                color = "lightyellow";
            }
        } else {
            if (tree->kind.var.acesso == DECLK) {
                snprintf(label, sizeof(label), "Var\\n%s", esc_name);
                color = "lightgreen";
            } else {
                snprintf(label, sizeof(label), "%s", esc_name);
                color = "lightyellow";
            }
        }
        free(esc_name);
    } else if (tree->nodekind == EXPK) {
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
                break;
            }
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
                break;
            }
            case VECTORK: {
                char *esc_name = escape_label(tree->kind.var.attr.name);
                snprintf(label, sizeof(label), "%s[]", esc_name);
                free(esc_name);
                color = "lightcyan";
                break;
            }
        }
    }

    fprintf(fp, "  node%d [label=\"%s\", shape=%s, style=\"filled\", fillcolor=%s];\n", current_id, label, shape, color);
    return current_id;
}

static void printSiblingChainDOT_simplified(DotContext *ctx, TreeNode *first, int parent_id) {
    if (first == NULL) return;

    FILE *fp = ctx->fp;
    int ids[512];
    int count = 0;
    TreeNode *cur = first;

    /* Cria todos os nos da cadeia de irmaos desse nivel */
    while (cur != NULL && count < 512) {
        ids[count++] = printNodeDOT_simplified(ctx, cur);
        cur = cur->sibling;
    }

    /* Liga pai -> cada irmao (vertical) */
    if (parent_id >= 0) {
        fprintf(fp, "  node%d -> node%d [weight=10];\n", parent_id, ids[0]);
        for (int i = 1; i < count; i++) {
            fprintf(fp, "  node%d -> node%d [style=invis, weight=10];\n", parent_id, ids[i]);
        }
    }

    /* Irmaos no mesmo rank e ligados com aresta dashed */
    if (count > 1) {
        fprintf(fp, "  { rank=same; ");
        for (int i = 0; i < count; i++) {
            fprintf(fp, "node%d; ", ids[i]);
        }
        fprintf(fp, "}\n");

        for (int i = 0; i < count - 1; i++) {
            fprintf(fp, "  node%d -> node%d [style=dashed, color=gray, constraint=false, minlen=2];\n", ids[i], ids[i + 1]);
        }
    }

    /* Processa ate 3 filhos de cada no dessa cadeia */
    cur = first;
    for (int i = 0; i < count; i++, cur = cur->sibling) {
        for (int c = 0; c < MAXCHILDREN; c++) {
            if (cur->child[c] != NULL) {
                printSiblingChainDOT_simplified(ctx, cur->child[c], ids[i]);
            }
        }
    }
}

/* Mantido para compatibilidade com chamadas antigas */
static int printTreeDOT_simplified(DotContext *ctx, TreeNode *tree, int parent_id) {
    if (tree == NULL) return -1;
    int before = ctx->node_counter;
    printSiblingChainDOT_simplified(ctx, tree, parent_id);
    return before;
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


    printSiblingChainDOT_simplified(&ctx, tree, -1);

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
