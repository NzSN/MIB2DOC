//
// Created by ayden on 2017/4/19.
//

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "mibTreeGen.h"
#include "type.h"
#include "list.h"
#include "queue.h"
#include "dispatcher.h"
#include "symbolTbl.h"

/* Declaration */
static int mibTreeLeaveAdd(mibObjectTreeNode *trap, char *parent);
static int mibTreeNodeAdd(mibObjectTreeNode *node, char *parent);

char currentTable[64];
extern mibObjectTreeNode root;
extern slice sliceContainer;
extern mibObjectTreeNode mibObjectTreeRoot;

/* Global */
mibObjectTreeNode root;
mibTreeHead trees;
symbolTable symTable;
slice symCollectSlice;
targetSymbolList tSymListHead;

// Type table
typeTable mibTypeTbl;

_Bool typeTableInit() {

    memset(&mibTypeTbl, 0, sizeof(typeTable));
    
    // Install SNMPv2 Types into type table.
    typeTableAdd(&mibTypeTbl, "Counter32", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "Counter64", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "Gauge32", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "Integer32", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "IpAddress", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "Opaque", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "TimeTicks", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "BITS", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "Unsigned32", CATE_BUILD_IN, NULL);
    typeTableAdd(&mibTypeTbl, "INTEGER", CATE_BUILD_IN, NULL);

    return OK;
}

int mibObjGen(int nodeType) {

    mibObjectTreeNode *newNode;
    char *ident, *type, *rw, *desc, *parent, *suffix;    

    ident = sliceRetriVal(&sliceContainer, SLICE_IDENTIFIER);;
    type = sliceRetriVal(&sliceContainer, SLICE_TYPE); 
    rw = sliceRetriVal(&sliceContainer, SLICE_PERMISSION);
    desc = sliceRetriVal(&sliceContainer, SLICE_DESCRIPTION);
    parent = sliceRetriVal(&sliceContainer, SLICE_PARENT); 
    suffix = sliceRetriVal(&sliceContainer, SLICE_OID_SUFFIX);
    
    // Node Build
    switch (nodeType) {
        case OBJECT:
            assert(ident != NULL);
            assert(type != NULL);
            assert(rw != NULL);
            assert(desc != NULL);
            assert(parent != NULL);
            assert(suffix != NULL);
            
            newNode = mibLeaveBuild(ident, type, rw, desc, suffix, parent);
            break;
        case TRAP:
            type = (char *)malloc(strlen("trap") + 1);
            strncpy(type, "trap", strlen("trap"));

            assert(ident != NULL); 
            assert(parent != NULL); 
            assert(suffix != NULL);
            assert(desc != NULL);
            
            newNode = mibLeaveBuild(ident, type, NULL, NULL, suffix, parent); 
            break;
        case OBJECT_IDENTIFIER:
            assert(ident != NULL);
            assert(parent != NULL);
            assert(suffix != NULL);
            
            newNode = mibNodeBuild(ident, suffix, parent);
            break;
        case SEQUENCE:
            /* ignore */
            break;
        default:
            break;
    }

    assert(mibTreeHeadAppend(&trees, newNode) != FALSE);
    
    sliceRelease_STATIC(&sliceContainer);

    return 0;
}

static char * oidComplement(char *parent, char *suffix) {
    char *oid, *parentOid;
    int oidLength = SIZE_OF_OID_STRING;
    mibObjectTreeNode *parentNode;

    parentNode = search_MibTree(&mibObjectTreeRoot, parent);

    if (parentNode == NULL)
        return NULL;
    
    parentOid = getOidFromInfo(parentNode);

    // Overflow checking.
    if (strlen(parentOid) >= SIZE_OF_OID_STRING) {
        oidLength += EXTRA_OF_OID_LEN; 
    }

    oid = (char *)malloc(oidLength);
    memset(oid, 0, oidLength);

    strncpy(oid, parentOid, oidLength);
    strncat(oid, ".", 1);
    strncat(oid, suffix, strlen(suffix));

    return oid;
}

