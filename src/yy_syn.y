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
%token COMPLIANCE_SPECIFIER
%token MIN_ACCESS_SPECIFIER
%token OBJECT_
%token ENUMERATE_MEMBER 

%token NOTIFY_SPECIFIER
%token NOTIFY_GRP_SPECIFIER

%token MODULE_SPECIFIER
%token MANDATORY_SPECIFIER
%token GROUP_SPECIFIER
%token OBJECT_SPECIFIER
%token WRITE_SPECIFIER
%token OBJ_GRP_SPECIFIER

%token OR
%token SIZE
/* OBJECT-IDENTITY TOKEN */
%token OBJ_IDENTITY_SPECIFIER

/* NOTIFICATION-TYPE */
%token NOTIFY_TYPE_SPECIFIER

%union {
    char *str;
}

%token SMI_SPECIFIER
%token <str> SMI_VAL

%token MOD_SPECIFIER LAST_UPDATED
%token ORGANIZATION REVISION CONTACT_INFO
%token <str> REVISION_DATE
 
%token <str> RANGE
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
	| SEQUENCE      MIB
    | SMI           MIB
	| END           MIB     
    | DEFINITION    MIB
    | IMPORT        MIB
    | MODULE_DEF    MIB
    | TYPE_DEFINED  MIB
    | TC_DEFINED    MIB
    | OBJ_GRP_DEFINED       MIB
    | NOTIFY_GRP_DEFINED    MIB
    | COMPLIANCE_DEFINED    MIB
    | OBJ_IDENTITY_DEFINED  MIB
    | NOTIFY_TYPE_DEFINED   MIB
    | /* empty */ ;

DEFINITION :
	IDENTIFIER DEF ASSIGNED BEGIN_ { };

IMPORT :
	IMPORTS_ MODULES SEMICOLON    {
        // Begining to import symbol from another mib files.
        importWorks(&importInfoStack); 
    };

REF_PART :
    REFERENCE | /* empty */ ;

/* OBJECT-GROUP */
OBJ_GRP_DEFINED :
    IDENTIFIER OBJ_GRP_SPECIFIER OBJ_GRP MOUNT;
OBJ_GRP :
    OBJ_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART;
OBJ_PART :
    OBJECTS_ L_BRACE OBJS R_BRACE
OBJS : 
    IDENTIFIER
    | IDENTIFIER COMMA OBJS;

/* NOTIFICATION-GROUP */
NOTIFY_GRP_DEFINED :
    IDENTIFIER NOTIFY_GRP_SPECIFIER NOTIFY_GRP MOUNT;
NOTIFY_GRP :
    NOTIFY_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART;
NOTIFY_PART :
    NOTIFY_SPECIFIER L_BRACE NOTIFICATIONS R_BRACE;
NOTIFICATIONS :
    IDENTIFIER
    | IDENTIFIER COMMA NOTIFICATIONS;

/* MODULE_COMPILANCE */
COMPLIANCE_DEFINED :
    IDENTIFIER COMPLIANCE_SPECIFIER COMPLIANCE_BODY MOUNT;
COMPLIANCE_BODY :
    STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART MODULE_PART;
MODULE_PART : MODULES_;
MODULES_ : 
    MODULE_
    | MODULE_ MODULES_;
MODULE_ :
    MODULE_SPECIFIER MODULE_NAME MANDATORY_PART COMPLIANCE_PART;
MODULE_NAME :
    IDENTIFIER 
    | /* empty */;
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
    OBJECT_ IDENTIFIER COMPLIANCE_SYNTAX COMPLIANCE_WRITE COMPLIANCE_ACCESS DESC_SPECIFIER DESC_VALUE; 
COMPLIANCE_SYNTAX : 
    SYNTAX_SPECIFIER SYNTAX_VALUE
    | /* empty */;
COMPLIANCE_WRITE : 
    WRITE_SPECIFIER SYNTAX_VALUE
    | /* empty */;
COMPLIANCE_ACCESS : 
    MIN_ACCESS_SPECIFIER ACCESS_VALUE
    | /* empty */;


/* TEXTUAL-CONVENTION */
TC_DEFINED :
    IDENTIFIER ASSIGNED TC_SPECIFIER TC {
        typeTableAdd(MIB_TYPE_TBL_R, strdup($IDENTIFIER), CATE_CUSTOM, NULL);         
    };
TC :
    DisplayPart STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART SYNTAX_SPECIFIER SYNTAX_VALUE;
DisplayPart:
    DISPLAY_HINT DESC_VALUE
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

/* OBJECT-IDENTITY */
OBJ_IDENTITY_DEFINED :
    IDENTIFIER OBJ_IDENTITY_SPECIFIER OBJ_IDENTITY;
OBJ_IDENTITY :
    STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE REF_PART MOUNT;  

/* NOTIFICATION-TYPE */
NOTIFY_TYPE_DEFINED : 
    IDENTIFIER NOTIFY_TYPE_SPECIFIER NOTIFY_TYPE MOUNT;
NOTIFY_TYPE : 
    NOTIFY_TYPE_OBJ_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER DESC_VALUE;
NOTIFY_TYPE_OBJ_PART :
    OBJ_PART
    | /* empty */;


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

        if (SW_STATE(pState) == DISPATCH_MODE_SYMBOL_COLLECTING) {
            // In include context mark the module scan is already done.
        } else if (SW_STATE(pState) == DISPATCH_MODE_DOC_GENERATING) {
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
    LAST_UPDATED DESC_VALUE
    ORGANIZATION DESC_VALUE 
    CONTACT_INFO DESC_VALUE 
    DESC_SPECIFIER DESC_VALUE 
    REVISIONS 
    MOUNT;


REVISIONS : 
    REVISION DESC_VALUE DESC_SPECIFIER DESC_VALUE REVISIONS
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
	TYPE TYPE_SPECIFIER {
		dispatchParam *param = disParamConstruct(SLICE_TYPE);
	    disParamStore(param, disParamConstruct($TYPE));
		dispatch(DISPATCH_PARAM_STAGE, param);
    }

TYPE_SPECIFIER :
    L_BRACE TYPE_SPECIFIER_ R_BRACE
    | /* empty */;
TYPE_SPECIFIER_ :
    ENUMERATE_MEMBERS
    | STRING_SIZE
    | ONE_OR_MORE_VAL;

ENUMERATE_MEMBERS :
    ENUMERATE_MEMBER 
    | ENUMERATE_MEMBER COMMA ENUMERATE_MEMBERS; 

STRING_SIZE :
    SIZE L_BRACE VAL R_BRACE;

ONE_OR_MORE_VAL :
    VAL
    | VAL OR ONE_OR_MORE_VAL ;
VAL :
    NUM
    | RANGE;

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

INDEX : INDEX_ L_BRACE INDEX_ITEM R_BRACE | /* empty */;

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
    fprintf(stderr, "Error occur during parsing %s at line %d\n", "adf", yylineno - 1);
}    

