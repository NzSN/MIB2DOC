%token OBJ_SPECIFIER
%token SYNTAX_SPECIFIER
%token ACCESS_SPECIFIER
%token STATUS_SPECIFIER STATUS_VALUE
%token DESC_SPECIFIER 
%token MOUNT_POINT ASSIGNED
%token BEGIN_ END_ DEF SEQ
%token COMMA SEMICOLON INDEX_ TRAP_SPECIFIER
%token OBJ_IDEN_ L_BRACE R_BRACE OBJECTS_
%token FROM_ IMPORTS_

%union {
    char *str;
}

%token SMI_SPECIFIER
%token <str> SMI_VAL

%token MOD_SPECIFIER LAST_UPDATED
%token ORGANIZATION REVISION CONTACT_INFO
%token <str> REVISION_DATE

%token <str> IDENTIFIER
%token <str> NUM
%token <str> TYPE_BUILT_IN
%token <str> ACCESS_VALUE
%token <str> DESC_VALUE


%union {
    struct sequence_item si;
    struct sequence sq;    
}

%type <str> TYPE
%type <si> SEQ_ITEM
%type <sq> SEQUENCE

// Prologue
%code top {
    #include <stdio.h>
    #include "type.h"
    #include "symbolTbl.h"
    #include "string.h"
    #include "yy_syn.def.h"
    #include "typeCheck.h"
    #include "dispatcher.h"
    #include "mibTreeGen.h"

    extern typeTable mibTypeTbl;
    extern symbolTable symTable;     
    dispatchParam importParam;
    genericStack importInfoStack; 
     
    int syntaxParserInit(void) {
        memset(&importParam, 0, sizeof(dispatchParam));
        genericStackConstruct(&importInfoStack, 20, sizeof(collectInfo *)); 
    }
}

// Grammar rules
%%

MIB :
	  OBJ_DEAL MIB
	| OBJ MIB
	| TRAP MIB
	| SEQUENCE MIB
    | SMI MIB
	| END MIB     
    | DEFINITION MIB
    | IMPORT MIB
    | MODULE_DEF MIB
    | TYPE_DEFINED MIB
    | /* empty */ ;

DEFINITION :
	IDENTIFIER DEF ASSIGNED BEGIN_ { };

IMPORT :
	IMPORTS_ MODULES SEMICOLON    {
        // Begining to import symbol from another mib files.
        importWorks(&importInfoStack); 
    };

TYPE :
    TYPE_BUILT_IN { 
        // Correctness of BUILT_IN type was checked by lexer
        $TYPE = $TYPE_BUILT_IN; 
    }
    | IDENTIFIER { 
        //  Customed type 
        $TYPE = $IDENTIFIER;  
        
        if (typeCheck_isValid(MIB_TYPE_TBL_R, $TYPE) == FALSE) {
            errorMsg("%s is not a valid type.\n", $TYPE);
            exit(1);
        }
    }

TYPE_DEFINED :
    IDENTIFIER ASSIGNED TYPE {
        _Bool isExists = typeTableIsTypeExists(MIB_TYPE_TBL_R, $IDENTIFIER);
        if (isExists) {
            errorMsg("%s is already exists.\n", $IDENTIFIER);
            abort();
        }
        typeTableAdd(MIB_TYPE_TBL_R, $IDENTIFIER, CATE_CUSTOM, NULL);         
    }

END :
    END_ {
        switchingState *pState = getCurSwInfo();

        if (SW_STATE((*pState)) == DISPATCH_MODE_SYMBOL_COLLECTING) {
            // In include context mark the module scan is already done.
        } else if (SW_STATE((*pState)) == DISPATCH_MODE_DOC_GENERATING) {
            // In mibTreeGen context we should merge seperate trees into one.
            mibTreeHeadMerge(MIB_TREE_R);  
            mibTreeHeadComplete(MIB_TREE_R, SYMBOL_TBL_R);
            mibTreeHeadOidComplete(MIB_TREE_R);
        }
    }

MODULES :
	MODULES_CONTENT MODULES 
    | /* empty */  ;

MODULES_CONTENT :
    ITEMS FROM_ IDENTIFIER {
        dispatchParam *current;    
        current = &importParam;
        
        int ret = TRUE;
        collectInfo *importInfo = collectInfoConst($IDENTIFIER);
         
        // Store symbols that should be included.
        while (current = dispatchParamNext(current)) {
            ret = collectInfo_add(importInfo, current->param);
            if (ret == FALSE) {
                printf("IMPORT: Symbol conflict.\n"); 
                exit(1);
            } 
        }
        disParamRelease_Static(&importParam, NULL); 

        push(&importInfoStack, &importInfo); 
    }

ITEMS :
	IDENTIFIER { 
        if (disParamStore(&importParam, disParamConstruct($IDENTIFIER)) == NULL) {
            exit(1); 
        }
    }
	| IDENTIFIER COMMA ITEMS { 
        if (disParamStore(&importParam, disParamConstruct($IDENTIFIER)) == NULL) {
            exit(1); 
        }
    }
	| /* empty */ ;

