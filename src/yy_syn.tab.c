/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* "%code top" blocks.  */
#line 87 "./src/yy_syn.y" /* yacc.c:316  */

    #include <stdio.h>
    #include "type.h"
    #include "symbolTbl.h"
    #include "string.h"
    #include "yy_syn.def.h"
    #include "typeCheck.h"
    #include "dispatcher.h"
    #include "mibTreeGen.h"
    #include "pair.h"
    #include "util.h"

    #include "moduleAlias.h"

    #define YYDEBUG 1

    extern typeTable mibTypeTbl;
    extern symbolTable symTable;
    dispatchParam importParam;
    genericStack importInfoStack;

    // Use to keep track of OID_CHAIN processing position
    static char *lastProcessed = NULL;

    int syntaxParserInit(void) {
        memset(&importParam, 0, sizeof(dispatchParam));
        genericStackConstruct(&importInfoStack, 20, sizeof(collectInfo *));
    }

    // TYPE Block type
    typedef enum {
        BLOCK_TYPE_BIT_NAME,
        BLOCK_TYPE_LITERAL
    };

#line 99 "./src/yy_syn.tab.c" /* yacc.c:316  */



/* Copy the first part of user declarations.  */

#line 105 "./src/yy_syn.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "yy_syn.tab.h".  */
#ifndef YY_YY_SRC_YY_SYN_TAB_H_INCLUDED
# define YY_YY_SRC_YY_SYN_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    OBJ_SPECIFIER = 258,
    SYNTAX_SPECIFIER = 259,
    ACCESS_SPECIFIER = 260,
    ACCESS_SPECIFIER_SHORT = 261,
    STATUS_SPECIFIER = 262,
    STATUS_VALUE = 263,
    DESC_SPECIFIER = 264,
    MOUNT_POINT = 265,
    ASSIGNED = 266,
    BEGIN_ = 267,
    END_ = 268,
    DEF = 269,
    SEQ = 270,
    COMMA = 271,
    SEMICOLON = 272,
    INDEX_ = 273,
    AUGMENTS_ = 274,
    TRAP_SPECIFIER = 275,
    OBJ_IDEN_ = 276,
    L_BRACE = 277,
    R_BRACE = 278,
    OBJECTS_ = 279,
    FROM_ = 280,
    IMPORTS_ = 281,
    ENTERPRISE_SPECIFIER = 282,
    TRAP_TYPE_SPECIFIER = 283,
    L_PAREN = 284,
    R_PAREN = 285,
    HEX_STRING = 286,
    UNITS_SPECIFIER = 287,
    IMPLED = 288,
    DEFVAL_SPECIFIER = 289,
    DISPLAY_HINT = 290,
    REFERENCE = 291,
    TC_SPECIFIER = 292,
    COMPLIANCE_SPECIFIER = 293,
    MIN_ACCESS_SPECIFIER = 294,
    OBJECT_ = 295,
    NOTIFY_SPECIFIER = 296,
    NOTIFY_GRP_SPECIFIER = 297,
    MODULE_SPECIFIER = 298,
    MANDATORY_SPECIFIER = 299,
    GROUP_SPECIFIER = 300,
    OBJECT_SPECIFIER = 301,
    WRITE_SPECIFIER = 302,
    OBJ_GRP_SPECIFIER = 303,
    TO = 304,
    OR = 305,
    SIZE = 306,
    OBJ_IDENTITY_SPECIFIER = 307,
    NOTIFY_TYPE_SPECIFIER = 308,
    SMI_SPECIFIER = 309,
    SMI_VAL = 310,
    MOD_SPECIFIER = 311,
    LAST_UPDATED = 312,
    ORGANIZATION = 313,
    REVISION = 314,
    CONTACT_INFO = 315,
    REVISION_DATE = 316,
    IDENTIFIER = 317,
    NUM = 318,
    TYPE_BUILT_IN = 319,
    ACCESS_VALUE = 320,
    STRING = 321
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 49 "./src/yy_syn.y" /* yacc.c:355  */

    char *str;
#line 67 "./src/yy_syn.y" /* yacc.c:355  */

    // SEQUENCE
    struct sequence_item si;
    struct sequence sq;