int mibObjGen_Leave() {
    mibObjectTreeNode *obj;
    char *ident, *type, *rw, *desc, *parent, *suffix, *oid;
    
    assert((ident = sliceGetVal(&sliceContainer, SLICE_IDENTIFIER)) != NULL);
    assert((type = sliceGetVal(&sliceContainer, SLICE_TYPE)) != NULL);
    assert((rw = sliceGetVal(&sliceContainer, SLICE_PERMISSION)) != NULL);
    assert((desc = sliceGetVal(&sliceContainer, SLICE_DESCRIPTION)) != NULL);
    assert((parent = sliceGetVal(&sliceContainer, SLICE_PARENT)) != NULL);
    assert((suffix = sliceGetVal(&sliceContainer, SLICE_OID_SUFFIX)) != NULL);

    oid = oidComplement(parent, suffix);
    obj = mibLeaveBuild(ident, type, rw, desc, oid, parent);
    mibTreeLeaveAdd(obj, parent);

    sliceReset(sliceNext(&sliceContainer));
    RELEASE_MEM(suffix);
    RELEASE_MEM(parent);
    return 0;
}

int mibObjGen_InnerNode() {
    mibObjectTreeNode *node;
    char *ident, *parent, *suffix, *oid;

    assert( (ident = sliceGetVal(&sliceContainer, SLICE_IDENTIFIER)) != NULL);
    assert( (parent = sliceGetVal(&sliceContainer, SLICE_PARENT)) != NULL);
    assert( (suffix = sliceGetVal(&sliceContainer, SLICE_OID_SUFFIX)) != NULL);

    oid = oidComplement(parent, suffix);

    node = mibNodeBuild(ident, oid, parent);
    mibTreeNodeAdd(node, parent);

    sliceReset(sliceNext(&sliceContainer));

    RELEASE_MEM(suffix);
    RELEASE_MEM(parent);

    return 0;
}

int mibObjGen_trap() {
    mibObjectTreeNode *obj; 
    char *ident, *parent, *suffix, *oid, *desc, *type;

    assert( (ident = sliceGetVal(&sliceContainer, SLICE_IDENTIFIER)) != NULL);

    type = (char *)malloc(strlen("trap")+1);
    memset(type, 0, strlen("trap")+1);
    strncpy(type, "trap", strlen("trap"));

    assert( (parent = sliceGetVal(&sliceContainer, SLICE_PARENT)) != NULL);
    assert( (suffix = sliceGetVal(&sliceContainer, SLICE_OID_SUFFIX)) != NULL);
    assert( (desc = sliceGetVal(&sliceContainer, SLICE_DESCRIPTION)) != NULL);

    oid = oidComplement(parent, suffix);
    
    obj = mibLeaveBuild(ident, type, NULL, NULL, oid, parent); 
    mibTreeLeaveAdd(obj, parent);

    sliceReset(sliceNext(&sliceContainer));

    RELEASE_MEM(desc);
    RELEASE_MEM(suffix);
    RELEASE_MEM(parent);

    return 0;
}

void deal_with_sequence() {}

static int tablePrint(char *ident, char *oid, char *rw, char *detail, FILE *output) {
    if (isNullPtr(ident) || isNullPtr(oid) || isNullPtr(rw) || isNullPtr(detail))
        return -1;
}

static int mibTreeLeaveAdd(mibObjectTreeNode *obj, char *parent) {
    if (isNullPtr(obj))
        return -1;
    return insert_MibTree(&mibObjectTreeRoot, obj, parent);
}

static int mibTreeNodeAdd(mibObjectTreeNode *obj, char *parent) {
    if (isNullPtr(obj))
        return -1;
    return insert_MibTree(&mibObjectTreeRoot, obj, parent);
}

