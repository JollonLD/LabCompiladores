/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
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

#if defined(_MSC_VER)
#define STRTOK_REENTRANT(str, delim, saveptr) strtok_s((str), (delim), (saveptr))
#elif defined(__MINGW32__) || defined(__MINGW64__)
static char *cminus_strtok_r(char *str, const char *delim, char **saveptr) {
    char *inicio;

    if (str != NULL)
        *saveptr = str;
    if (*saveptr == NULL || **saveptr == '\0')
        return NULL;

    inicio = *saveptr;
    while (**saveptr != '\0' && strchr(delim, **saveptr) == NULL)
        (*saveptr)++;
    if (**saveptr != '\0') {
        **saveptr = '\0';
        (*saveptr)++;
    } else {
        *saveptr = NULL;
    }
    return inicio;
}
#define STRTOK_REENTRANT(str, delim, saveptr) cminus_strtok_r((str), (delim), (saveptr))
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

#line 126 "cminus.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "cminus.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_NUM = 4,                        /* NUM  */
  YYSYMBOL_ELSE = 5,                       /* ELSE  */
  YYSYMBOL_IF = 6,                         /* IF  */
  YYSYMBOL_INT = 7,                        /* INT  */
  YYSYMBOL_RETURN = 8,                     /* RETURN  */
  YYSYMBOL_VOID = 9,                       /* VOID  */
  YYSYMBOL_WHILE = 10,                     /* WHILE  */
  YYSYMBOL_LE = 11,                        /* LE  */
  YYSYMBOL_LT = 12,                        /* LT  */
  YYSYMBOL_GT = 13,                        /* GT  */
  YYSYMBOL_GE = 14,                        /* GE  */
  YYSYMBOL_EQ = 15,                        /* EQ  */
  YYSYMBOL_NE = 16,                        /* NE  */
  YYSYMBOL_ASSIGN = 17,                    /* ASSIGN  */
  YYSYMBOL_SEMI = 18,                      /* SEMI  */
  YYSYMBOL_COMMA = 19,                     /* COMMA  */
  YYSYMBOL_LPAREN = 20,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 21,                    /* RPAREN  */
  YYSYMBOL_LBRACK = 22,                    /* LBRACK  */
  YYSYMBOL_RBRACK = 23,                    /* RBRACK  */
  YYSYMBOL_LBRACE = 24,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 25,                    /* RBRACE  */
  YYSYMBOL_PLUS = 26,                      /* PLUS  */
  YYSYMBOL_MINUS = 27,                     /* MINUS  */
  YYSYMBOL_TIMES = 28,                     /* TIMES  */
  YYSYMBOL_DIVIDE = 29,                    /* DIVIDE  */
  YYSYMBOL_YYACCEPT = 30,                  /* $accept  */
  YYSYMBOL_program = 31,                   /* program  */
  YYSYMBOL_declaration_list = 32,          /* declaration_list  */
  YYSYMBOL_declaration = 33,               /* declaration  */
  YYSYMBOL_var_declaration = 34,           /* var_declaration  */
  YYSYMBOL_type_specifier = 35,            /* type_specifier  */
  YYSYMBOL_fun_declaration = 36,           /* fun_declaration  */
  YYSYMBOL_37_1 = 37,                      /* $@1  */
  YYSYMBOL_params = 38,                    /* params  */
  YYSYMBOL_param_list = 39,                /* param_list  */
  YYSYMBOL_param = 40,                     /* param  */
  YYSYMBOL_compound_stmt = 41,             /* compound_stmt  */
  YYSYMBOL_local_declarations = 42,        /* local_declarations  */
  YYSYMBOL_statement_list = 43,            /* statement_list  */
  YYSYMBOL_statement = 44,                 /* statement  */
  YYSYMBOL_compound_stmt_with_scope = 45,  /* compound_stmt_with_scope  */
  YYSYMBOL_46_2 = 46,                      /* $@2  */
  YYSYMBOL_expression_stmt = 47,           /* expression_stmt  */
  YYSYMBOL_selection_stmt = 48,            /* selection_stmt  */
  YYSYMBOL_iteration_stmt = 49,            /* iteration_stmt  */
  YYSYMBOL_return_stmt = 50,               /* return_stmt  */
  YYSYMBOL_expression = 51,                /* expression  */
  YYSYMBOL_var = 52,                       /* var  */
  YYSYMBOL_simple_expression = 53,         /* simple_expression  */
  YYSYMBOL_relop = 54,                     /* relop  */
  YYSYMBOL_additive_expression = 55,       /* additive_expression  */
  YYSYMBOL_addop = 56,                     /* addop  */
  YYSYMBOL_term = 57,                      /* term  */
  YYSYMBOL_mulop = 58,                     /* mulop  */
  YYSYMBOL_factor = 59,                    /* factor  */
  YYSYMBOL_call = 60,                      /* call  */
  YYSYMBOL_args = 61,                      /* args  */
  YYSYMBOL_arg_list = 62                   /* arg_list  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

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
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  105

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
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

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "NUM", "ELSE",
  "IF", "INT", "RETURN", "VOID", "WHILE", "LE", "LT", "GT", "GE", "EQ",
  "NE", "ASSIGN", "SEMI", "COMMA", "LPAREN", "RPAREN", "LBRACK", "RBRACK",
  "LBRACE", "RBRACE", "PLUS", "MINUS", "TIMES", "DIVIDE", "$accept",
  "program", "declaration_list", "declaration", "var_declaration",
  "type_specifier", "fun_declaration", "$@1", "params", "param_list",
  "param", "compound_stmt", "local_declarations", "statement_list",
  "statement", "compound_stmt_with_scope", "$@2", "expression_stmt",
  "selection_stmt", "iteration_stmt", "return_stmt", "expression", "var",
  "simple_expression", "relop", "additive_expression", "addop", "term",
  "mulop", "factor", "call", "args", "arg_list", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-57)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-15)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
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

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
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

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -57,   -57,   -57,    78,    64,    25,   -57,   -57,   -57,   -57,
      68,    36,   -57,   -57,   -56,   -57,   -57,   -57,   -57,   -57,
     -57,   -40,   -23,   -57,   -57,    11,   -57,    12,   -57,     3,
     -57,   -57,   -57
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     3,     4,     5,     6,     7,     8,    14,    20,    21,
      22,    29,    32,    35,    45,    46,    47,    48,    49,    50,
      51,    52,    53,    54,    77,    55,    78,    56,    81,    57,
      58,    83,    84
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
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

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
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

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    30,    31,    32,    32,    33,    33,    34,    34,    35,
      35,    37,    36,    38,    38,    38,    39,    39,    40,    40,
      41,    42,    42,    43,    43,    44,    44,    44,    44,    44,
      46,    45,    47,    47,    48,    48,    49,    50,    50,    51,
      51,    52,    52,    53,    53,    54,    54,    54,    54,    54,
      54,    55,    55,    56,    56,    57,    57,    58,    58,    59,
      59,    59,    59,    60,    61,    61,    62,    62
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     6,     1,
       1,     0,     7,     1,     1,     0,     3,     1,     2,     4,
       4,     2,     0,     2,     0,     1,     1,     1,     1,     1,
       0,     2,     2,     1,     5,     7,     5,     2,     3,     3,
       1,     1,     4,     3,     1,     1,     1,     1,     1,     1,
       1,     3,     1,     1,     1,     3,     1,     1,     1,     3,
       1,     1,     1,     4,     1,     0,     3,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (ctx, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, ctx); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ParserContext *ctx)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (ctx);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, ParserContext *ctx)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, ctx);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, ParserContext *ctx)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], ctx);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, ctx); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
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
            else
              goto append;

          append:
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

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, ParserContext *ctx)
{
  YY_USE (yyvaluep);
  YY_USE (ctx);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (ParserContext *ctx)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: declaration_list  */