#line 221 "./src/yy_syn.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_YY_SYN_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 238 "./src/yy_syn.tab.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

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

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
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

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  40
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   247

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  67
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  85
/* YYNRULES -- Number of rules.  */
#define YYNRULES  149
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  319

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   321

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   127,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   146,
     149,   155,   156,   159,   182,   188,   194,   199,   199,   203,
     205,   209,   213,   215,   217,   218,   222,   226,   228,   230,
     231,   235,   239,   240,   242,   243,   245,   247,   248,   250,
     251,   253,   254,   255,   257,   258,   260,   261,   263,   264,
     266,   269,   271,   272,   274,   275,   277,   278,   283,   287,
     289,   290,   293,   297,   309,   317,   321,   325,   327,   328,
     332,   340,   358,   369,   375,   383,   383,   386,   389,   399,
     407,   408,   411,   417,   433,   436,   443,   446,   456,   459,
     459,   462,   463,   466,   469,   476,   479,   482,   484,   487,
     488,   491,   494,   498,   503,   504,   506,   507,   510,   513,
     513,   516,   523,   523,   526,   529,   536,   537,   538,   541,
     542,   544,   545,   547,   548,   549,   550,   551,   553,   554,
     556,   557,   559,   560,   563,   572,   585,   591,   604,   607
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "OBJ_SPECIFIER", "SYNTAX_SPECIFIER",
  "ACCESS_SPECIFIER", "ACCESS_SPECIFIER_SHORT", "STATUS_SPECIFIER",
  "STATUS_VALUE", "DESC_SPECIFIER", "MOUNT_POINT", "ASSIGNED", "BEGIN_",
  "END_", "DEF", "SEQ", "COMMA", "SEMICOLON", "INDEX_", "AUGMENTS_",
  "TRAP_SPECIFIER", "OBJ_IDEN_", "L_BRACE", "R_BRACE", "OBJECTS_", "FROM_",
  "IMPORTS_", "ENTERPRISE_SPECIFIER", "TRAP_TYPE_SPECIFIER", "L_PAREN",
  "R_PAREN", "HEX_STRING", "UNITS_SPECIFIER", "IMPLED", "DEFVAL_SPECIFIER",
  "DISPLAY_HINT", "REFERENCE", "TC_SPECIFIER", "COMPLIANCE_SPECIFIER",
  "MIN_ACCESS_SPECIFIER", "OBJECT_", "NOTIFY_SPECIFIER",
  "NOTIFY_GRP_SPECIFIER", "MODULE_SPECIFIER", "MANDATORY_SPECIFIER",
  "GROUP_SPECIFIER", "OBJECT_SPECIFIER", "WRITE_SPECIFIER",
  "OBJ_GRP_SPECIFIER", "TO", "OR", "SIZE", "OBJ_IDENTITY_SPECIFIER",
  "NOTIFY_TYPE_SPECIFIER", "SMI_SPECIFIER", "SMI_VAL", "MOD_SPECIFIER",
  "LAST_UPDATED", "ORGANIZATION", "REVISION", "CONTACT_INFO",
  "REVISION_DATE", "IDENTIFIER", "NUM", "TYPE_BUILT_IN", "ACCESS_VALUE",
  "STRING", "$accept", "MIB", "DEFINITION", "IMPORT", "MODULES",
  "MODULES_CONTENT", "ITEMS", "REF_PART", "TRAP_TYPE", "ENTERPRISE_PART",
  "OBJ_GRP_DEFINED", "OBJ_GRP", "OBJ_PART", "OBJS", "NOTIFY_GRP_DEFINED",
  "NOTIFY_GRP", "NOTIFY_PART", "NOTIFICATIONS", "COMPLIANCE_DEFINED",
  "COMPLIANCE_BODY", "MODULE_PART", "MODULES_", "MODULE_", "MODULE_NAME",
  "MANDATORY_PART", "GROUPS", "Group", "COMPLIANCE_PART", "COMPLIANCES",
  "COMPLIANCE", "COMPLIANCE_GRP", "COMPLIANCE_OBJ", "COMPLIANCE_SYNTAX",
  "COMPLIANCE_WRITE", "COMPLIANCE_ACCESS", "TC_DEFINED", "TC",
  "DisplayPart", "TYPE", "OBJ_IDENTITY_DEFINED", "OBJ_IDENTITY",
  "NOTIFY_TYPE_DEFINED", "NOTIFY_TYPE", "NOTIFY_TYPE_OBJ_PART",
  "TYPE_DEFINED", "END", "SEQUENCE", "SEQ_ITEM", "MAYBE_COMMA", "SMI",
  "MODULE_DEF", "MODULE_BODY", "REVISIONS", "OBJ_DEAL", "OBJ_IDENTIFIER",
  "OBJ_TYPE", "HEAD", "BODY", "PROPERTY", "OBJECT", "OBJECT_ITEM",
  "SYNTAX", "SYNTAX_VALUE", "TYPE_SPECIFIER", "TYPE_SPECIFIER_",
  "ENUMERATE_MEMBERS", "ENUMERATE_MEMBER", "ONE_OR_MORE_VAL", "VAL",
  "RANGE", "NUM_", "ACCESS", "ACCESS_FIELD", "STATUS", "DESCRIPTION",
  "INDEX", "INDEX_ITEM", "DEFVAL", "DEFVAL_VAL", "BITS_VALUE", "BIT_NAMES",
  "UNITS", "MOUNT", "OID_CHAIN", "OID_ATTACH", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321
};
# endif

