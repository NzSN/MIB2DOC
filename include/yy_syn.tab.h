/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

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
    STATUS_SPECIFIER = 261,
    STATUS_VALUE = 262,
    DESC_SPECIFIER = 263,
    MOUNT_POINT = 264,
    ASSIGNED = 265,
    BEGIN_ = 266,
    END_ = 267,
    DEF = 268,
    SEQ = 269,
    COMMA = 270,
    SEMICOLON = 271,
    INDEX_ = 272,
    AUGMENTS_ = 273,
    TRAP_SPECIFIER = 274,
    OBJ_IDEN_ = 275,
    L_BRACE = 276,
    R_BRACE = 277,
    OBJECTS_ = 278,
    FROM_ = 279,
    IMPORTS_ = 280,
    L_PAREN = 281,
    R_PAREN = 282,
    HEX_STRING = 283,
    UNITS_SPECIFIER = 284,
    DEFVAL_SPECIFIER = 285,
    DISPLAY_HINT = 286,
    REFERENCE = 287,
    TC_SPECIFIER = 288,
    COMPLIANCE_SPECIFIER = 289,
    MIN_ACCESS_SPECIFIER = 290,
    OBJECT_ = 291,
    NOTIFY_SPECIFIER = 292,
    NOTIFY_GRP_SPECIFIER = 293,
    MODULE_SPECIFIER = 294,
    MANDATORY_SPECIFIER = 295,
    GROUP_SPECIFIER = 296,
    OBJECT_SPECIFIER = 297,
    WRITE_SPECIFIER = 298,
    OBJ_GRP_SPECIFIER = 299,
    TO = 300,
    OR = 301,
    SIZE = 302,
    OBJ_IDENTITY_SPECIFIER = 303,
    NOTIFY_TYPE_SPECIFIER = 304,
    SMI_SPECIFIER = 305,
    SMI_VAL = 306,
    MOD_SPECIFIER = 307,
    LAST_UPDATED = 308,
    ORGANIZATION = 309,
    REVISION = 310,
    CONTACT_INFO = 311,
    REVISION_DATE = 312,
    IDENTIFIER = 313,
    NUM = 314,
    TYPE_BUILT_IN = 315,
    ACCESS_VALUE = 316,
    STRING = 317
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 44 "./src/yy_syn.y" /* yacc.c:1921  */

    char *str;
#line 62 "./src/yy_syn.y" /* yacc.c:1921  */

    struct sequence_item si;
    struct sequence sq;    

#line 129 "./src/yy_syn.tab.h" /* yacc.c:1921  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_YY_SYN_TAB_H_INCLUDED  */
