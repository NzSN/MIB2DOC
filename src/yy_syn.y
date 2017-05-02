%token IDENTIFIER OBJ_SPECIFIER
%token SYNTAX_SPECIFIER
%token ACCESS_SPECIFIER ACCESS_VALUE
%token STATUS_SPECIFIER STATUS_VALUE
%token DESC_SPECIFIER DESC_VALUE
%token MOUNT_POINT ASSIGNED
%token BEGIN_ END_ DEF SEQ
%token COMMA SEMICOLON INDEX_ TRAP_SPECIFIER
%token OBJ_IDEN_ L_BRACE R_BRACE OBJECTS_
%token TYPE NUM FROM_ IMPORTS_

%{
    #include <stdio.h>
    #include "../include/type.h"
    #include "../include/mibTreeGen.h"
    #include "../include/mibTreeObjTree.h"

    #define YYSTYPE char *

    extern char *yylval;
    void yyerror(char const *s) {
        fprintf(stderr, "%s: %s\n", s, yylval);
    }
%}

%%
MIB : DEFINITION IMPORT MAIN_PART               { };
MAIN_PART : OBJ_IDENTIFIER MAIN_PART |
            OBJ MAIN_PART |
            TRAP MAIN_PART |
            SEQUENCE MAIN_PART |
            END_                                { } ;

DEFINITION : IDENTIFIER DEF ASSIGNED BEGIN_     { };

IMPORT : IMPORTS_ MODULES SEMICOLON             { };
MODULES : ITEMS FROM_ IDENTIFIER MODULES |
          /* empty */  ;
ITEMS : IDENTIFIER |
        IDENTIFIER COMMA ITEMS |
        /* empty */ ;

SEQUENCE : IDENTIFIER ASSIGNED SEQ L_BRACE SEQ_ITEM R_BRACE { };
SEQ_ITEM : IDENTIFIER TYPE COMMA SEQ_ITEM |
           IDENTIFIER TYPE ;

OBJ_IDENTIFIER : IDENTIFIER OBJ_IDEN_ ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE { };

OBJ : HEAD BODY                                 { };
TRAP :  TRAP_HEAD PROPERTY                      { };

TRAP_HEAD : IDENTIFIER TRAP_SPECIFIER
HEAD : IDENTIFIER OBJ_SPECIFIER                 { } ;
BODY : PROPERTY                                 { } ;
PROPERTY :  SYNTAX PROPERTY |
            ACCESS PROPERTY |
            STATUS PROPERTY |
            DESCRIPTION PROPERTY |
            INDEX PROPERTY |
            MOUNT PROPERTY |
            OBJECT PROPERTY |
            /* empty */                         { };
OBJECT : OBJECTS_ L_BRACE OBJECT_ITEM R_BRACE
OBJECT_ITEM : IDENTIFIER COMMA OBJECT_ITEM  |
              IDENTIFIER ;
SYNTAX : SYNTAX_SPECIFIER SYNTAX_VALUE          { };
SYNTAX_VALUE : TYPE |
               IDENTIFIER ;
ACCESS : ACCESS_SPECIFIER ACCESS_VALUE          { appendElement_el(&elistHead, $2); };
STATUS : STATUS_SPECIFIER STATUS_VALUE          { };
DESCRIPTION : DESC_SPECIFIER DESC_VALUE         { };
INDEX : INDEX_ L_BRACE INDEX_ITEM R_BRACE       { };
INDEX_ITEM : IDENTIFIER COMMA INDEX_ITEM |
             IDENTIFIER ;
MOUNT : ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE { };
%%