#line 196 "cminusSintSem.y"
    { 
        (yyval.node) = (yyvsp[0].node);
        ctx->ast_root = (yyval.node);
        printf("Analise sintatica concluida com sucesso!\n"); 
        if (!ctx->has_errors) {
            printf("Analise semantica: Nenhum erro encontrado.\n");
        }
        /* Impressão da AST movida para a main */
    }
#line 1525 "cminus.tab.c"
    break;

  case 3: /* declaration_list: declaration_list declaration  */
#line 210 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        } else {
            (yyval.node) = (yyvsp[0].node);
        }
    }
#line 1541 "cminus.tab.c"
    break;

  case 4: /* declaration_list: declaration  */
#line 221 "cminusSintSem.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1547 "cminus.tab.c"
    break;

  case 5: /* declaration: var_declaration  */
#line 226 "cminusSintSem.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1553 "cminus.tab.c"
    break;

  case 6: /* declaration: fun_declaration  */
#line 227 "cminusSintSem.y"
                      { (yyval.node) = (yyvsp[0].node); }
#line 1559 "cminus.tab.c"
    break;

  case 7: /* var_declaration: type_specifier ID SEMI  */
#line 233 "cminusSintSem.y"
    {
        /* Análise semântica */
        insert_symbol_ctx(ctx, (yyvsp[-1].id), (yyvsp[-2].tipo), KIND_VAR, yylineno);
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[-2].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[-2].tipo);
        
        /* Criar nó ID e anexar como filho */
        (yyval.node)->child[0] = newVarNode(IDK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[-1].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_VAR;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[-2].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        free((yyvsp[-1].id));
    }