#define YYPACT_NINF -245

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-245)))

#define YYTABLE_NINF -120

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      -1,  -245,   -15,   -34,     6,    29,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,  -245,    -1,    33,    17,    22,   -15,    18,  -245,  -245,
       1,    41,    46,    49,    71,    39,    57,    75,    57,    27,
    -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,
    -245,  -245,  -245,  -245,  -245,  -245,  -245,     2,  -245,  -245,
      51,    24,  -245,  -245,    25,    66,    54,  -245,  -245,    -7,
      78,    69,    30,    84,    86,    85,    73,    85,    90,    76,
      85,    92,   108,    85,  -245,    85,   111,    53,    85,  -245,
      -7,  -245,    56,    26,  -245,  -245,    58,    59,  -245,   114,
      61,     4,  -245,  -245,  -245,    62,  -245,    60,   118,   106,
    -245,    67,  -245,   122,    70,  -245,   123,   124,  -245,  -245,
     126,    77,  -245,  -245,  -245,  -245,  -245,   129,    72,   -38,
     115,  -245,   131,   112,   117,   127,  -245,   113,    96,   116,
      94,  -245,    98,    87,   137,    83,    89,   136,   130,   145,
     139,   133,   148,    93,   149,    97,   152,   153,  -245,    -7,
    -245,   155,   102,  -245,    61,  -245,     5,  -245,     5,     9,
     138,   103,   132,   -22,     7,    67,  -245,   101,    70,  -245,
     105,   132,   107,   109,  -245,   110,   132,   156,   119,   144,
    -245,   147,  -245,  -245,  -245,  -245,  -245,   120,   135,   121,
     157,   125,   146,   158,  -245,   132,  -245,   132,  -245,   132,
     128,  -245,    55,    58,  -245,   132,  -245,   159,  -245,   134,
    -245,  -245,   135,   160,  -245,   146,  -245,  -245,  -245,  -245,
    -245,   170,   161,   166,   163,  -245,   178,  -245,  -245,   151,
    -245,  -245,  -245,   140,   141,   142,   169,    85,     2,   171,
     -27,   143,   176,   175,   177,   -17,   181,  -245,   150,   154,
     162,  -245,  -245,   -27,  -245,  -245,   164,  -245,   141,  -245,
    -245,   165,  -245,  -245,  -245,  -245,   184,   179,  -245,  -245,
     185,   183,   205,   201,  -245,   202,  -245,   197,   191,  -245,
    -245,   167,  -245,   150,     2,   168,   172,   173,   165,  -245,
     203,   194,  -245,  -245,     2,   182,  -245,   143,  -245,   167,
    -245,  -245,   180,   209,  -245,  -245,  -245,   174,  -245
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      18,    81,    22,     0,     0,     0,    18,    18,    18,    18,
      18,    18,    18,    18,    18,    18,    18,    18,    18,    18,
      18,    92,    18,    98,    24,     0,    22,     0,    87,    95,
       0,     0,     0,     0,     0,     0,     0,     0,    79,     0,
       1,     7,     8,    17,    12,    13,    14,    11,    15,    16,
      10,     6,     4,     5,     9,     2,     3,     0,    94,    96,
     143,    26,    20,    21,     0,     0,    71,    73,    72,   107,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    78,     0,     0,     0,     0,   105,
     107,   103,     0,     0,    25,    23,     0,     0,    68,     0,
       0,     0,    80,   106,    19,     0,    30,     0,     0,     0,
      41,     0,    36,     0,     0,    31,     0,     0,    74,    76,
       0,     0,    88,   104,   142,   122,   123,     0,     0,     0,
       0,    70,     0,     0,     0,    86,   120,     0,   116,     0,
     114,   117,     0,     0,     0,     0,     0,    39,     0,     0,
      34,     0,     0,     0,     0,     0,     0,     0,   121,   107,
      82,     0,     0,   108,    85,   111,     0,   109,     0,     0,
       0,     0,    28,   149,     0,     0,    38,     0,     0,    33,
       0,    28,     0,     0,   124,     0,    28,    86,     0,     0,
     112,     0,   115,   119,   118,    93,    29,     0,     0,     0,
       0,     0,   149,     0,    40,    28,    35,    28,    75,    28,
       0,   125,   128,    85,    84,    28,   113,     0,    27,    48,
      42,    43,    44,     0,   144,   149,   146,   145,    37,    32,
      77,     0,     0,     0,   132,    83,     0,   110,    47,    50,
      45,   148,   147,     0,     0,     0,     0,     0,     0,     0,
      55,    91,   130,     0,     0,     0,   100,    69,     0,     0,
       0,    46,    54,    56,    58,    59,     0,    89,     0,   126,
     127,   139,   135,   133,   134,   136,     0,     0,    97,    53,
       0,    51,    63,     0,    57,     0,   129,   140,     0,   138,
     131,     0,    49,     0,     0,    65,     0,     0,     0,   137,
     102,     0,    52,    62,     0,    67,    60,    91,   141,     0,
      99,    64,     0,     0,    90,   101,    66,     0,    61
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -245,    95,  -245,  -245,   196,  -245,   186,  -130,  -245,  -245,
    -245,  -245,   187,    42,  -245,  -245,  -245,    48,  -245,  -245,
    -245,    10,  -245,  -245,  -245,   -67,  -245,  -245,   -35,  -245,
    -245,  -245,  -245,  -245,  -245,  -245,  -245,  -245,   -29,  -245,
    -245,  -245,  -245,  -245,  -245,  -245,  -245,    20,    44,  -245,
    -245,  -245,   -73,  -245,  -245,  -245,  -245,  -245,  -245,  -245,
     -74,  -245,  -244,   -88,  -245,    79,  -245,  -138,  -245,  -245,
      68,  -245,  -245,  -245,  -245,  -245,   -32,  -245,  -245,  -245,
     -57,  -245,   -77,  -245,  -183
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     5,     6,     7,    25,    26,    27,   198,     8,    73,
       9,    80,    81,   151,    10,    77,    78,   148,    11,    75,
     220,   221,   222,   239,   250,   280,   281,   261,   262,   263,
     264,   265,   295,   305,   313,    12,    98,    99,    90,    13,
      83,    14,    85,    86,    15,    16,    17,   130,   165,    18,
      19,    88,   267,    20,    21,    22,    23,    58,    59,   278,
     301,    60,    91,   102,   103,   134,   135,   139,   140,   141,
     142,   127,   128,   157,   186,   234,   253,   247,   276,   288,
     289,    93,   110,   174,   201
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     112,    69,   123,   115,   257,   271,   118,   199,   119,    29,
     -26,   122,     1,   259,   272,   100,    65,    30,   260,   226,
      31,    28,   101,    89,    67,     2,    68,    32,   191,    40,
     192,   125,   126,    61,    33,   136,   136,    57,    66,    62,
     136,   200,   242,    64,    34,   273,   274,    24,    35,   275,
     303,   208,    70,     3,    36,   137,   212,    71,    37,    38,
     311,     4,    39,    67,    67,    68,    68,   138,   138,   202,
     203,   187,   193,   232,   233,   228,    72,   229,    74,   230,
      76,    79,    82,    92,    87,   236,    24,    95,    96,    97,
     104,   105,   106,   107,   108,   111,   109,   113,   114,   116,
     159,    41,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,   117,    56,   120,   121,
     129,   132,   124,   133,   143,   131,   144,   145,   146,   147,
     149,   152,   150,   153,   154,   155,   156,   158,   160,   161,
     163,   162,   166,   164,   168,  -119,   167,   169,   171,   172,
     170,   173,   175,   176,   177,   178,   179,   180,   182,   181,
     184,   195,   185,   183,   188,   189,   196,   205,   197,   210,
     256,   207,   213,   209,   216,   199,   211,   217,   219,   243,
     224,   227,   248,   244,   223,   215,   218,   225,   245,   237,
     241,   255,   268,   258,   231,   249,   238,   246,   269,   293,
     270,   291,   266,   252,   254,   277,   251,   290,   292,   294,
     296,   297,   279,   298,   299,   304,   282,   310,   317,   309,
     206,   312,    63,   204,   283,    84,   302,   287,   284,   300,
     285,   214,   240,   235,   314,   315,   286,   194,   306,   307,
     318,   308,     0,   190,     0,   316,     0,    94
};

