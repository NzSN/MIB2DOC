//
// Created by ayden on 2017/4/19.
//

#include <malloc.h>
#include <string.h>
#include "mibTreeGen.h"
#include "type.h"
#include "list.h"
#include "queue.h"

/* Declaration */
static int mibTreeLeaveAdd(char *ident, char *type,
        char *rw, char *desc,
        char *parent, char *suffix);
static int mibTreeNodeAdd(char *ident, char *oid, char *parent);
static char * oidComplement(char *parent, char *suffix);

char currentTable[64];
extern mibObjectTreeNode root;
extern elementList elistHead;

/* Global */
mibObjectTreeNode root;
symbolTable symTable;
elementList symCollectList;
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

    ident = getElement_el(&elistHead, IDENTIFIER_EL)->content;
    type = getElement_el(&elistHead, TYPE_EL)->content;
    rw = getElement_el(&elistHead, RW_EL)->content;
    desc = getElement_el(&elistHead, DESCRIPTION_EL)->content;
    parent = getElement_el(&elistHead, PARENT_EL)->content;
    suffix = getElement_el(&elistHead, SUFFIX_EL)->content;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(type) ||
            IS_PTR_NULL(rw) || IS_PTR_NULL(desc) ||
            IS_PTR_NULL(desc) || IS_PTR_NULL(parent) ||
            IS_PTR_NULL(suffix)) {
        return -1;
    }

    oid = oidComplement(parent, suffix);
    mibTreeLeaveAdd(ident, type, rw, desc, parent, oid);

    reset_el(&elistHead);
    RELEASE_PTR(suffix);
    RELEASE_PTR(parent);
    return 0;
}

int deal_with_objIdent() {
    char *ident, *parent, *suffix, *oid;

    ident = getElement_el(&elistHead, IDENTIFIER_EL)->content;
    parent = getElement_el(&elistHead, PARENT_EL)->content;
    suffix = getElement_el(&elistHead, SUFFIX_EL)->content;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(parent) || IS_PTR_NULL(suffix))
        return -1;

    oid = oidComplement(parent, suffix);
    mibTreeNodeAdd(ident, oid, parent);

    reset_el(&elistHead);
    RELEASE_PTR(suffix);
    RELEASE_PTR(parent);
    return 0;
}

int deal_with_trap() {
    char *ident, *parent, *suffix, *oid, *desc, *type;

    ident = getElement_el(&elistHead, IDENTIFIER_EL)->content;
    type = (char *)malloc(strlen("trap")+1);
    memset(type, 0, strlen("trap")+1);
    strncpy(type, "trap", strlen("trap"));
    parent = getElement_el(&elistHead, PARENT_EL)->content;
    suffix = getElement_el(&elistHead, SUFFIX_EL)->content;
    desc = getElement_el(&elistHead, DESCRIPTION_EL)->content;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(type) || IS_PTR_NULL(parent) || IS_PTR_NULL(suffix))
        return -1;

    oid = oidComplement(parent, suffix);
    mibTreeLeaveAdd(ident, type, NULL, NULL, parent, oid);

    reset_el(&elistHead);

    RELEASE_PTR(desc);
    RELEASE_PTR(suffix);
    RELEASE_PTR(parent);
    return 0;
}

void deal_with_sequence() {}

static int tablePrint(char *ident, char *oid, char *rw, char *detail, FILE *output) {
    if (IS_PTR_NULL(ident) || IS_PTR_NULL(oid) ||
        IS_PTR_NULL(rw) || IS_PTR_NULL(detail))
        return -1;
}



static int mibTreeLeaveAdd(char *ident, char *type, char *rw, char *desc, char *parent, char *oid) {
    mibObjectTreeNode *obj;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(type) || IS_PTR_NULL(parent) || IS_PTR_NULL(oid)) {
        return -1;
    }

    obj = mibLeaveBuild(ident, type, rw, desc, oid);
    if (IS_PTR_NULL(obj))
        return -1;

    insert_mot(&mibObjectTreeRoot, obj, parent);
}

static int mibTreeNodeAdd(char *ident, char *oid, char *parent) {
    mibObjectTreeNode *obj;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(oid))
        return -1;

    obj = mibNodeBuild(ident, oid);
    if (IS_PTR_NULL(obj))
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

