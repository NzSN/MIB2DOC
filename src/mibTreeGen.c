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
slice symCollectList;
targetSymbolList tSymListHead;

int deal_with(int type) {
    switch (type) {
        case OBJECT:
            deal_with_object();
            break;
        case TRAP:
            deal_with_trap();
            break;
        case OBJECT_IDENTIFIER:
            deal_with_objIdent();
            break;
        case SEQUENCE:
            /* ignore */
            break;
        default:
            break;
    }
    return 0;
}

int deal_with_object() {
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

int deal_with_objIdent() {
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

int deal_with_trap() {
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

    insert_mot(&mibObjectTreeRoot, obj, parent);
}

static int mibTreeNodeAdd(char *ident, char *oid, char *parent) {
    mibObjectTreeNode *obj;

    if (isNullPtr(ident) || isNullPtr(oid))
        return -1;

    obj = mibNodeBuild(ident, oid);
    if (isNullPtr(obj))
        return -1;

    insert_mot(&mibObjectTreeRoot, obj, parent);
}

static char * oidComplement(char *parent, char *suffix) {
    char *oid;
    mibObjectTreeNode *parentNode;

    oid = (char *)malloc(SIZE_OF_OID_STRING);
    memset(oid, 0, SIZE_OF_OID_STRING);
    parentNode = search_mot(&mibObjectTreeRoot, parent);

    if (parentNode == NULL)
        return NULL;


    strncpy(oid, getOidFromInfo(parentNode), SIZE_OF_OID_STRING);
    strncat(oid, ".", 1);
    strncat(oid, suffix, strlen(suffix));

    return oid;
}

/*
 * Symbol Collecting
 */

static int symbolCollect_OBJECT(dispatchParam *param);
static int symbolCollect_DESCRIPTION(dispatchParam *param);
static int symbolCollect_IDENTIFIER(dispatchParam *param);
static int symbolCollect_OBJECT_IDENTIFIER(dispatchParam *param);
static int symbolCollect_PARENT(dispatchParam *param);
static int symbolCollect_RW(dispatchParam *param);
static int symbolCollect_SEQUENCE(dispatchParam *param);
static int symbolCollect_SMI_DEF(dispatchParam *param);
static int symbolCollect_SUFFIX(dispatchParam *param);
static int symbolCollect_TRAP(dispatchParam *param);
static int symbolCollect_TYPE(dispatchParam *param);

int (*symbolCollectRoutine[SLICE_TYPE_MAXIMUM])(dispatchParam *);

/* Initialize symbolCollectRoutine array */
int symbolCollectingInit() {
    /* Symbol Table building function */
    symbolCollectRoutine[OBJECT] = symbolCollect_OBJECT;
    symbolCollectRoutine[TRAP] = symbolCollect_TRAP;
    symbolCollectRoutine[OBJECT_IDENTIFIER] = symbolCollect_OBJECT_IDENTIFIER;
    symbolCollectRoutine[SEQUENCE] = symbolCollect_OBJECT_IDENTIFIER;
    symbolCollectRoutine[SMI_DEF] = symbolCollect_SMI_DEF;

    /* Parameter Collecting function */
    symbolCollectRoutine[SLICE_IDENTIFIER] = symbolCollect_IDENTIFIER;
    symbolCollectRoutine[SLICE_TYPE] = symbolCollect_TYPE;
    symbolCollectRoutine[SLICE_PERMISSION] = symbolCollect_RW;
    symbolCollectRoutine[SLICE_DESCRIPTION] = symbolCollect_DESCRIPTION;
    symbolCollectRoutine[SLICE_PARENT] = symbolCollect_PARENT;
    symbolCollectRoutine[SLICE_OID_SUFFIX] = symbolCollect_SUFFIX;

    return 0;
}

int symbolCollecting(int type, dispatchParam *param) {
    return symbolCollectRoutine[type](param);
}

static int symbolCollect_OBJECT(dispatchParam *param) {
    sliceRelease(&symCollectList);
}


static int symbolCollect_TRAP(dispatchParam *param) {
    sliceRelease(&symCollectList);
}

static int symbolCollect_OBJECT_IDENTIFIER(dispatchParam *param) {
    symbolTable *newMod;
    symbol_t *newSymbol;
    char *modIdent;
    char *symbolIdent;
    char *parentIdent = sliceGet(&symCollectList, SLICE_PARENT)->sliVal;

    symbolIdent = sliceGet(&symCollectList, SLICE_IDENTIFIER)->sliVal;

    /* Is that

    /* Is the symbol exists in symbol table ? */
    if (symbolSearching(symbolIdent)) {
        return 1;
    }

    modIdent = (char *)malloc(MAX_CHAR_OF_MOD_IDENT);
    switch_CurrentMod(modIdent, MAX_CHAR_OF_MOD_IDENT);
    parentIdent = sliceGet(&symCollectList, SLICE_PARENT)->sliVal;

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

    /* Need to remove the symbol found from list in thmodStack */

    return 0;
}

static int symbolCollect_SEQUENCE(dispatchParam *param) {
    sliceRelease(&symCollectList);
}

static int symbolCollect_SMI_DEF(dispatchParam *param) {
    /* Record into symtable */
}

static int symbolCollect_IDENTIFIER(dispatchParam *param) {

    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_IDENTIFIER, param);
    return 0;
}

static int symbolCollect_TYPE(dispatchParam *param) {

    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_TYPE, param);
    return 0;
}

static int symbolCollect_RW(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_PERMISSION, param);
    return 0;
}

static int symbolCollect_DESCRIPTION(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_DESCRIPTION, param);
    return 0;
}

static int symbolCollect_PARENT(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_PARENT, param);
    return 0;
}

static int symbolCollect_SUFFIX(dispatchParam *param) {
    if (isNullPtr(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SLICE_OID_SUFFIX, param);
    return 0;
}
