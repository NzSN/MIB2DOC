/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

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
    DEFVAL_SPECIFIER = 288,
    DISPLAY_HINT = 289,
    REFERENCE = 290,
    TC_SPECIFIER = 291,
    COMPLIANCE_SPECIFIER = 292,
    MIN_ACCESS_SPECIFIER = 293,
    OBJECT_ = 294,
    NOTIFY_SPECIFIER = 295,
    NOTIFY_GRP_SPECIFIER = 296,
    MODULE_SPECIFIER = 297,
    MANDATORY_SPECIFIER = 298,
    GROUP_SPECIFIER = 299,
    OBJECT_SPECIFIER = 300,
    WRITE_SPECIFIER = 301,
    OBJ_GRP_SPECIFIER = 302,
    TO = 303,
    OR = 304,
    SIZE = 305,
    OBJ_IDENTITY_SPECIFIER = 306,
    NOTIFY_TYPE_SPECIFIER = 307,
    SMI_SPECIFIER = 308,
    SMI_VAL = 309,
    MOD_SPECIFIER = 310,
    LAST_UPDATED = 311,
    ORGANIZATION = 312,
    REVISION = 313,
    CONTACT_INFO = 314,
    REVISION_DATE = 315,
    IDENTIFIER = 316,
    NUM = 317,
    TYPE_BUILT_IN = 318,
    ACCESS_VALUE = 319,
    STRING = 320
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 47 "./src/yy_syn.y" /* yacc.c:1909  */

    char *str;
#line 65 "./src/yy_syn.y" /* yacc.c:1909  */

    // SEQUENCE
    struct sequence_item si;
    struct sequence sq;

#line 129 "./src/yy_syn.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_YY_SYN_TAB_H_INCLUDED  */