SEQUENCE :
	IDENTIFIER ASSIGNED SEQ L_BRACE SEQ_ITEM R_BRACE {
        // Todo: fix the length value to be correctly.
        $SEQUENCE.identifier = $IDENTIFIER; 
        $SEQUENCE.length = -1;
        sequence_item *head = seqItemNext(&$SEQ_ITEM);
        seqItemAppend(&$SEQUENCE.items, head);

        typeTableAdd(&mibTypeTbl, strdup($IDENTIFIER), CATE_SEQUENCE, &$SEQUENCE); 
    };

SEQ_ITEM :
	IDENTIFIER TYPE COMMA SEQ_ITEM {
        sequence_item *newItem = seqItemConst();
        newItem->ident = $IDENTIFIER;
        newItem->type = $TYPE;
        seqItemAppend(&$$, newItem); 

    }
	| IDENTIFIER TYPE {
        sequence_item *newItem = seqItemConst();
        newItem->ident = $IDENTIFIER;
        newItem->type = $TYPE; 
        seqItemAppend(&$$, newItem); 
    };

SMI :
    SMI_SPECIFIER SMI_VAL {};

MODULE_DEF :
    IDENTIFIER MOD_SPECIFIER MODULE_BODY;

MODULE_BODY : 
    LAST_UPDATED REVISION_DATE ORGANIZATION DESC_VALUE CONTACT_INFO DESC_VALUE DESC_SPECIFIER DESC_VALUE REVISIONS MOUNT {

    };

REVISIONS : 
    REVISION REVISION_DATE DESC_SPECIFIER DESC_VALUE REVISIONS {
         
    }
    | /* empty */;

OBJ_DEAL :
	OBJ_IDENTIFIER { 
        dispatchParam *param = disParamConstruct(OBJECT_IDENTIFIER);
        dispatch(MIBTREE_GENERATION, param);
    };

OBJ_IDENTIFIER :
	IDENTIFIER[chil] OBJ_IDEN_ ASSIGNED L_BRACE IDENTIFIER[parent] NUM R_BRACE {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		disParamStore(param, disParamConstruct($chil));
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_PARENT);
		disParamStore(param, disParamConstruct($parent));
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_OID_SUFFIX);
		disParamStore(param, disParamConstruct($NUM));
		dispatch(DISPATCH_PARAM_STAGE, param);
};

OBJ :
	HEAD BODY { dispatch(MIBTREE_GENERATION, disParamConstruct(OBJECT)); };

TRAP :
	TRAP_HEAD PROPERTY    { dispatch(MIBTREE_GENERATION, disParamConstruct(TRAP)); };

TRAP_HEAD :
	IDENTIFIER TRAP_SPECIFIER {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		disParamStore(param, disParamConstruct($IDENTIFIER));

		dispatch(DISPATCH_PARAM_STAGE, param);
	};

HEAD :
	IDENTIFIER OBJ_SPECIFIER {
		dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
		disParamStore(param, disParamConstruct($IDENTIFIER));
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

BODY :
	PROPERTY;

PROPERTY :
	SYNTAX PROPERTY
	| ACCESS PROPERTY
	| STATUS PROPERTY
	| DESCRIPTION PROPERTY
	| INDEX PROPERTY
 	| MOUNT PROPERTY
 	| OBJECT PROPERTY
 	| /* empty */;

OBJECT :
	OBJECTS_ L_BRACE OBJECT_ITEM R_BRACE;

OBJECT_ITEM :
	IDENTIFIER COMMA OBJECT_ITEM
	| IDENTIFIER ;

SYNTAX :
	SYNTAX_SPECIFIER SYNTAX_VALUE ;

SYNTAX_VALUE :
	TYPE {
		dispatchParam *param = disParamConstruct(SLICE_TYPE);
	    disParamStore(param, disParamConstruct($TYPE));
		dispatch(DISPATCH_PARAM_STAGE, param);
    }

ACCESS :
	ACCESS_SPECIFIER ACCESS_VALUE {
		dispatchParam *param = disParamConstruct(SLICE_PERMISSION);
		disParamStore(param, disParamConstruct($ACCESS_VALUE));
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

STATUS :
	STATUS_SPECIFIER STATUS_VALUE;

DESCRIPTION :
	DESC_SPECIFIER DESC_VALUE {
		dispatchParam *param = disParamConstruct(SLICE_DESCRIPTION);
		disParamStore(param, disParamConstruct($DESC_VALUE));
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

INDEX : INDEX_ L_BRACE INDEX_ITEM R_BRACE;

INDEX_ITEM :
	IDENTIFIER COMMA INDEX_ITEM
	| IDENTIFIER;

MOUNT :
	ASSIGNED L_BRACE IDENTIFIER NUM R_BRACE {
		dispatchParam *param = disParamConstruct(SLICE_PARENT);
		disParamStore(param, disParamConstruct($IDENTIFIER));
		dispatch(DISPATCH_PARAM_STAGE, param);

		param = disParamConstruct(SLICE_OID_SUFFIX);
	    disParamStore(param, disParamConstruct($NUM));
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

%%

// Epilogue
extern YYSTYPE yylval;
void yyerror(char const *s) {
    fprintf(stderr, "%s", s);
}    

