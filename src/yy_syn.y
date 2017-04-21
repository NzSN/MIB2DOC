%token IDENTIFIER OBJ_SPECIFIER
%token SYNTAX_SPECIFIER SYNTAX_VALUE
%token ACCESS_SPECIFIER ACCESS_VALUE
%token STATUS_SPECIFIER STATUS_VALUE
%token DESC_SPECIFIER DESC_VALUE
%token MOUNT_POINT ASSIGNED
%token BEGIN_ END_ DEF SEQ
%token COMMA SEMICOLON
%token OBJ_IDEN_ LBRACE RBRACE
%token TYPE NUM FROM_ IMPORTS_

%{
    #include <stdio.h>
    #include "../include/type.h"

    #define YYSTYPE char *
    extern char *yylval;
    void yyerror(char const *s) {
        fprintf(stderr, "%d %s: %s\n", s, yylval);
    }
%}

%%
MIB : DEFINITION IMPORT MAIN_PART
MAIN_PART : OBJ_IDENTIFIER MAIN_PART |
            OBJ MAIN_PART |
            SEQUENCE MAIN_PART |
            END_                            { printf("%s\n", "MAIN_PART"); } ;

DEFINITION : IDENTIFIER DEF ASSIGNED BEGIN_ { printf("%s\n", "DEFINITIONS"); };

IMPORT : IMPORTS_ MODULES SEMICOLON { printf("%s\n", "IMPORT"); };
MODULES : ITEMS FROM_ IDENTIFIER MODULES |
          /* empty */  ;
ITEMS : IDENTIFIER |
        IDENTIFIER COMMA ITEMS |
        /* empty */

SEQUENCE : IDENTIFIER ASSIGNED SEQ LBRACE SEQ_ITEM RBRACE ;
SEQ_ITEM : IDENTIFIER TYPE COMMA SEQ_ITEM |
           IDENTIFIER TYPE ;

OBJ_IDENTIFIER : IDENTIFIER OBJ_IDEN_ MOUNT { printf("%s\n", "OBJ_IDENTIFIER"); };

OBJ :  HEAD BODY                        { printf("%s\n", "OBJ"); } ;

HEAD : IDENTIFIER OBJ_SPECIFIER         { printf("%s\n", "HEAD"); } ;
BODY : PROPERTY                         { printf("%s\n", "BODY"); } ;
PROPERTY :  SYNTAX PROPERTY |
            ACCESS PROPERTY |
            STATUS PROPERTY |
            DESCRIPTION PROPERTY |
            MOUNT PROPERTY |
            /* empty */                 { printf("%s\n", "PROPERTY"); };
SYNTAX : SYNTAX_SPECIFIER SYNTAX_VALUE  { printf("%s: %s\n", "SYNTAX", yylval); };
ACCESS : ACCESS_SPECIFIER ACCESS_VALUE  { printf("%s: %s\n", "ACCESS", yylval); };
STATUS : STATUS_SPECIFIER STATUS_VALUE  { printf("%s: %s\n", "STATUS", yylval); };
DESCRIPTION : DESC_SPECIFIER DESC_VALUE { printf("%s: %s\n", "DESCRIPTION", yylval); };
MOUNT : ASSIGNED MOUNT_POINT            { printf("%s: %s\n", "MOUNT", yylval); };
%%