static const yytype_int16 yycheck[] =
{
      77,    30,    90,    80,   248,    22,    83,    29,    85,     3,
      25,    88,    13,    40,    31,    22,    15,    11,    45,   202,
      14,    55,    29,    21,    62,    26,    64,    21,   166,     0,
     168,     5,     6,    16,    28,    31,    31,     4,    37,    17,
      31,    63,   225,    25,    38,    62,    63,    62,    42,    66,
     294,   181,    11,    54,    48,    51,   186,    11,    52,    53,
     304,    62,    56,    62,    62,    64,    64,    63,    63,    62,
      63,   159,    63,    18,    19,   205,    27,   207,     7,   209,
      41,    24,     7,    32,    57,   215,    62,    62,    22,    35,
      12,    22,    62,     9,     8,    22,    11,     7,    22,     7,
     129,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,     8,    22,     7,    66,
      62,     7,    66,    62,    62,    66,    66,     9,    22,    62,
       8,     8,    62,     9,     8,    58,     7,    65,    23,     8,
      23,    29,    29,    16,    50,    49,    30,    49,    11,    66,
      63,    62,    16,    23,     9,    16,    23,     9,     9,    66,
       8,    23,     9,    66,     9,    63,    63,    66,    36,    60,
     247,    66,    16,    66,    30,    29,    66,    30,    43,     9,
      23,    23,     4,    22,    63,    66,    66,    62,    22,    30,
      30,    22,    16,    22,    66,    44,    62,    34,    23,    16,
      23,    22,    59,    62,    62,    24,    66,    23,    23,     4,
       9,     9,    62,    16,    23,    47,    62,    23,     9,    16,
     178,    39,    26,   175,    62,    38,   293,    62,   263,    62,
      66,   187,   222,   213,   307,   309,   268,   169,    66,    66,
      66,   298,    -1,   164,    -1,    65,    -1,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    13,    26,    54,    62,    68,    69,    70,    75,    77,
      81,    85,   102,   106,   108,   111,   112,   113,   116,   117,
     120,   121,   122,   123,    62,    71,    72,    73,    55,     3,
      11,    14,    21,    28,    38,    42,    48,    52,    53,    56,
       0,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,     4,   124,   125,
     128,    16,    17,    71,    25,    15,    37,    62,    64,   105,
      11,    11,    27,    76,     7,    86,    41,    82,    83,    24,
      78,    79,     7,   107,    79,   109,   110,    57,   118,    21,
     105,   129,    32,   148,    73,    62,    22,    35,   103,   104,
      22,    29,   130,   131,    12,    22,    62,     9,     8,    11,
     149,    22,   149,     7,    22,   149,     7,     8,   149,   149,
       7,    66,   149,   130,    66,     5,     6,   138,   139,    62,
     114,    66,     7,    62,   132,   133,    31,    51,    63,   134,
     135,   136,   137,    62,    66,     9,    22,    62,    84,     8,
      62,    80,     8,     9,     8,    58,     7,   140,    65,   105,
      23,     8,    29,    23,    16,   115,    29,    30,    50,    49,
      63,    11,    66,    62,   150,    16,    23,     9,    16,    23,
       9,    66,     9,    66,     8,     9,   141,   130,     9,    63,
     132,   134,   134,    63,   137,    23,    63,    36,    74,    29,
      63,   151,    62,    63,    84,    66,    80,    66,    74,    66,
      60,    66,    74,    16,   115,    66,    30,    30,    66,    43,
      87,    88,    89,    63,    23,    62,   151,    23,    74,    74,
      74,    66,    18,    19,   142,   114,    74,    30,    62,    90,
      88,    30,   151,     9,    22,    22,    34,   144,     4,    44,
      91,    66,    62,   143,    62,    22,   149,   129,    22,    40,
      45,    94,    95,    96,    97,    98,    59,   119,    16,    23,
      23,    22,    31,    62,    63,    66,   145,    24,   126,    62,
      92,    93,    62,    62,    95,    66,   143,    62,   146,   147,
      23,    22,    23,    16,     4,    99,     9,     9,    16,    23,
      62,   127,    92,   129,    47,   100,    66,    66,   147,    16,
      23,   129,    39,   101,   119,   127,    65,     9,    66
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    67,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    69,
      70,    71,    71,    72,    73,    73,    73,    74,    74,    75,
      76,    77,    78,    79,    80,    80,    81,    82,    83,    84,
      84,    85,    86,    87,    88,    88,    89,    90,    90,    91,
      91,    92,    92,    93,    94,    94,    95,    95,    96,    96,
      97,    98,    99,    99,   100,   100,   101,   101,   102,   103,
     104,   104,   105,   105,   106,   107,   108,   109,   110,   110,
     111,   112,   113,   114,   114,   115,   115,   116,   117,   118,
     119,   119,   120,   121,   122,   123,   124,   125,   125,   126,
     126,   127,   127,   128,   129,   129,   130,   130,   131,   131,
     131,   132,   132,   133,   134,   134,   135,   135,   136,   137,
     137,   138,   139,   139,   140,   141,   142,   142,   142,   143,
     143,   144,   144,   145,   145,   145,   145,   145,   146,   146,
     147,   147,   148,   148,   149,   149,   150,   150,   151,   151
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     0,     4,
       3,     2,     0,     3,     1,     3,     0,     2,     0,     7,
       2,     4,     6,     4,     1,     3,     4,     6,     4,     1,
       3,     4,     6,     1,     1,     2,     4,     1,     0,     4,
       0,     1,     3,     1,     1,     0,     1,     2,     1,     1,
       4,     7,     2,     0,     2,     0,     2,     0,     4,     8,
       2,     0,     1,     1,     4,     5,     4,     6,     1,     0,
       4,     1,     6,     5,     4,     1,     0,     2,     4,     9,
       5,     0,     1,     7,     2,     2,     1,    10,     0,     4,
       0,     3,     1,     2,     2,     1,     1,     0,     3,     3,
       6,     2,     3,     4,     1,     3,     1,     1,     3,     1,
       1,     2,     1,     1,     2,     2,     4,     4,     0,     3,
       1,     4,     0,     1,     1,     1,     1,     3,     1,     0,
       1,     3,     2,     0,     5,     5,     3,     4,     3,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
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
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



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

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
          case 62: /* IDENTIFIER  */
#line 84 "./src/yy_syn.y" /* yacc.c:684  */
      { fprintf(yyoutput, "%s", ((*yyvaluep).str)); }
#line 958 "./src/yy_syn.tab.c" /* yacc.c:684  */
        break;

    case 63: /* NUM  */
#line 83 "./src/yy_syn.y" /* yacc.c:684  */
      { fprintf(yyoutput, "%s", ((*yyvaluep).str)); }
#line 964 "./src/yy_syn.tab.c" /* yacc.c:684  */
        break;

    case 133: /* ENUMERATE_MEMBER  */
#line 82 "./src/yy_syn.y" /* yacc.c:684  */
      { fprintf(yyoutput, "%s", ((*yyvaluep).str)); }
#line 970 "./src/yy_syn.tab.c" /* yacc.c:684  */
        break;


      default:
        break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
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
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
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
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
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
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
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
  int yytoken = 0;
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

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
        case 19:
#line 146 "./src/yy_syn.y" /* yacc.c:1646  */
    { }
#line 1569 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 149 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        // Begining to import symbol from another mib files.
        importWorks(&importInfoStack);
    }