#line 1582 "cminus.tab.c"
    break;

  case 8: /* var_declaration: type_specifier ID LBRACK NUM RBRACK SEMI  */
#line 252 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[-5].tipo) == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: array '%s' nao pode ser void - LINHA: %d\n",
                    (yyvsp[-4].id), yylineno);
            ctx->has_errors = 1;
        } else {
            insert_array_ctx(ctx, (yyvsp[-4].id), (yyvsp[-2].ival), yylineno);
        }
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[-5].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[-5].tipo);
        
        /* Criar nó ID array e anexar como filho */
        (yyval.node)->child[0] = newVarNode(VECTORK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[-4].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_ARRAY;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[-5].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        /* Adicionar tamanho do array como filho do nó array */
        (yyval.node)->child[0]->child[0] = newExpNode(CONSTK);
        (yyval.node)->child[0]->child[0]->kind.var.attr.val = (yyvsp[-2].ival);
        (yyval.node)->child[0]->child[0]->type = TYPE_INT;
        (yyval.node)->child[0]->child[0]->lineno = yylineno;
        
        free((yyvsp[-4].id));
    }
#line 1617 "cminus.tab.c"
    break;

  case 9: /* type_specifier: INT  */
#line 286 "cminusSintSem.y"
         { 
        (yyval.tipo) = TYPE_INT;
    }
#line 1625 "cminus.tab.c"
    break;

  case 10: /* type_specifier: VOID  */
#line 289 "cminusSintSem.y"
           { 
        (yyval.tipo) = TYPE_VOID;
    }
#line 1633 "cminus.tab.c"
    break;

  case 11: /* $@1: %empty  */
#line 297 "cminusSintSem.y"
    {
        /* Análise semântica */
        insert_function_ctx(ctx, (yyvsp[0].id), (yyvsp[-1].tipo), yylineno);
        Simbolo *func_sym = lookup_symbol_current_ctx(ctx, (yyvsp[0].id));
        enter_scope_ctx(ctx);
        if (func_sym) {
            func_sym->def_scope = ctx->escopo_atual;
        }
    }
#line 1647 "cminus.tab.c"
    break;

  case 12: /* fun_declaration: type_specifier ID $@1 LPAREN params RPAREN compound_stmt  */
#line 307 "cminusSintSem.y"
    {
        leave_scope_ctx(ctx);
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[-6].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[-6].tipo);
        
        /* Criar nó ID função e anexar como filho */
        (yyval.node)->child[0] = newVarNode(IDK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[-5].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_FUNC;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[-6].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        /* Adicionar parâmetros e corpo como filhos do nó função */
        (yyval.node)->child[0]->child[0] = (yyvsp[-2].node);  /* parâmetros */
        (yyval.node)->child[0]->child[1] = (yyvsp[0].node);  /* corpo */
        
        free((yyvsp[-5].id));
    }
#line 1673 "cminus.tab.c"
    break;

  case 13: /* params: param_list  */