extern mibObjectTreeNode mibObjectTreeRoot;
int upperTreeGeneration(symbolTable *symTbl) {
#if 0
    mibObjectTreeNode *root;
    mibObjectTreeNode *current;
    mibObjectTreeNode *childNode;
    symbolTable *table;
    symbol_t *sym;
    symbol_t *child;
    symbol_t *temp;
    genericStack stack;

    if (isNullPtr(symTbl)) {
        return ERROR_NULL_REF;
    }
    root = &mibObjectTreeRoot;
    insert_MibTree(root, mibNodeBuild("iso", "1", NULL), "root");
    push(&stack, root);

    sym = (symbol_t *)malloc(sizeof(symbol_t));
    memset(sym, 0, sizeof(symbol_t));
    while (stack.top > 0) {
        pop(&stack, current);
        if (symbolSearchingByParent(&symTable, current->identifier, sym)) {
            child = containerOf(sym->symNode.next, symbol_t, symNode);
            while (child != NULL) {
                if (child->symType == SYMBOL_TYPE_NODE) {
                    childNode = mibNodeBuild(strdup(child->symIdent),
                        strdup(child->symInfo.nodeMeta.suffix), NULL);
                    insert_MibTree(root, childNode, strdup(current->identifier));
                    push(&stack, childNode);
                } else if (child->symType == SYMBOL_TYPE_LEAVE) {
                    childNode = mibLeaveBuild(strdup(child->symIdent),
                        strdup(child->symInfo.leaveMeta.type),
                        strdup(child->symInfo.leaveMeta.permission),
                        NULL,
                        strdup(child->symInfo.leaveMeta.suffix), NULL);
                    insert_MibTree(root, childNode, strdup(current->identifier));
                }
                temp = child;
                child = containerOf(&child->symNode, symbol_t, symNode);

                RELEASE_MEM(temp->symIdent);
                RELEASE_MEM(temp);
            }
        }
    }
#endif
}

/*
 * Symbol Collecting
 */
static int symbolCollect_BUILD_INNER_NODE(dispatchParam *param);
static int symbolCollect_PARAM_DESC(dispatchParam *param);
static int symbolCollect_PARAM_IDENT(dispatchParam *param);
static int symbolCollect_BUILD_LEAVE_NODE(dispatchParam *param);
static int symbolCollect_PARAM_PARENT(dispatchParam *param);
static int symbolCollect_PARAM_PERM(dispatchParam *param);
static int symbolCollect_BUILD_SEQUENCE(dispatchParam *param);
static int symbolCollect_BUILD_SMI_DEF(dispatchParam *param);
static int symbolCollect_PARAM_SUFFIX(dispatchParam *param);
static int symbolCollect_BUILD_TRAP(dispatchParam *param);
static int symbolCollect_PARAM_TYPE(dispatchParam *param);

int (*symbolCollectRoutine[SLICE_TYPE_MAXIMUM])(dispatchParam *);

/* Initialize symbolCollectRoutine array */
int symbolCollectingInit() {
    /* Symbol Table building function */
    symbolCollectRoutine[OBJECT_IDENTIFIER] = symbolCollect_BUILD_INNER_NODE;
    symbolCollectRoutine[TRAP] = symbolCollect_BUILD_TRAP;
    symbolCollectRoutine[OBJECT] = symbolCollect_BUILD_LEAVE_NODE;
    symbolCollectRoutine[SEQUENCE] = symbolCollect_BUILD_SEQUENCE;
    symbolCollectRoutine[SMI_DEF] = symbolCollect_BUILD_SMI_DEF;

    /* Parameter Collecting function */
    symbolCollectRoutine[SLICE_IDENTIFIER] = symbolCollect_PARAM_IDENT;
    symbolCollectRoutine[SLICE_TYPE] = symbolCollect_PARAM_TYPE;
    symbolCollectRoutine[SLICE_PERMISSION] = symbolCollect_PARAM_PERM;
    symbolCollectRoutine[SLICE_DESCRIPTION] = symbolCollect_PARAM_DESC;
    symbolCollectRoutine[SLICE_PARENT] = symbolCollect_PARAM_PARENT;
    symbolCollectRoutine[SLICE_OID_SUFFIX] = symbolCollect_PARAM_SUFFIX;

    return 0;
}

int symbolCollecting(int type, dispatchParam *param) {
    char *ident;

    if (type >= OBJECT && type <= SMI_DEF) {
        ident = sliceGetVal(&symCollectSlice, SLICE_IDENTIFIER);
        if (!collectInfo_exists(SW_CUR_IMPORT(&swState), ident)) {
            sliceRelease_STATIC(&symCollectSlice);
            return TRUE;
        }
    }   
    return symbolCollectRoutine[type](param);
}

