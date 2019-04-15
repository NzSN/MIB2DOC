%token OBJ_SPECIFIER
%token SYNTAX_SPECIFIER
%token ACCESS_SPECIFIER
%token STATUS_SPECIFIER STATUS_VALUE
%token DESC_SPECIFIER 
%token MOUNT_POINT ASSIGNED
%token BEGIN_ END_ DEF SEQ
%token COMMA SEMICOLON INDEX_ AUGMENTS_ TRAP_SPECIFIER
%token OBJ_IDEN_ L_BRACE R_BRACE OBJECTS_
%token FROM_ IMPORTS_

%token L_PAREN R_PAREN

%token HEX_STRING
%token UNITS_SPECIFIER

%token DEFVAL_SPECIFIER
%token DISPLAY_HINT
%token REFERENCE
%token TC_SPECIFIER
%token COMPLIANCE_SPECIFIER
%token MIN_ACCESS_SPECIFIER
%token OBJECT_

%token NOTIFY_SPECIFIER
%token NOTIFY_GRP_SPECIFIER

%token MODULE_SPECIFIER
%token MANDATORY_SPECIFIER
%token GROUP_SPECIFIER
%token OBJECT_SPECIFIER
%token WRITE_SPECIFIER
%token OBJ_GRP_SPECIFIER

%token TO
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
 
%token <str> IDENTIFIER
%token <str> NUM
%token <str> TYPE_BUILT_IN
%token <str> ACCESS_VALUE
%token <str> STRING


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
    REFERENCE STRING | /* empty */ ;

/* OBJECT-GROUP */
OBJ_GRP_DEFINED :
    IDENTIFIER OBJ_GRP_SPECIFIER OBJ_GRP MOUNT {
        PARAM_FLUSH();
    };
OBJ_GRP :
    OBJ_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER STRING REF_PART;
OBJ_PART :
    OBJECTS_ L_BRACE OBJS R_BRACE
OBJS : 
    IDENTIFIER
    | IDENTIFIER COMMA OBJS;

/* NOTIFICATION-GROUP */
NOTIFY_GRP_DEFINED :
    IDENTIFIER NOTIFY_GRP_SPECIFIER NOTIFY_GRP MOUNT {
        PARAM_FLUSH();
    };
NOTIFY_GRP :
    NOTIFY_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER STRING REF_PART;
NOTIFY_PART :
    NOTIFY_SPECIFIER L_BRACE NOTIFICATIONS R_BRACE;
NOTIFICATIONS :
    IDENTIFIER
    | IDENTIFIER COMMA NOTIFICATIONS;

/* MODULE_COMPILANCE */
COMPLIANCE_DEFINED :
    IDENTIFIER COMPLIANCE_SPECIFIER COMPLIANCE_BODY MOUNT {
       PARAM_FLUSH(); 
    };
COMPLIANCE_BODY :
    STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER STRING REF_PART MODULE_PART;
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
    DESC_SPECIFIER STRING;
COMPLIANCE_OBJ : 
    OBJECT_ IDENTIFIER COMPLIANCE_SYNTAX COMPLIANCE_WRITE COMPLIANCE_ACCESS DESC_SPECIFIER STRING; 
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
    DisplayPart STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER STRING REF_PART SYNTAX_SPECIFIER SYNTAX_VALUE;
DisplayPart:
    DISPLAY_HINT STRING
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
            /* Go into second pass because the checking may fail by
               define after use */     
            disParamStore(pendingTypes, disParamConstruct($IDENTIFIER));
        }
    }

/* OBJECT-IDENTITY */
OBJ_IDENTITY_DEFINED :
    IDENTIFIER OBJ_IDENTITY_SPECIFIER OBJ_IDENTITY MOUNT {
        PARAM_FLUSH();
    };
OBJ_IDENTITY :
    STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER STRING REF_PART;  

/* NOTIFICATION-TYPE */
NOTIFY_TYPE_DEFINED : 
    IDENTIFIER NOTIFY_TYPE_SPECIFIER NOTIFY_TYPE MOUNT {
        PARAM_FLUSH();
    };
