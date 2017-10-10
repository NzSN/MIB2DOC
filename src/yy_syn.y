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
    #include "dispatcher.h"
    #define YYSTYPE char *

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
    | SMI MAIN_PART
	| END_    { } ;

DEFINITION :
	IDENTIFIER DEF ASSIGNED BEGIN_    { };

IMPORT :
	IMPORTS_ MODULES SEMICOLON    { /* Build upper layer of mibtree */ };
MODULES :
	ITEMS FROM_ IDENTIFIER MODULES {
		params_t *param = buildParam($3);
		param->next = buildParam($1);
		if (dispatch(SWITCH_TO_INC_BUFFER, param) == abort_terminate) {
            return abort_terminate;
        }
	}
	| /* empty */  ;
ITEMS :
	IDENTIFIER
	| IDENTIFIER COMMA ITEMS
	| /* empty */ ;

SEQUENCE :
	IDENTIFIER ASSIGNED SEQ L_BRACE SEQ_ITEM R_BRACE { };
SEQ_ITEM :
	IDENTIFIER TYPE COMMA SEQ_ITEM
	| IDENTIFIER TYPE    { };
SMI :
    "SMI" IDENTIFIER {
        params_t *param  = buildParam(IDENTIFIER_EL);
        params_t *paramNext = buildParam($1);

        dispatch(DISPATCH_PARAM_STAGE, param);
        dispatch(MIBTREE_GENERATION, buildParam(SMI_DEF));
    };


OBJ_DEAL :
	OBJ_IDENTIFIER { deal_with(OBJECT_IDENTIFIER); }
OBJ_IDENTIFIER :
	IDENTIFIER OBJ_IDEN_ ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
		params_t *param = buildParam(IDENTIFIER_EL);
		param->next = buildParam($1);
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = buildParam(PARENT_EL);
		param->next = buildParam($5);
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = buildParam(SUFFIX_EL);
		param->next = buildParam($6);
		dispatch(DISPATCH_PARAM_STAGE, param);
};

OBJ :
	HEAD BODY { dispatch(MIBTREE_GENERATION, buildParam(OBJECT)); };
TRAP :
	TRAP_HEAD PROPERTY    { dispatch(MIBTREE_GENERATION, buildParam(TRAP)); };
TRAP_HEAD :
	IDENTIFIER TRAP_SPECIFIER {
		params_t *param = buildParam(IDENTIFIER_EL);
		param->next = buildParam($1);

		dispatch(DISPATCH_PARAM_STAGE, param);
	};
HEAD :
	IDENTIFIER OBJ_SPECIFIER {
		params_t *param = buildParam(IDENTIFIER_EL);
		param->next = buildParam($1);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};
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
	TYPE {
		params_t *param = buildParam(TYPE_EL);
		param->next = buildParam($1);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};
 	| IDENTIFIER {
 		params_t *param = buildParam(TYPE_EL);
		param->next = buildParam($1);
		dispatch(DISPATCH_PARAM_STAGE, param);
 	};
ACCESS :
	ACCESS_SPECIFIER ACCESS_VALUE {
		params_t *param = buildParam(RW_EL);
		param->next = buildParam($2);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};
STATUS :
	STATUS_SPECIFIER STATUS_VALUE {};
DESCRIPTION :
	DESC_SPECIFIER DESC_VALUE {
		params_t *param = buildParam(DESCRIPTION_EL);
		param->next = buildParam($2);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};
INDEX : INDEX_ L_BRACE INDEX_ITEM R_BRACE    {};
INDEX_ITEM :
	IDENTIFIER COMMA INDEX_ITEM
	| IDENTIFIER    { };
MOUNT :
	ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
		params_t *param = buildParam(PARENT_EL);
		param->next = buildParam($3);
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = buildParam(SUFFIX_EL);
		param->next = buildParam($4);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

%%