#line 1578 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 159 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        dispatchParam *current;
        current = &importParam;

        char *moduleName = moduleAliasTrans((yyvsp[0].str));

        int ret = TRUE;
        collectInfo *importInfo = collectInfoConst(moduleName);

        // Store symbols that should be included.
        while (current = dispatchParamNext(current)) {
            ret = collectInfo_add(importInfo, current->param);
            if (ret == FALSE) {
                errorMsg("IMPORT: Symbol conflict.\n");
                exit(1);
            }
        }
        disParamRelease_Static(&importParam, NULL);

        push(&importInfoStack, &importInfo);
    }
#line 1604 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 182 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        dispatchParam *symbol = disParamConstruct((yyvsp[0].str));
        if (disParamStore(&importParam, symbol) == NULL) {
            exit(1);
        }
    }
#line 1615 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 188 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        dispatchParam *symbol = disParamConstruct((yyvsp[-2].str));
        if (disParamStore(&importParam, symbol) == NULL) {
            exit(1);
        }
    }
#line 1626 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 209 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        PARAM_FLUSH();
    }
#line 1634 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 222 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        PARAM_FLUSH();
    }
#line 1642 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 235 "./src/yy_syn.y" /* yacc.c:1646  */
    {
       PARAM_FLUSH();
    }