static int symbolCollect_OBJECT(params_t *param);
static int symbolCollect_DESCRIPTION(params_t *param);
static int symbolCollect_IDENTIFIER(params_t *param);
static int symbolCollect_OBJECT_IDENTIFIER(params_t *param);
static int symbolCollect_PARENT(params_t *param);
static int symbolCollect_RW(params_t *param);
static int symbolCollect_SEQUENCE(params_t *param);
static int symbolCollect_SMI_DEF(params_t *param);
static int symbolCollect_SUFFIX(params_t *param);
static int symbolCollect_TRAP(params_t *param);
static int symbolCollect_TYPE(params_t *param);

int (*symbolCollectRoutine[NUM_OF_COLLECT_ROUTINE])(params_t *);

/* Initialize symbolCollectRoutine array */
int symbolCollectingInit() {
    /* Symbol Table building function */
    symbolCollectRoutine[OBJECT] = symbolCollect_OBJECT;
    symbolCollectRoutine[TRAP] = symbolCollect_TRAP;
    symbolCollectRoutine[OBJECT_IDENTIFIER] = symbolCollect_OBJECT_IDENTIFIER;
    symbolCollectRoutine[SEQUENCE] = symbolCollect_OBJECT_IDENTIFIER;
    symbolCollectRoutine[SMI_DEF] = symbolCollect_SMI_DEF;

    /* Parameter Collecting function */
    symbolCollectRoutine[IDENTIFIER_EL] = symbolCollect_IDENTIFIER;
    symbolCollectRoutine[TYPE_EL] = symbolCollect_TYPE;
    symbolCollectRoutine[RW_EL] = symbolCollect_RW;
    symbolCollectRoutine[DESCRIPTION_EL] = symbolCollect_DESCRIPTION;
    symbolCollectRoutine[PARENT_EL] = symbolCollect_PARENT;
    symbolCollectRoutine[SUFFIX_EL] = symbolCollect_SUFFIX;

    return 0;
}

int symbolCollecting(int type, params_t *param) {
    return symbolCollectRoutine[type](param);
}

static int symbolCollect_OBJECT(params_t *param) {
    flushAll_el(&symCollectList);
}


static int symbolCollect_TRAP(params_t *param) {
    flushAll_el(&symCollectList);
}

static int symbolCollect_OBJECT_IDENTIFIER(params_t *param) {
    symbolTable *newMod;
    symbol_t *newSymbol;
    char *modIdent;
    char *symbolIdent;
    char *parentIdent = getElement_el(&symCollectList, PARENT_EL)->content;

    symbolIdent = getElement_el(&symCollectList, IDENTIFIER_EL)->content;

    /* Is the symbol exists in symbol table ? */
    if (symbolSearching(symbolIdent)) {
        return 1;
    }

    modIdent = (char *)malloc(MAX_CHAR_OF_MOD_IDENT)
    switch_CurrentMod(modIdent, MAX_CHAR_OF_MOD_IDENT);
    parentIdent = getElement_el(&symCollectList, PARENT_EL);

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

    return 0;
}

static int symbolCollect_SEQUENCE(params_t *param) {
    flushAll_el(&symCollectList);
}

static int symbolCollect_SMI_DEF(params_t *param) {
    /* Record into symtable */
}

static int symbolCollect_IDENTIFIER(params_t *param) {

    if (IS_PTR_NULL(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(IDENTIFIER_EL, param);
    return 0;
}

static int symbolCollect_TYPE(params_t *param) {

    if (IS_PTR_NULL(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(TYPE_EL, param);
    return 0;
}

static int symbolCollect_RW(params_t *param) {
    if (IS_PTR_NULL(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(RW_EL, param);
    return 0;
}

static int symbolCollect_DESCRIPTION(params_t *param) {
    if (IS_PTR_NULL(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(DESCRIPTION_EL, param);
    return 0;
}

static int symbolCollect_PARENT(params_t *param) {
    if (IS_PTR_NULL(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(PARENT_EL, param);
    return 0;
}

static int symbolCollect_SUFFIX(params_t *param) {
    if (IS_PTR_NULL(param)) {
        return -1;
    }

    PARAM_STORE_TO_SYM_LIST(SUFFIX_EL, param);
    return 0;
}