#line 332 "cminusSintSem.y"
               { (yyval.node) = (yyvsp[0].node); }
#line 1679 "cminus.tab.c"
    break;

  case 14: /* params: VOID  */
#line 333 "cminusSintSem.y"
           { (yyval.node) = NULL; }
#line 1685 "cminus.tab.c"
    break;

  case 15: /* params: %empty  */
#line 334 "cminusSintSem.y"
                  { (yyval.node) = NULL; }
#line 1691 "cminus.tab.c"
    break;

  case 16: /* param_list: param_list COMMA param  */
#line 340 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[-2].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        } else {
            (yyval.node) = (yyvsp[0].node);
        }
    }
#line 1707 "cminus.tab.c"
    break;

  case 17: /* param_list: param  */
#line 351 "cminusSintSem.y"
            { (yyval.node) = (yyvsp[0].node); }
#line 1713 "cminus.tab.c"
    break;

  case 18: /* param: type_specifier ID  */
#line 357 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[-1].tipo) == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: parametro '%s' nao pode ser void - LINHA: %d\n",
                    (yyvsp[0].id), yylineno);
            ctx->has_errors = 1;
        } else {
            insert_symbol_ctx(ctx, (yyvsp[0].id), (yyvsp[-1].tipo), KIND_VAR, yylineno);
            Simbolo *p = lookup_symbol_current_ctx(ctx, (yyvsp[0].id));
            if (p) p->is_param = 1;
        }
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[-1].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[-1].tipo);
        
        (yyval.node)->child[0] = newVarNode(IDK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[0].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_VAR;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = (yyvsp[-1].tipo);
        (yyval.node)->child[0]->lineno = yylineno;
        
        free((yyvsp[0].id));
    }
#line 1743 "cminus.tab.c"
    break;

  case 19: /* param: type_specifier ID LBRACK RBRACK  */
#line 383 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[-3].tipo) == TYPE_VOID) {
            fprintf(stderr, "ERRO SEMANTICO: parametro array '%s' nao pode ser void - LINHA: %d\n",
                    (yyvsp[-2].id), yylineno);
            ctx->has_errors = 1;
        } else {
            insert_symbol_ctx(ctx, (yyvsp[-2].id), TYPE_INT_ARRAY, KIND_ARRAY, yylineno);
            Simbolo *p = lookup_symbol_current_ctx(ctx, (yyvsp[-2].id));
            if (p) p->is_param = 1;
        }
        
        /* Construção da AST - padrão correto */
        (yyval.node) = newStmtNode((yyvsp[-3].tipo) == TYPE_INT ? INTEGERK : VOIDK);
        (yyval.node)->type = (yyvsp[-3].tipo);
        
        (yyval.node)->child[0] = newVarNode(VECTORK);
        (yyval.node)->child[0]->kind.var.attr.name = strdup((yyvsp[-2].id));
        (yyval.node)->child[0]->kind.var.varKind = KIND_ARRAY;
        (yyval.node)->child[0]->kind.var.acesso = DECLK;
        (yyval.node)->child[0]->type = TYPE_INT_ARRAY;
        (yyval.node)->child[0]->lineno = yylineno;
        
        free((yyvsp[-2].id));
    }
#line 1773 "cminus.tab.c"
    break;

  case 20: /* compound_stmt: LBRACE local_declarations statement_list RBRACE  */
#line 413 "cminusSintSem.y"
    {
        (yyval.node) = newStmtNode(COMPK);
        (yyval.node)->child[0] = (yyvsp[-2].node);  /* declarações locais */
        (yyval.node)->child[1] = (yyvsp[-1].node);  /* lista de statements */
        (yyval.node)->lineno = yylineno;
    }
#line 1784 "cminus.tab.c"
    break;

  case 21: /* local_declarations: local_declarations var_declaration  */
#line 423 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        } else {
            (yyval.node) = (yyvsp[0].node);
        }
    }
#line 1800 "cminus.tab.c"
    break;

  case 22: /* local_declarations: %empty  */
#line 434 "cminusSintSem.y"
                  { (yyval.node) = NULL; }
#line 1806 "cminus.tab.c"
    break;

  case 23: /* statement_list: statement_list statement  */