#line 1650 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 283 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        typeTableAdd(MIB_TYPE_TBL_R, strdup((yyvsp[-3].str)), CATE_CUSTOM, NULL);
    }
#line 1658 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 293 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        // Correctness of BUILT_IN type was checked by lexer
        (yyval.str) = (yyvsp[0].str);
    }
#line 1667 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 297 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        //  Customed type
        (yyval.str) = (yyvsp[0].str);
        if (typeCheck_isValid(MIB_TYPE_TBL_R, (yyval.str)) == FALSE) {
            /* Go into second pass because the checking may fail if it's
               defined after use */
            disParamStore(pendingTypes, disParamConstruct((yyvsp[0].str)));
        }
    }
#line 1681 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 309 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
        disParamStore(param, disParamConstruct((yyvsp[-3].str)));
        dispatch(DISPATCH_PARAM_STAGE, param);

        dispatch(MIBTREE_GENERATION, disParamConstruct(OBJECT));
    }
#line 1693 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 321 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        PARAM_FLUSH();
    }
#line 1701 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 332 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        _Bool isExists = typeTableIsTypeExists(MIB_TYPE_TBL_R, (yyvsp[-3].str));
        if (!isExists) {
            typeTableAdd(MIB_TYPE_TBL_R, (yyvsp[-3].str), CATE_CUSTOM, NULL);
        }
    }
