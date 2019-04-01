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
    L_PAREN = 280,
    R_PAREN = 281,
    DISPLAY_HINT = 282,
    REFERENCE = 283,
    TC_SPECIFIER = 284,
    COMPLIANCE_SPECIFIER = 285,
    MIN_ACCESS_SPECIFIER = 286,
    OBJECT_ = 287,
    NOTIFY_SPECIFIER = 288,
    NOTIFY_GRP_SPECIFIER = 289,
    MODULE_SPECIFIER = 290,
    MANDATORY_SPECIFIER = 291,
    GROUP_SPECIFIER = 292,
    OBJECT_SPECIFIER = 293,
    WRITE_SPECIFIER = 294,
    OBJ_GRP_SPECIFIER = 295,
    TO = 296,
    OR = 297,
    SIZE = 298,
    OBJ_IDENTITY_SPECIFIER = 299,
    NOTIFY_TYPE_SPECIFIER = 300,
    SMI_SPECIFIER = 301,
    SMI_VAL = 302,
    MOD_SPECIFIER = 303,
    LAST_UPDATED = 304,
    ORGANIZATION = 305,
    REVISION = 306,
    CONTACT_INFO = 307,
    REVISION_DATE = 308,
    IDENTIFIER = 309,
    NUM = 310,
    TYPE_BUILT_IN = 311,
    ACCESS_VALUE = 312,
    DESC_VALUE = 313
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 40 "./src/yy_syn.y" /* yacc.c:1909  */

    char *str;
#line 58 "./src/yy_syn.y" /* yacc.c:1909  */

    struct sequence_item si;
    struct sequence sq;    

#line 121 "./src/yy_syn.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_YY_SYN_TAB_H_INCLUDED  */