#line 440 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[-1].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-1].node);
        } else {
            (yyval.node) = (yyvsp[0].node);
        }
    }
#line 1822 "cminus.tab.c"
    break;

  case 24: /* statement_list: %empty  */
#line 451 "cminusSintSem.y"
                  { (yyval.node) = NULL; }
#line 1828 "cminus.tab.c"
    break;

  case 25: /* statement: expression_stmt  */
#line 456 "cminusSintSem.y"
                    { (yyval.node) = (yyvsp[0].node); }
#line 1834 "cminus.tab.c"
    break;

  case 26: /* statement: compound_stmt_with_scope  */
#line 457 "cminusSintSem.y"
                               { (yyval.node) = (yyvsp[0].node); }
#line 1840 "cminus.tab.c"
    break;

  case 27: /* statement: selection_stmt  */
#line 458 "cminusSintSem.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1846 "cminus.tab.c"
    break;

  case 28: /* statement: iteration_stmt  */
#line 459 "cminusSintSem.y"
                     { (yyval.node) = (yyvsp[0].node); }
#line 1852 "cminus.tab.c"
    break;

  case 29: /* statement: return_stmt  */
#line 460 "cminusSintSem.y"
                  { (yyval.node) = (yyvsp[0].node); }
#line 1858 "cminus.tab.c"
    break;

  case 30: /* $@2: %empty  */
#line 464 "cminusSintSem.y"
    { enter_scope_ctx(ctx); }
#line 1864 "cminus.tab.c"
    break;

  case 31: /* compound_stmt_with_scope: $@2 compound_stmt  */
#line 466 "cminusSintSem.y"
    { 
        leave_scope_ctx(ctx);
        (yyval.node) = (yyvsp[0].node);
    }
#line 1873 "cminus.tab.c"
    break;

  case 32: /* expression_stmt: expression SEMI  */
#line 474 "cminusSintSem.y"
                    { (yyval.node) = (yyvsp[-1].node); }
#line 1879 "cminus.tab.c"
    break;

  case 33: /* expression_stmt: SEMI  */
#line 475 "cminusSintSem.y"
           { (yyval.node) = NULL; }
#line 1885 "cminus.tab.c"
    break;

  case 34: /* selection_stmt: IF LPAREN expression RPAREN statement  */
#line 481 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[-2].node) && (yyvsp[-2].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do IF deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }

        /* Construção da AST */
        (yyval.node) = newStmtNode(IFK);
        (yyval.node)->child[0] = (yyvsp[-2].node);    /* condição */
        (yyval.node)->child[1] = (yyvsp[0].node);    /* then */
        (yyval.node)->child[2] = NULL;  /* else */
        (yyval.node)->lineno = yylineno;
    }
#line 1904 "cminus.tab.c"
    break;

  case 35: /* selection_stmt: IF LPAREN expression RPAREN statement ELSE statement  */
#line 496 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[-4].node) && (yyvsp[-4].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do IF deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }

        /* Construção da AST */
        (yyval.node) = newStmtNode(IFK);
        (yyval.node)->child[0] = (yyvsp[-4].node);    /* condição */
        (yyval.node)->child[1] = (yyvsp[-2].node);    /* then */
        (yyval.node)->child[2] = (yyvsp[0].node);    /* else */
        (yyval.node)->lineno = yylineno;
    }
#line 1923 "cminus.tab.c"
    break;

  case 36: /* iteration_stmt: WHILE LPAREN expression RPAREN statement  */
