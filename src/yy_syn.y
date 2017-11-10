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
	IMPORTS_ MODULES SEMICOLON    {
        /* build upper tree if all including is finished */
        if (swState.importStackIndex == 0) {
            /* importStack is empty */
            upperTreeGeneration(&symTable);
        } else {
            /* do nothing */
        }
    };
MODULES :
	ITEMS FROM_ IDENTIFIER MODULES {
		dispatchParam *param = disParamConstruct($3);
		param->next = disParamConstruct($1);
		if (dispatch(SWITCH_TO_INC_BUFFER, param) == abort_terminate) {
            return abort_terminate;
        }
	} |
    /* empty */  ;
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
        dispatchParam *param  = disParamConstruct(SLICE_IDENTIFIER);
        dispatchParam *paramNext = disParamConstruct($1);

        dispatch(DISPATCH_PARAM_STAGE, param);
        dispatch(MIBTREE_GENERATION, disParamConstruct(SMI_DEF));
    };


OBJ_DEAL :
	OBJ_IDENTIFIER { mibObjGen(OBJECT_IDENTIFIER); }
OBJ_IDENTIFIER :
	IDENTIFIER OBJ_IDEN_ ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		param->next = disParamConstruct($1);
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_PARENT);
		param->next = disParamConstruct($5);
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_OID_SUFFIX);
		param->next = disParamConstruct($6);
		dispatch(DISPATCH_PARAM_STAGE, param);
};

OBJ :
	HEAD BODY { dispatch(MIBTREE_GENERATION, disParamConstruct(OBJECT)); };
TRAP :
	TRAP_HEAD PROPERTY    { dispatch(MIBTREE_GENERATION, disParamConstruct(TRAP)); };
TRAP_HEAD :
	IDENTIFIER TRAP_SPECIFIER {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		param->next = disParamConstruct($1);

		dispatch(DISPATCH_PARAM_STAGE, param);
	};
HEAD :
	IDENTIFIER OBJ_SPECIFIER {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		param->next = disParamConstruct($1);
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
		dispatchParam *param = disParamConstruct(SLICE_TYPE);
		param->next = disParamConstruct($1);
		dispatch(DISPATCH_PARAM_STAGE, param);
    }
    | IDENTIFIER {
 		dispatchParam *param = disParamConstruct(SLICE_TYPE);
		param->next = disParamConstruct($1);
		dispatch(DISPATCH_PARAM_STAGE, param);
 	};
ACCESS :
	ACCESS_SPECIFIER ACCESS_VALUE {
		dispatchParam *param = disParamConstruct(SLICE_PERMISSION);
		param->next = disParamConstruct($2);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};
STATUS :
	STATUS_SPECIFIER STATUS_VALUE {};
DESCRIPTION :
	DESC_SPECIFIER DESC_VALUE {
		dispatchParam *param = disParamConstruct(SLICE_DESCRIPTION);
		param->next = disParamConstruct($2);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};
INDEX : INDEX_ L_BRACE INDEX_ITEM R_BRACE    {};
INDEX_ITEM :
	IDENTIFIER COMMA INDEX_ITEM
	| IDENTIFIER    { };
MOUNT :
	ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
		dispatchParam *param = disParamConstruct(SLICE_PARENT);
		param->next = disParamConstruct($3);
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_OID_SUFFIX);
		param->next = disParamConstruct($4);
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

%%
