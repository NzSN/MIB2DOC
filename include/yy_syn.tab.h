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
    L_PAREN = 282,
    R_PAREN = 283,
    HEX_STRING = 284,
    UNITS_SPECIFIER = 285,
    DEFVAL_SPECIFIER = 286,
    DISPLAY_HINT = 287,
    REFERENCE = 288,
    TC_SPECIFIER = 289,
    COMPLIANCE_SPECIFIER = 290,
    MIN_ACCESS_SPECIFIER = 291,
    OBJECT_ = 292,
    NOTIFY_SPECIFIER = 293,
    NOTIFY_GRP_SPECIFIER = 294,
    MODULE_SPECIFIER = 295,
    MANDATORY_SPECIFIER = 296,
    GROUP_SPECIFIER = 297,
    OBJECT_SPECIFIER = 298,
    WRITE_SPECIFIER = 299,
    OBJ_GRP_SPECIFIER = 300,
    TO = 301,
    OR = 302,
    SIZE = 303,
    OBJ_IDENTITY_SPECIFIER = 304,
    NOTIFY_TYPE_SPECIFIER = 305,
    SMI_SPECIFIER = 306,
    SMI_VAL = 307,
    MOD_SPECIFIER = 308,
    LAST_UPDATED = 309,
    ORGANIZATION = 310,
    REVISION = 311,
    CONTACT_INFO = 312,
    REVISION_DATE = 313,
    IDENTIFIER = 314,
    NUM = 315,
    TYPE_BUILT_IN = 316,
    ACCESS_VALUE = 317,
    STRING = 318
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 45 "./src/yy_syn.y" /* yacc.c:1909  */

    char *str;
#line 63 "./src/yy_syn.y" /* yacc.c:1909  */

    // SEQUENCE
    struct sequence_item si;
    struct sequence sq;

#line 127 "./src/yy_syn.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_YY_SYN_TAB_H_INCLUDED  */
