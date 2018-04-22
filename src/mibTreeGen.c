//
// Created by ayden on 2017/4/19.
//

#include <malloc.h>
#include <string.h>
#include "mibTreeGen.h"
#include "type.h"
#include "list.h"
#include "queue.h"
#include "dispatcher.h"

/* Declaration */
static int mibTreeLeaveAdd(char *ident, char *type,
        char *rw, char *desc,
        char *parent, char *suffix);
static int mibTreeNodeAdd(char *ident, char *oid, char *parent);
static char * oidComplement(char *parent, char *suffix);

char currentTable[64];
extern mibObjectTreeNode root;
extern slice sliceContainer;
extern mibObjectTreeNode mibObjectTreeRoot;

/* Global */
mibObjectTreeNode root;
symbolTable symTable;
slice symCollectSlice;
targetSymbolList tSymListHead;

int mibObjGen(int type) {
    switch (type) {
        case OBJECT:
            mibObjGen_Leave();
            break;
        case TRAP:
            mibObjGen_trap();
            break;
        case OBJECT_IDENTIFIER:
            mibObjGen_InnerNode();
            break;
        case SEQUENCE:
            /* ignore */
            break;
        default:
            break;
    }
    return 0;
}

int mibObjGen_Leave() {
    char *ident, *type, *rw, *desc, *parent, *suffix, *oid;

    ident = sliceGet(&sliceContainer, SLICE_IDENTIFIER)->sliVal;
    type = sliceGet(&sliceContainer, SLICE_TYPE)->sliVal;
    rw = sliceGet(&sliceContainer, SLICE_PERMISSION)->sliVal;
    desc = sliceGet(&sliceContainer, SLICE_DESCRIPTION)->sliVal;
    parent = sliceGet(&sliceContainer, SLICE_PARENT)->sliVal;
    suffix = sliceGet(&sliceContainer, SLICE_OID_SUFFIX)->sliVal;

    if (isNullPtr(ident) || isNullPtr(type) || isNullPtr(rw)
        || isNullPtr(desc) || isNullPtr(desc) || isNullPtr(parent)
        || isNullPtr(suffix)) {
        return -1;
    }

    oid = oidComplement(parent, suffix);
    mibTreeLeaveAdd(ident, type, rw, desc, parent, oid);

    sliceReset(&sliceContainer);
    RELEASE_MEM(suffix);
    RELEASE_MEM(parent);
    return 0;
}

int mibObjGen_InnerNode() {
    char *ident, *parent, *suffix, *oid;

    ident = sliceGet(&sliceContainer, SLICE_IDENTIFIER)->sliVal;
    parent = sliceGet(&sliceContainer, SLICE_PARENT)->sliVal;
    suffix = sliceGet(&sliceContainer, SLICE_OID_SUFFIX)->sliVal;

    if (isNullPtr(ident) || isNullPtr(parent) || isNullPtr(suffix))
        return -1;

    oid = oidComplement(parent, suffix);
    mibTreeNodeAdd(ident, oid, parent);

    sliceReset(&sliceContainer);
    RELEASE_MEM(suffix);
    RELEASE_MEM(parent);
    return 0;
}

int mibObjGen_trap() {
    char *ident, *parent, *suffix, *oid, *desc, *type;

    ident = sliceGet(&sliceContainer, SLICE_IDENTIFIER)->sliVal;
    type = (char *)malloc(strlen("trap")+1);
    memset(type, 0, strlen("trap")+1);
    strncpy(type, "trap", strlen("trap"));
    parent = sliceGet(&sliceContainer, SLICE_PARENT)->sliVal;
    suffix = sliceGet(&sliceContainer, SLICE_OID_SUFFIX)->sliVal;
    desc = sliceGet(&sliceContainer, SLICE_DESCRIPTION)->sliVal;

    if (isNullPtr(ident) || isNullPtr(type) || isNullPtr(parent) || isNullPtr(suffix))
        return -1;

    oid = oidComplement(parent, suffix);
    mibTreeLeaveAdd(ident, type, NULL, NULL, parent, oid);

    sliceReset(&sliceContainer);

    RELEASE_MEM(desc);
    RELEASE_MEM(suffix);
    RELEASE_MEM(parent);
    return 0;
}

void deal_with_sequence() {}

static int tablePrint(char *ident, char *oid, char *rw, char *detail, FILE *output) {
    if (isNullPtr(ident) || isNullPtr(oid) ||
        isNullPtr(rw) || isNullPtr(detail))
        return -1;
}



static int mibTreeLeaveAdd(char *ident, char *type,
    char *rw, char *desc,
    char *parent, char *oid) {

    mibObjectTreeNode *obj;

    if (isNullPtr(ident)
        || isNullPtr(type)
        || isNullPtr(parent)
        || isNullPtr(oid)) {
        return -1;
    }

    obj = mibLeaveBuild(ident, type, rw, desc, oid);
    if (isNullPtr(obj))
        return -1;

    insert_MibTree(&mibObjectTreeRoot, obj, parent);
}

static int mibTreeNodeAdd(char *ident, char *oid, char *parent) {
    mibObjectTreeNode *obj;

    if (isNullPtr(ident) || isNullPtr(oid))
        return -1;

    obj = mibNodeBuild(ident, oid);
    if (isNullPtr(obj))
        return -1;

    insert_MibTree(&mibObjectTreeRoot, obj, parent);
}