#line 515 "cminusSintSem.y"
    {
        /* Análise semântica */
        if ((yyvsp[-2].node) && (yyvsp[-2].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: condicao do WHILE deve ser inteira - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }
        
        /* Construção da AST */
        (yyval.node) = newStmtNode(WHILEK);
        (yyval.node)->child[0] = (yyvsp[-2].node);    /* condição */
        (yyval.node)->child[1] = (yyvsp[0].node);    /* corpo */
        (yyval.node)->lineno = yylineno;
    }
#line 1941 "cminus.tab.c"
    break;

  case 37: /* return_stmt: RETURN SEMI  */
#line 533 "cminusSintSem.y"
    {
        (yyval.node) = newStmtNode(RETURNK);
        (yyval.node)->child[0] = NULL;  /* sem expressão */
        (yyval.node)->lineno = yylineno;
    }
#line 1951 "cminus.tab.c"
    break;

  case 38: /* return_stmt: RETURN expression SEMI  */
#line 539 "cminusSintSem.y"
    {
        (yyval.node) = newStmtNode(RETURNK);
        (yyval.node)->child[0] = (yyvsp[-1].node);    /* expressão */
        (yyval.node)->lineno = yylineno;
    }
#line 1961 "cminus.tab.c"
    break;

  case 39: /* expression: var ASSIGN expression  */
#line 549 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar var_type = (yyvsp[-2].node) ? (yyvsp[-2].node)->type : TYPE_ERROR;
        TipoVar exp_type = (yyvsp[0].node) ? (yyvsp[0].node)->type : TYPE_ERROR;
        
        if ((yyvsp[-2].node) && (yyvsp[-2].node)->nodekind == VARK && (yyvsp[-2].node)->kind.var.varKind == KIND_ARRAY &&
            (yyvsp[-2].node)->child[0] == NULL) {
            fprintf(stderr, "ERRO SEMANTICO: nao e possivel atribuir a array completo - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        } else if (var_type != exp_type && exp_type != TYPE_ERROR && var_type != TYPE_ERROR) {
            fprintf(stderr, "ERRO SEMANTICO: tipos incompativeis na atribuicao - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        }
        
        /* Construção da AST */
        (yyval.node) = newExpNode(ASSIGNK);
        (yyval.node)->child[0] = (yyvsp[-2].node);
        (yyval.node)->child[1] = (yyvsp[0].node);
        (yyval.node)->type = var_type;
        (yyval.node)->lineno = yylineno;
    }
#line 1987 "cminus.tab.c"
    break;

  case 40: /* expression: simple_expression  */
#line 570 "cminusSintSem.y"
                        { (yyval.node) = (yyvsp[0].node); }
#line 1993 "cminus.tab.c"
    break;

  case 41: /* var: ID  */
#line 576 "cminusSintSem.y"
    {
        Simbolo *s = lookup_symbol_ctx(ctx, (yyvsp[0].id));
        TipoVar tipo = TYPE_ERROR;
        TipoSimbolo kind = KIND_VAR;
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada - LINHA: %d\n", (yyvsp[0].id), yylineno);
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
        (yyval.node)->kind.var.attr.name = strdup((yyvsp[0].id));
        (yyval.node)->kind.var.varKind = kind;
        (yyval.node)->kind.var.acesso = ACCESSK;
        (yyval.node)->lineno = yylineno;
        free((yyvsp[0].id));
    }
#line 2025 "cminus.tab.c"
    break;

  case 42: /* var: ID LBRACK expression RBRACK  */
#line 604 "cminusSintSem.y"
    {
        Simbolo *s = lookup_symbol_ctx(ctx, (yyvsp[-3].id));
        TipoVar tipo = TYPE_ERROR;
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: variavel '%s' nao declarada - LINHA: %d\n", (yyvsp[-3].id), yylineno);
            ctx->has_errors = 1;
        } else if (s->kind != KIND_ARRAY) {
            fprintf(stderr, "ERRO SEMANTICO: '%s' nao e um array - LINHA: %d\n", (yyvsp[-3].id), yylineno);
            ctx->has_errors = 1;
        } else if ((yyvsp[-1].node) && (yyvsp[-1].node)->type != TYPE_INT) {
            fprintf(stderr, "ERRO SEMANTICO: indice de array deve ser inteiro - LINHA: %d\n", yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = TYPE_INT;
        }
        
        /* Cria nó de acesso a array */
        (yyval.node) = newVarNode(VECTORK);
        (yyval.node)->kind.var.attr.name = strdup((yyvsp[-3].id));
        (yyval.node)->kind.var.varKind = KIND_ARRAY;
        (yyval.node)->kind.var.acesso = ACCESSK;
        (yyval.node)->child[0] = (yyvsp[-1].node);  /* índice */
        (yyval.node)->type = tipo;
        (yyval.node)->lineno = yylineno;
        free((yyvsp[-3].id));
    }
#line 2057 "cminus.tab.c"
    break;

  case 43: /* simple_expression: additive_expression relop additive_expression  */
#line 636 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar t1 = (yyvsp[-2].node) ? (yyvsp[-2].node)->type : TYPE_ERROR;
        TipoVar t2 = (yyvsp[0].node) ? (yyvsp[0].node)->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "relacional", t1, t2, yylineno);
        
        /* Construção da AST */
        (yyval.node) = newExpNode(OPK);
        (yyval.node)->child[0] = (yyvsp[-2].node);
        (yyval.node)->child[1] = (yyvsp[0].node);
        (yyval.node)->op = (yyvsp[-1].ival);  /* operador relacional */
        (yyval.node)->type = result;
        (yyval.node)->lineno = yylineno;
    }
#line 2076 "cminus.tab.c"
    break;

  case 44: /* simple_expression: additive_expression  */
#line 650 "cminusSintSem.y"
                          { (yyval.node) = (yyvsp[0].node); }
#line 2082 "cminus.tab.c"
    break;

  case 45: /* relop: LE  */
#line 654 "cminusSintSem.y"
           { (yyval.ival) = LE; }
#line 2088 "cminus.tab.c"
    break;

  case 46: /* relop: LT  */
#line 655 "cminusSintSem.y"
           { (yyval.ival) = LT; }
#line 2094 "cminus.tab.c"
    break;

  case 47: /* relop: GT  */
#line 656 "cminusSintSem.y"
           { (yyval.ival) = GT; }
#line 2100 "cminus.tab.c"
    break;

  case 48: /* relop: GE  */
#line 657 "cminusSintSem.y"
           { (yyval.ival) = GE; }
#line 2106 "cminus.tab.c"
    break;

  case 49: /* relop: EQ  */
#line 658 "cminusSintSem.y"
           { (yyval.ival) = EQ; }
#line 2112 "cminus.tab.c"
    break;

  case 50: /* relop: NE  */
#line 659 "cminusSintSem.y"
           { (yyval.ival) = NE; }
#line 2118 "cminus.tab.c"
    break;

  case 51: /* additive_expression: additive_expression addop term  */
#line 665 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar t1 = (yyvsp[-2].node) ? (yyvsp[-2].node)->type : TYPE_ERROR;
        TipoVar t2 = (yyvsp[0].node) ? (yyvsp[0].node)->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "aditivo", t1, t2, yylineno);
        
        /* Construção da AST */
        (yyval.node) = newExpNode(OPK);
        (yyval.node)->child[0] = (yyvsp[-2].node);
        (yyval.node)->child[1] = (yyvsp[0].node);
        (yyval.node)->op = (yyvsp[-1].ival);  /* operador aditivo */
        (yyval.node)->type = result;
        (yyval.node)->lineno = yylineno;
    }
