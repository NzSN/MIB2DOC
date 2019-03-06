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
    TRAP_SPECIFIER = 273,
    OBJ_IDEN_ = 274,
    L_BRACE = 275,
    R_BRACE = 276,
    OBJECTS_ = 277,
    FROM_ = 278,
    IMPORTS_ = 279,
    SMI_SPECIFIER = 280,
    SMI_VAL = 281,
    MOD_SPECIFIER = 282,
    LAST_UPDATED = 283,
    ORGANIZATION = 284,
    REVISION = 285,
    CONTACT_INFO = 286,
    REVISION_DATE = 287,
    IDENTIFIER = 288,
    NUM = 289,
    TYPE = 290,
    ACCESS_VALUE = 291,
    DESC_VALUE = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 12 "./src/yy_syn.y" /* yacc.c:1909  */

    char *str;
#line 29 "./src/yy_syn.y" /* yacc.c:1909  */

    struct sequence_item *si;
    struct sequence *sq;    

#line 100 "./src/yy_syn.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_YY_SYN_TAB_H_INCLUDED  */