static char * oidComplement(char *parent, char *suffix) {
    char *oid;
    mibObjectTreeNode *parentNode;

    oid = (char *)malloc(SIZE_OF_OID_STRING);
    memset(oid, 0, SIZE_OF_OID_STRING);
    parentNode = search_MibTree(&mibObjectTreeRoot, parent);

    if (parentNode == NULL)
        return NULL;

    strncpy(oid, getOidFromInfo(parentNode), SIZE_OF_OID_STRING);
    strncat(oid, ".", 1);
    strncat(oid, suffix, strlen(suffix));

    return oid;
}

extern mibObjectTreeNode mibObjectTreeRoot;
int upperTreeGeneration(symbolTable *symTbl) {
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
    insert_MibTree(root, mibNodeBuild("iso", "1"), "root");
    push(&stack, root);

    sym = (symbol_t *)malloc(symbol_t);
    memset(sym, NULL, sizeof(symbol_t));
    while (stack.top > 0) {
        current = (mibObjectTreeNode *)pop(stack);
        if (symbolSearchingByParent(&symTable, current->identifier, sym)) {
            child = containerOf(sym->node.next, symbol_t, symNode);
            while (child != NULL) {
                if (child->symType == SYMBOL_TYPE_NODE) {
                    childNode = mibNodeBuild(strdup(child->symIdent),
                        strdup(child->symInfo.nodeMeta.suffix));
                    insert_MibTree(root, childNode, strdup(current->identifier));
                    push(&stack, childNode);
                } else if (child->symType == SYMBOL_TYPE_LEAVE) {
                    childNode = mibLeaveBuild(strdup(child->symIdent),
                                    strdup(child->symInfo.leaveMeta.type),
                                    strdup(child->symInfo.leaveMeta.permission),
                                    NULL,
                                    strdup(child->symInfo.leaveMeta.suffix));
                    insert_MibTree(root, childNode, strdup(current->identifier));
                }
                temp = child;
                child = containerOf(child->node.next, symbol_t, symNode);

                RELEASE_MEM(temp->symIdent);
                RELEASE_MEM(temp);
            }
        }
    }
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
    symbolCollectRoutine[OBJECT] = symbolCollect_BUILD_INNER_NODE;
    symbolCollectRoutine[TRAP] = symbolCollect_BUILD_TRAP;
    symbolCollectRoutine[OBJECT_IDENTIFIER] = symbolCollect_BUILD_LEAVE_NODE;
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
    // fixme: before collecting a symbol you should 
    // check to see that whether the symbol you encounter
    // is the one you want to include
    return symbolCollectRoutine[type](param);
}

static int symbolCollect_BUILD_INNER_NODE(dispatchParam *param) {
    int retVal;
    symbolTable *newMod;
    symbol_t *newSymbol;
    identList *listHead, *listProcessing;
    char *modIdent;
    char *symbolIdent;
    char *parentIdent = sliceGet(&symCollectSlice, SLICE_PARENT)->sliVal;

    symbolIdent = sliceGet(&symCollectSlice, SLICE_IDENTIFIER)->sliVal;

    /* Is the symbol exists in symbol table ? */
    if (symbolSearching(symbolIdent)) {
        /* Already exist only need to remove it from modStack */
        goto MOD_STACK_OP_REMOVE;
    }

    modIdent = (char *)malloc(MAX_CHAR_OF_MOD_IDENT);
    switch_CurrentMod(modIdent, MAX_CHAR_OF_MOD_IDENT);
    parentIdent = sliceGet(&symCollectSlice, SLICE_PARENT)->sliVal;

    /* Is the module specify by modIdent is exists ? */
    if (!symbolModSearching(modIdent)) {
        newMod = (symbolTable *)malloc(sizeof(symbolTable));
        newMod->modName = modIdent;
        symbolModuleAdd(&symTable, newMod);
    }

    newSymbol = (symbol_t *)malloc(sizeof(symbol_t));
    newSymbol->identifier = symbolIdent;
    newSymbol->type = SYMBOL_TYPE_NODE;
    newSymbol->metadata.nodeMeta.parentIdent = parentIdent;
    symbolAdd(modIdent, newSymbol);

MOD_STACK_OP_REMOVE:
    /* Need to remove the symbol found from list in the modStack */
    listHead = (identList *)swState.modStack[swState.importStackIndex].symbols;
    retVal = rmSymFromIdentList(listHead, symbolIdent);
    return retVal;
}


static int symbolCollect_BUILD_TRAP(dispatchParam *param) {
    sliceRelease(&symCollectSlice);
    retVal = rmSymFromIdentList(swState.modStack[swState.importStackIndex].symbols, symbolIdent);
    return retVal;
}

static int symbolCollect_BUILD_LEAVE_NODE(dispatchParam *param) {
    retVal = rmSymFromIdentList(swState.modStack[swState.importStackIndex].symbols, symbolIdent);
    return retVal;
}

static int symbolCollect_BUILD_SEQUENCE(dispatchParam *param) {
    sliceRelease(&symCollectSlice);
    retVal = rmSymFromIdentList(swState.modStack[swState.importStackIndex].symbols, symbolIdent);
    return retVal;
}

static int symbolCollect_BUILD_SMI_DEF(dispatchParam *param) {
    /* Record into symtable */
    sliceRelease(&symCollectSlice);
    retVal = rmSymFromIdentList(swState.modStack[swState.importStackIndex].symbols, symbolIdent);
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