#line 2137 "cminus.tab.c"
    break;

  case 52: /* additive_expression: term  */
#line 679 "cminusSintSem.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2143 "cminus.tab.c"
    break;

  case 53: /* addop: PLUS  */
#line 683 "cminusSintSem.y"
             { (yyval.ival) = PLUS; }
#line 2149 "cminus.tab.c"
    break;

  case 54: /* addop: MINUS  */
#line 684 "cminusSintSem.y"
              { (yyval.ival) = MINUS; }
#line 2155 "cminus.tab.c"
    break;

  case 55: /* term: term mulop factor  */
#line 690 "cminusSintSem.y"
    {
        /* Análise semântica */
        TipoVar t1 = (yyvsp[-2].node) ? (yyvsp[-2].node)->type : TYPE_ERROR;
        TipoVar t2 = (yyvsp[0].node) ? (yyvsp[0].node)->type : TYPE_ERROR;
        TipoVar result = check_expression_type_ctx(ctx, "multiplicativo", t1, t2, yylineno);
        
        /* Construção da AST */
        (yyval.node) = newExpNode(OPK);
        (yyval.node)->child[0] = (yyvsp[-2].node);
        (yyval.node)->child[1] = (yyvsp[0].node);
        (yyval.node)->op = (yyvsp[-1].ival);  /* operador multiplicativo */
        (yyval.node)->type = result;
        (yyval.node)->lineno = yylineno;
    }