NOTIFY_TYPE : 
    NOTIFY_TYPE_OBJ_PART STATUS_SPECIFIER STATUS_VALUE DESC_SPECIFIER STRING REF_PART;
NOTIFY_TYPE_OBJ_PART :
    OBJ_PART
    | /* empty */;


TYPE_DEFINED :
    IDENTIFIER ASSIGNED TYPE {
        _Bool isExists = typeTableIsTypeExists(MIB_TYPE_TBL_R, $IDENTIFIER);
        if (!isExists) {
            typeTableAdd(MIB_TYPE_TBL_R, $IDENTIFIER, CATE_CUSTOM, NULL);         
        }
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
	IDENTIFIER TYPE TYPE_SPECIFIER COMMA SEQ_ITEM {
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
    IDENTIFIER MOD_SPECIFIER MODULE_BODY MOUNT {
        dispatchParam *param = disParamConstruct(SLICE_IDENTIFIER);
        disParamStore(param, disParamConstruct($IDENTIFIER));
        dispatch(DISPATCH_PARAM_STAGE, param);              
        
        param = disParamConstruct(OBJECT_IDENTIFIER);
        dispatch(MIBTREE_GENERATION, param);
    };

MODULE_BODY : 
    LAST_UPDATED STRING
    ORGANIZATION STRING 
    CONTACT_INFO STRING 
    DESC_SPECIFIER STRING 
    REVISIONS;


REVISIONS : 
    REVISION STRING DESC_SPECIFIER STRING REVISIONS
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
    UNITS
	ACCESS
	STATUS
	DESCRIPTION
    REF_PART
	INDEX
    DEFVAL
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
    | OBJ_IDEN_;

TYPE_SPECIFIER :
    TYPE_SPECIFIER_
    | /* empty */;
TYPE_SPECIFIER_ :
    L_BRACE ENUMERATE_MEMBERS R_BRACE
    | L_PAREN ONE_OR_MORE_VAL R_PAREN;

ENUMERATE_MEMBERS :
    ENUMERATE_MEMBER 
    | ENUMERATE_MEMBER COMMA ENUMERATE_MEMBERS; 
ENUMERATE_MEMBER:
    IDENTIFIER L_PAREN NUM R_PAREN;

ONE_OR_MORE_VAL :
    VAL
    | VAL OR ONE_OR_MORE_VAL ;
VAL :
    NUM
    | RANGE;
RANGE:
    NUM TO NUM

ACCESS :
	ACCESS_SPECIFIER ACCESS_VALUE {
		dispatchParam *param = disParamConstruct(SLICE_PERMISSION);
		disParamStore(param, disParamConstruct($ACCESS_VALUE));
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

STATUS :
	STATUS_SPECIFIER STATUS_VALUE;

DESCRIPTION :
	DESC_SPECIFIER STRING {
		dispatchParam *param = disParamConstruct(SLICE_DESCRIPTION);
		disParamStore(param, disParamConstruct($STRING));
		dispatch(DISPATCH_PARAM_STAGE, param);
	};

INDEX : 
    INDEX_ L_BRACE INDEX_ITEM R_BRACE 
    | AUGMENTS_ L_BRACE IDENTIFIER R_BRACE
    | /* empty */;

INDEX_ITEM :
	IDENTIFIER COMMA INDEX_ITEM
	| IDENTIFIER;

DEFVAL : DEFVAL_SPECIFIER L_BRACE DEFVAL_VAL R_BRACE
    | /* empty */;
DEFVAL_VAL :
    IDENTIFIER
    | NUM
    | HEX_STRING
    | STRING
    | L_BRACE BITS_VALUE R_BRACE;
BITS_VALUE :
    BIT_NAMES
    | /* empty */;
BIT_NAMES :
    IDENTIFIER
    | IDENTIFIER COMMA BIT_NAMES;  

UNITS : UNITS_SPECIFIER STRING
    | /* empty */;

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
    fprintf(stderr, "Error occur during parsing %s at line %d: %s\n", 
        SW_CUR_FILE_NAME(&swState), yylineno, yytext);
}    