static int symbolCollect_BUILD_INNER_NODE(dispatchParam *param) {
    int retVal;
    symbolTable *newMod;
    collectInfo *pCollect;
    symbol_t *newSymbol;
    identList *listHead, *listProcessing;

    char *symbolIdent, *parentIdent, *suffix;
    
    parentIdent = sliceRetriVal(&symCollectSlice, SLICE_PARENT);
    symbolIdent = sliceRetriVal(&symCollectSlice, SLICE_IDENTIFIER);
    suffix = sliceRetriVal(&symCollectSlice, SLICE_OID_SUFFIX); 

    // Is the symbol exists in symbol table ?
    if (symbolTableSearch(&symTable, symbolIdent)) {
        // Already exist only need to remove it from modStack
        RELEASE_MEM(parentIdent);
        RELEASE_MEM(symbolIdent);
        RELEASE_MEM(suffix);
        goto POST_CLEANING;
    }
    
    // Install the symbol into symbol table.
    newSymbol = symbolNodeConst(symbolIdent, parentIdent, suffix);
    symbolTableAdd(&symTable, newSymbol); 

POST_CLEANING:
    // Need to remove the symbol found from list in the modStack
    retVal = collectInfo_del(SW_CUR_IMPORT(&swState), symbolIdent);
    sliceRelease_STATIC(&symCollectSlice);
    return retVal;
}

static int symbolCollect_BUILD_TRAP(dispatchParam *param) {
    int retVal;
    char *symbolIdent = sliceGetVal(&symCollectSlice, SLICE_IDENTIFIER);

    retVal = collectInfo_del(SW_CUR_IMPORT(&swState), symbolIdent);
    sliceRelease_STATIC(&symCollectSlice);

    return retVal;
}

static int symbolCollect_BUILD_LEAVE_NODE(dispatchParam *param) {
    int retVal;
    char *symbolIdent = sliceGetVal(&symCollectSlice, SLICE_IDENTIFIER);
     
    retVal = collectInfo_del(SW_CUR_IMPORT(&swState), symbolIdent);
    sliceRelease_STATIC(&symCollectSlice);

    return retVal;
}

static int symbolCollect_BUILD_SEQUENCE(dispatchParam *param) {
    int retVal;
    char *symbolIdent = sliceGetVal(&symCollectSlice, SLICE_IDENTIFIER);

    retVal = collectInfo_del(SW_CUR_IMPORT(&swState), symbolIdent);
    sliceRelease_STATIC(&symCollectSlice);

    return retVal;
}

static int symbolCollect_BUILD_SMI_DEF(dispatchParam *param) {
    int retVal;
    char *symbolIdent = sliceGetVal(&symCollectSlice, SLICE_IDENTIFIER);

    retVal = collectInfo_del(SW_CUR_IMPORT(&swState), symbolIdent);
    sliceRelease_STATIC(&symCollectSlice);

    return retVal;
}

static int symbolCollect_PARAM_IDENT(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_IDENTIFIER, param);
    return 0;
}

static int symbolCollect_PARAM_TYPE(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_TYPE, param);
    return 0;
}

static int symbolCollect_PARAM_PERM(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_PERMISSION, param);
    return 0;
}

static int symbolCollect_PARAM_DESC(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_DESCRIPTION, param);
    return 0;
}

static int symbolCollect_PARAM_PARENT(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_PARENT, param);
    return 0;
}

static int symbolCollect_PARAM_SUFFIX(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_OID_SUFFIX, param);
    return 0;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void mibTreeGen__SYMBOL_COLLECT(void **state) {
    symbol_t *symbol; 
    
    symTableInit(); 

    symbolCollect_PARAM_IDENT(disParamConstruct(strdup("IDENT")));
    symbolCollect_PARAM_TYPE(disParamConstruct(strdup("TYPE"))); 
    symbolCollect_PARAM_PARENT(disParamConstruct(strdup("PARENT")));
    symbolCollect_PARAM_SUFFIX(disParamConstruct(strdup("SUFFIX")));
    symbolCollect_PARAM_DESC(disParamConstruct(strdup("DESC")));
    symbolCollect_PARAM_PERM(disParamConstruct(strdup("PERM")));

    symbolCollect_BUILD_INNER_NODE(NULL);
    
    symbol = symbolTableSearch(&symTable, "IDENT");
    assert_non_null(symbol); 
}

void mibTreeGenTesting(void **state) {
                   
}

#endif /* MIB2DOC_UNIT_TESTING */