#line 2174 "cminus.tab.c"
    break;

  case 56: /* term: factor  */
#line 704 "cminusSintSem.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2180 "cminus.tab.c"
    break;

  case 57: /* mulop: TIMES  */
#line 708 "cminusSintSem.y"
              { (yyval.ival) = TIMES; }
#line 2186 "cminus.tab.c"
    break;

  case 58: /* mulop: DIVIDE  */
#line 709 "cminusSintSem.y"
               { (yyval.ival) = DIVIDE; }
#line 2192 "cminus.tab.c"
    break;

  case 59: /* factor: LPAREN expression RPAREN  */
#line 714 "cminusSintSem.y"
                             { (yyval.node) = (yyvsp[-1].node); }
#line 2198 "cminus.tab.c"
    break;

  case 60: /* factor: var  */
#line 715 "cminusSintSem.y"
          { (yyval.node) = (yyvsp[0].node); }
#line 2204 "cminus.tab.c"
    break;

  case 61: /* factor: call  */
#line 716 "cminusSintSem.y"
           { (yyval.node) = (yyvsp[0].node); }
#line 2210 "cminus.tab.c"
    break;

  case 62: /* factor: NUM  */
#line 718 "cminusSintSem.y"
    { 
        (yyval.node) = newExpNode(CONSTK);
        (yyval.node)->kind.var.attr.val = (yyvsp[0].ival);
        (yyval.node)->type = TYPE_INT;
        (yyval.node)->lineno = yylineno;
    }
#line 2221 "cminus.tab.c"
    break;

  case 63: /* call: ID LPAREN args RPAREN  */
#line 729 "cminusSintSem.y"
    {
        Simbolo *s = lookup_symbol_ctx(ctx, (yyvsp[-3].id));
        TipoVar tipo = TYPE_INT;  /* padrão */
        
        if (!s) {
            fprintf(stderr, "ERRO SEMANTICO: funcao '%s' nao declarada - LINHA: %d\n", (yyvsp[-3].id), yylineno);
            ctx->has_errors = 1;
        } else if (s->kind != KIND_FUNC) {
            fprintf(stderr, "ERRO SEMANTICO: '%s' nao e uma funcao - LINHA: %d\n", (yyvsp[-3].id), yylineno);
            ctx->has_errors = 1;
        } else {
            tipo = s->tipo;  /* tipo de retorno da função */
        }
        
        /* Cria nó de chamada de função */
        (yyval.node) = newExpNode(CALLK);
        (yyval.node)->kind.var.attr.name = strdup((yyvsp[-3].id));
        (yyval.node)->child[0] = (yyvsp[-1].node);  /* argumentos */
        (yyval.node)->type = tipo;
        (yyval.node)->lineno = yylineno;
        free((yyvsp[-3].id));
    }
#line 2248 "cminus.tab.c"
    break;

  case 64: /* args: arg_list  */
#line 755 "cminusSintSem.y"
             { (yyval.node) = (yyvsp[0].node); }
#line 2254 "cminus.tab.c"
    break;

  case 65: /* args: %empty  */
#line 756 "cminusSintSem.y"
                  { (yyval.node) = NULL; }
#line 2260 "cminus.tab.c"
    break;

  case 66: /* arg_list: arg_list COMMA expression  */
#line 762 "cminusSintSem.y"
    {
        TreeNode *t = (yyvsp[-2].node);
        if (t != NULL) {
            while (t->sibling != NULL)
                t = t->sibling;
            t->sibling = (yyvsp[0].node);
            (yyval.node) = (yyvsp[-2].node);
        } else {
            (yyval.node) = (yyvsp[0].node);
        }
    }
#line 2276 "cminus.tab.c"
    break;

  case 67: /* arg_list: expression  */
#line 773 "cminusSintSem.y"
                 { (yyval.node) = (yyvsp[0].node); }
#line 2282 "cminus.tab.c"
    break;


#line 2286 "cminus.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (ctx, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, ctx);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (ctx, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, ctx);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, ctx);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

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
