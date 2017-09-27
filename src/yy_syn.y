%define api.pure full
%parse-param {int flag}
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

%code top {
    #include <stdio.h>
    #include "type.h"
    #include "mibTreeGen.h"
    #include "mibTreeObjTree.h"

    #define YYSTYPE char *

	params_t paramHead;
	
    extern char *yylval;
    void yyerror(char const *s) {
        fprintf(stderr, "%s: %s\n", s, yylval);
    }
}

%%
MIB : 
	DEFINITION IMPORT MAIN_PART    { };
MAIN_PART : 
	OBJ_DEAL MAIN_PART 
	| OBJ MAIN_PART 
	| TRAP MAIN_PART 
	| SEQUENCE MAIN_PART 
	| END_    { } ;

DEFINITION : IDENTIFIER DEF ASSIGNED BEGIN_    { };

IMPORT : 
	IMPORTS_ MODULES SEMICOLON    { };
MODULES : 
	ITEMS FROM_ IDENTIFIER MODULES 
	| /* empty */  ;
ITEMS : 
	IDENTIFIER 
	| IDENTIFIER COMMA ITEMS 
	| /* empty */ ;

SEQUENCE : IDENTIFIER ASSIGNED SEQ L_BRACE SEQ_ITEM R_BRACE { };
SEQ_ITEM : IDENTIFIER TYPE COMMA SEQ_ITEM |
           IDENTIFIER TYPE ;
SMI :
    "SMI" IDENTIFIER {
        params_t *param  = buildParam(IDENTIFIER_EL); 
        params_t *paramNext = buildParam(NULL);
        
        paramNext->param = (void *)malloc(strlen($1));
        strncpy(paramNext->param, $1, strlen($1));
        param->next = paramNext;

        dispatch(DISPATCH_PARAM_STAGE, param);
        dispatch(SYMBOL_COLLECTING, buildParam(SMI_DEF));
    };


OBJ_DEAL : OBJ_IDENTIFIER { deal_with(OBJECT_IDENTIFIER); }
OBJ_IDENTIFIER : 
	IDENTIFIER OBJ_IDEN_ ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
        
 		appendElement_el(&elistHead, buildElement(IDENTIFIER_EL, $1));
        appendElement_el(&elistHead, buildElement(PARENT_EL, $5));
        appendElement_el(&elistHead, buildElement(SUFFIX_EL, $6));                                                                                
};

OBJ : 
	HEAD BODY    { deal_with(OBJECT); };
TRAP :  
	TRAP_HEAD PROPERTY    { deal_with(TRAP); };
TRAP_HEAD : 
	IDENTIFIER TRAP_SPECIFIER    { appendElement_el(&elistHead, buildElement(IDENTIFIER_EL, $1)); } ;
HEAD : 
	IDENTIFIER OBJ_SPECIFIER    { appendElement_el(&elistHead, buildElement(IDENTIFIER_EL, $1)); } ;
BODY : 
	PROPERTY    {};
PROPERTY :  
	SYNTAX PROPERTY 
	| ACCESS PROPERTY
	| STATUS PROPERTY 
	| DESCRIPTION PROPERTY
	| INDEX PROPERTY 
 	| MOUNT PROPERTY 
 	| OBJECT PROPERTY 
 	| /* empty */    { };
            
OBJECT : 
	OBJECTS_ L_BRACE OBJECT_ITEM R_BRACE
	
OBJECT_ITEM : 
	IDENTIFIER COMMA OBJECT_ITEM  
	| IDENTIFIER ;
	
SYNTAX : 
	SYNTAX_SPECIFIER SYNTAX_VALUE ;
SYNTAX_VALUE : 
	TYPE    { appendElement_el(&elistHead, buildElement(TYPE_EL, $1)); };
 	| IDENTIFIER    { appendElement_el(&elistHead, buildElement(TYPE_EL, $1)); };
ACCESS : ACCESS_SPECIFIER ACCESS_VALUE    { appendElement_el(&elistHead, buildElement(RW_EL, $2)); };
STATUS : STATUS_SPECIFIER STATUS_VALUE    {};
DESCRIPTION : DESC_SPECIFIER DESC_VALUE    { appendElement_el(&elistHead, buildElement(DESCRIPTION_EL, $2)); };
INDEX : INDEX_ L_BRACE INDEX_ITEM R_BRACE    {};
INDEX_ITEM : 
	IDENTIFIER COMMA INDEX_ITEM 
	| IDENTIFIER ;
MOUNT : ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
	appendElement_el(&elistHead, buildElement(PARENT_EL, $3));
	appendElement_el(&elistHead, buildElement(SUFFIX_EL, $4));
};
%%