#line 1712 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 340 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        switchingState *pState = getCurSwInfo();

        if (SW_STATE(pState) == DISPATCH_MODE_SYMBOL_COLLECTING) {
            // In include context mark the module scan is already done.
            // Todo: type checking second pass should deal here but not at
            //       <<EOF>> of flex.
        } else if (SW_STATE(pState) == DISPATCH_MODE_DOC_GENERATING) {
            // In mibTreeGen context we should merge seperate trees into one.
            if (MIB_TREE_R->numOfTree > 0) {
                mibTreeHeadMerge(MIB_TREE_R);
                mibTreeHeadComplete(MIB_TREE_R, SYMBOL_TBL_R);
                mibTreeHeadOidComplete(MIB_TREE_R);
            }
        }
    }
#line 1733 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 358 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        // Todo: fix the length value to be correctly.
        (yyval.sq).identifier = (yyvsp[-5].str);
        (yyval.sq).length = -1;
        sequence_item *head = seqItemNext(&(yyvsp[-1].si));
        seqItemAppend(&(yyval.sq).items, head);

        typeTableAdd(&mibTypeTbl, strdup((yyvsp[-5].str)), CATE_SEQUENCE, &(yyval.sq));
    }
#line 1747 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 369 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        sequence_item *newItem = seqItemConst();
        newItem->ident = (yyvsp[-4].str);
        newItem->type = (yyvsp[-3].str);
        seqItemAppend(&(yyval.si), newItem);
    }
#line 1758 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 375 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        sequence_item *newItem = seqItemConst();
        newItem->ident = (yyvsp[-3].str);
        newItem->type = (yyvsp[-2].str);
        seqItemAppend(&(yyval.si), newItem);
    }
#line 1769 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 386 "./src/yy_syn.y" /* yacc.c:1646  */
    {}
#line 1775 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 389 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
        disParamStore(param, disParamConstruct((yyvsp[-3].str)));
        dispatch(DISPATCH_PARAM_STAGE, param);

        param = disParamConstruct(OBJECT_IDENTIFIER);
        dispatch(MIBTREE_GENERATION, param);
    }
#line 1788 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 411 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        dispatchParam *param = disParamConstruct(OBJECT_IDENTIFIER);
        dispatch(MIBTREE_GENERATION, param);
    }
