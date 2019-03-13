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

%token DISPLAY_HINT
%token REFERENCE
%token TC_SPECIFIER

%token NOTIFY_SPECIFIER

%token MODULE_SPECIFIER
%token MANDATORY_SPECIFIER
%token GROUP_SPECIFIER
%token OBJECT_SPECIFIER
%token WRITE_SPECIFIER
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
	  OBJ_DEAL      MIB
	| OBJ           MIB
	| TRAP          MIB
	| SEQUENCE      MIB
    | SMI           MIB
	| END           MIB     
    | DEFINITION    MIB
    | IMPORT        MIB
    | MODULE_DEF    MIB
    | TYPE_DEFINED  MIB
    | TC            MIB
    | OBJ_GRP       MIB
    | NOTIFY_GRP    MIB
    | COMPILANCE    MIB
    | /* empty */ ;

DEFINITION :
	IDENTIFIER DEF ASSIGNED BEGIN_ { };

IMPORT :
	IMPORTS_ MODULES SEMICOLON    {
        // Begining to import symbol from another mib files.
        importWorks(&importInfoStack); 
    };

/* Textual convention */
TC :
   IDENTIFIER ASSIGNED TC_SPECIFIER TC_NOTATION {
        typeTableAdd(MIB_TYPE_TBL_R, $IDENTIFIER, CATE_CUSTOM, NULL); 
    };

TC_NOTATION :
    DisplayPart STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART SYNTAX_SPECIFIER SYNTAX_VALUE;

DisplayPart :
    DISPLAY_HINT | /* empty */ ;

REF_PART :
    REFERENCE | /* empty */ ;

/* OBJECT-GROUP */
OBJ_GRP :
    OBJ_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART;
OBJ_PART :
    IDENTIFIER[OBJ_NAME] L_BRACE OBJS R_BRACE
OBJS : 
    IDENTIFIER 
    | IDENTIFIER COMMA OBJS;

/* NOTIFICATION-GROUP */
NOTIFY_GRP :
    NOTIFY_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART;
NOTIFY_PART :
    NOTIFY_SPECIFIER L_BRACE NOTIFICATIONS R_BRACE;
NOTIFICATIONS :
    IDENTIFIER
    | IDENTIFIER COMMA NOTIFICATIONS;

/* MODULE_COMPILANCE */
COMPILANCE :
    STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART MODULE_PART;
MODULE_PART : MODULES_;
MODULES_ : 
    MODULE_
    | MODULE_ MODULES_;
MODULE_ :
    MODULE_SPECIFIER IDENTIFIER MANDATORY_PART COMPLIANCE_PART;
MANDATORY_PART : 
    MANDATORY_SPECIFIER L_BRACE GROUPS R_BRACE; 
GROUPS: 
    Group 
    | Group COMMA GROUPS;
Group : IDENTIFIER;
COMPLIANCE_PART : 
    COMPLIANCES
    | /* empty */;
COMPLIANCES : 
    COMPLIANCE 
    | COMPLIANCE COMPLIANCES;
COMPLIANCE : 
    COMPLIANCE_GRP
    | COMPLIANCE_OBJ;
COMPLIANCE_GRP :
    GROUP_SPECIFIER IDENTIFIER
    DESC_SPECIFIER DESC_VALUE;
COMPLIANCE_OBJ : 
    OBJECT_SPECIFIER IDENTIFIER COMPLIANCE_SYNTAX COMPLIANCE_WRITE COMPLIANCE_ACCESS DESC_SPECIFIER DESC_VALUE; 
COMPLIANCE_SYNTAX : 
    SYNTAX_SPECIFIER SYNTAX_VALUE
    | /* empty */;
COMPLIANCE_WRITE : 
    WRITE_SPECIFIER SYNTAX_VALUE
    | /* empty */;
COMPLIANCE_ACCESS : 
    ACCESS_SPECIFIER ACCESS_VALUE
    | /* empty */;

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
                errorMsg("IMPORT: Symbol conflict.\n"); 
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
	SYNTAX
	ACCESS
	STATUS
	DESCRIPTION
	INDEX
 	MOUNT
 	OBJECT
 	| /* empty */;

OBJECT :
	OBJECTS_ L_BRACE OBJECT_ITEM R_BRACE | /* empty */;

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