#line 1797 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 417 "./src/yy_syn.y" /* yacc.c:1646  */
    {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		disParamStore(param, disParamConstruct((yyvsp[-6].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_PARENT);
		disParamStore(param, disParamConstruct((yyvsp[-2].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_OID_SUFFIX);
		disParamStore(param, disParamConstruct((yyvsp[-1].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);
}
#line 1815 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 433 "./src/yy_syn.y" /* yacc.c:1646  */
    { dispatch(MIBTREE_GENERATION, disParamConstruct(OBJECT)); }
#line 1821 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 436 "./src/yy_syn.y" /* yacc.c:1646  */
    {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		disParamStore(param, disParamConstruct((yyvsp[-1].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);
	}
#line 1831 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 469 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        if (SW_STATE(getCurSwInfo()) == DISPATCH_MODE_DOC_GENERATING) {
            dispatchParam *param = disParamConstruct(SLICE_TYPE);
            disParamStore(param, disParamConstruct((yyvsp[-1].str)));
            dispatch(DISPATCH_PARAM_STAGE, param);
        }
    }
#line 1843 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 479 "./src/yy_syn.y" /* yacc.c:1646  */
    {

    }
#line 1851 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 484 "./src/yy_syn.y" /* yacc.c:1646  */
    {

    }
#line 1859 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 491 "./src/yy_syn.y" /* yacc.c:1646  */
    {

    }
#line 1867 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 494 "./src/yy_syn.y" /* yacc.c:1646  */
    {

    }
#line 1875 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 498 "./src/yy_syn.y" /* yacc.c:1646  */
    {

    }
#line 1883 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 516 "./src/yy_syn.y" /* yacc.c:1646  */
    {
		dispatchParam *param = disParamConstruct(SLICE_PERMISSION);
		disParamStore(param, disParamConstruct((yyvsp[0].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);
	}
#line 1893 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 529 "./src/yy_syn.y" /* yacc.c:1646  */
    {
		dispatchParam *param = disParamConstruct(SLICE_DESCRIPTION);
		disParamStore(param, disParamConstruct((yyvsp[0].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);
	}
#line 1903 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 563 "./src/yy_syn.y" /* yacc.c:1646  */
    {
		dispatchParam *param = disParamConstruct(SLICE_PARENT);
		disParamStore(param, disParamConstruct((yyvsp[-2].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_OID_SUFFIX);
	    disParamStore(param, disParamConstruct((yyvsp[-1].str)));
		dispatch(DISPATCH_PARAM_STAGE, param);
	}
#line 1917 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 572 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        lastProcessed = NULL;

        dispatchParam *param = disParamConstruct(SLICE_PARENT);
        disParamStore(param, disParamConstruct((yyvsp[-2].str)));
        dispatch(DISPATCH_PARAM_STAGE, param);

        param = disParamConstruct(SLICE_OID_SUFFIX);
        disParamStore(param, disParamConstruct((yyvsp[-1].str)));
        dispatch(DISPATCH_PARAM_STAGE, param);
    }
#line 1933 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 585 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        (yyval.str) = (yyvsp[-1].str);
        if (!symbolTableSearch(&symTable, (yyvsp[-1].str)))
            symbolTableAdd(&symTable, symbolNodeConst((yyvsp[-1].str), lastProcessed, (yyvsp[0].str)));
        lastProcessed = (yyvsp[-1].str);
    }
#line 1944 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 591 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        if (!symbolTableSearch(&symTable, (yyvsp[-3].str)))
            symbolTableAdd(&symTable, symbolNodeConst((yyvsp[-3].str), lastProcessed, (yyvsp[-2].str)));

        lastProcessed = (yyvsp[-3].str);

        if (!symbolTableSearch(&symTable, (yyvsp[-1].str)))
            symbolTableAdd(&symTable, symbolNodeConst((yyvsp[-1].str), lastProcessed, (yyvsp[0].str)));

        lastProcessed = (yyvsp[-1].str);
    }
#line 1960 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 604 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        (yyval.str) = (yyvsp[-1].str);
    }
#line 1968 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 607 "./src/yy_syn.y" /* yacc.c:1646  */
    {
        (yyval.str) = strdup("-1");
    }
#line 1976 "./src/yy_syn.tab.c" /* yacc.c:1646  */
    break;


#line 1980 "./src/yy_syn.tab.c" /* yacc.c:1646  */
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
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
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
                      yytoken, &yylval);
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

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
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
  return yyresult;
}
#line 611 "./src/yy_syn.y" /* yacc.c:1906  */


// Epilogue
extern YYSTYPE yylval;
void yyerror(char const *s) {
    fprintf(stderr, "Error occur during parsing %s at line %d: %s\n",
        SW_CUR_FILE_NAME(&swState), yylineno, yytext);
}
