/* typeTable.c */

#include "util.h"
#include "options.h"
#include "typeTable.h"

// typeItem
typeItem * typeItemConst();
typeItem * typeItemCopy(typeItem *item);
int typeItemDestruct(typeItem *item);
int typeItemAssignment(typeItem *lval, const typeItem *rval);
int typeItemIsEqual(const typeItem *first, const typeItem *sec);

typeItem * typeItemSearch(const typeItem *items, const char *typeName);
int typeItemAppend(typeItem *items, char *typeName, typeCate cate, const void *ref);
int typeItemDel(typeItem *items, char *typeName);

typeItem * typeItemPrev(const typeItem *item);
typeItem * typeItemNext(const typeItem *item);
typeItem * typeItemTail(const typeItem *item);

typeItem * typeItemConst() {
    typeItem *item = (typeItem *)Malloc(sizeof(typeItem));

    memset(item, 0, sizeof(typeItem));

    return item;
}

// Note: ref will never be released and its
// not editable so Shallow copy is not a problem.
typeItem * typeItemCopy(typeItem *item) {
    if (isNullPtr(item)) return NULL;

    typeItem *item_copy = typeItemConst();
    item_copy->cate = item->cate;
    item_copy->type = strdup(item->type);
    item_copy->ref = item->ref;

    return item_copy;
}

int typeItemDestruct(typeItem *item) {
    if (isNullPtr(item)) return ERROR;

    RELEASE_IF_NON_NULL(item->type);

    RELEASE_MEM(item);

    return OK;
}

int typeItemAssignment(typeItem *lval, const typeItem *rval) {
    if (isNullPtr(lval) || isNullPtr(rval))
        return ERROR;

    RELEASE_IF_NON_NULL(lval->type);

    lval->cate = rval->cate;
    lval->type = strdup(rval->type);
    lval->ref = rval->ref;

    return OK;
}

static _Bool itemCmp(const listNode *node, const char *typeName) {
    typeItem *item = containerOf(node, typeItem, node);
    return isStringEqual(item->type, typeName);
}

typeItem * typeItemSearch(const typeItem *items, const char *typeName) {
    if (isNullPtr(items) || isNullPtr(typeName))
        return NULL;

    listNode *found = listNodeSearch(listNodeNext(&items->node), (listNodeCmp)itemCmp, (void *)typeName);
    if (isNullPtr(found)) return NULL;

    return containerOf(found, typeItem, node);
}

int typeItemIsEqual(const typeItem *first, const typeItem *sec) {
    if (isNullPtr(first) || isNullPtr(sec))
        return ERROR;

    return isStringEqual(first->type, sec->type);
}

/* Should check that is a type already be added to type table
 *
 * fixme: This function's time complexity is O^2, should be optimized */
int typeItemAppend(typeItem *items, char *typeName, typeCate cate, const void *ref) {
    if (isNullPtr(items) || isNullPtr(typeName))
        return ERROR;

    // The type is already exist
    if (typeItemSearch(items, typeName))
        return OK;
    else {
        if (optMngIsOptSet(optionsManager, OPT_KEY_SHOW_T_DEF))
            printf("ADD %s at %s : %d\n", typeName, SW_CUR_FILE_NAME(&swState), yylineno);
    }

    typeItem *lastItem = typeItemTail(items);
    typeItem *newItem = typeItemConst();

    newItem->type = typeName;
    newItem->ref = (void *)ref;
    newItem->cate = cate;

    if (listNodeInsert(&lastItem->node, &newItem->node) == NULL)
        return ERROR;

    return OK;
}

int typeItemDel(typeItem *items, char *typeName) {
    if (isNullPtr(items) || isNullPtr(typeName))
        return ERROR;

    typeItem *found;

    found = typeItemSearch(items, typeName);
    if (found) {
        listNodeDelete(&found->node);
        typeItemDestruct(found);
    }

    return OK;
}

typeItem * typeItemTail(const typeItem *item) {
    if (isNullPtr(item))
        return NULL;

    return containerOf(listNodeTail(&item->node), typeItem, node);
}

typeItem * typeItemPrev(const typeItem *item) {
    if (isNullPtr(item) || isNullPtr(item->node.prev))
        return NULL;

    return containerOf(listNodePrev(&item->node), typeItem, node);
}

typeItem * typeItemNext(const typeItem *item) {
    if (isNullPtr(item) || isNullPtr(item->node.next))
        return NULL;

    return containerOf(listNodeNext(&item->node), typeItem, node);
}
typeTable * typeTableConst() {
    typeTable *tbl = (typeTable *)Malloc(sizeof(typeTable));

    memset(tbl, 0, sizeof(typeTable));

    return tbl;
}

typeTable * typeTableCopy(const typeTable *tbl) {
    return (typeTable *)tbl;
}

int typeTableDestruct(typeTable *tbl) {
    if (isNullPtr(tbl)) return ERROR;

    typeItem *itemCurrent = typeItemNext(&tbl->items);
    typeItem *itemNext = typeItemNext(itemCurrent);

    while (isNonNullPtr(itemCurrent)) {
        typeItemDestruct(itemCurrent);

        itemCurrent = itemNext;
        itemNext = typeItemNext(itemNext);
    }

    return OK;
}

int typeTableAssignment(typeTable *lval, const typeTable *rval) {
    return 0;
}

static _Bool typeTableEqualChecke(const listNode *lNode, const listNode *rNode) {
    typeItem *lItem = containerOf(lNode, typeItem, node);
    typeItem *rItem = containerOf(rNode, typeItem, node);

    if (isStringEqual(lItem->type, rItem->type))
        return TRUE;
    else
        return FALSE;
}

int typeTableIsEqual(const typeTable *firstTbl, const typeTable *secTbl) {
    if (isNullPtr(firstTbl) || isNullPtr(secTbl))
        return ERROR;

    if (firstTbl->numOfTypes != secTbl->numOfTypes)
        return FALSE;

    return listNodeIsEqual(listNodeNext(&firstTbl->items.node),
                           listNodeNext(&secTbl->items.node),
                           typeTableEqualChecke);
}

int typeTableIsTypeExists(const typeTable *tbl, const char *type) {
    if (isNullPtr(tbl) || isNullPtr(type))
        return ERROR;

    typeItem *found = typeItemSearch(&tbl->items, type);

    return isNonNullPtr(found) ? TRUE : FALSE;
}

typeCate typeTableTypeCate(const typeTable *tbl, const char *type) {
    if (isNullPtr(tbl) || isNullPtr(type))
        return ERROR;

    typeItem *found = typeItemSearch(&tbl->items, type);
    if (!found) return CATE_ERROR;

    return found->cate;
}

int typeTableAdd(typeTable *tbl, char *type, typeCate cate, const void *ref) {
    if (isNullPtr(tbl) || isNullPtr(type))
        return ERROR;

    if (typeItemAppend(&tbl->items, type, cate, ref) == ERROR)
        return ERROR;

    assert(++tbl->numOfTypes >= 0);

    return OK;
}

int typeTableDel(typeTable *tbl, char *type) {
    if (isNullPtr(tbl) || isNullPtr(type))
        return ERROR;

    if (typeItemDel(&tbl->items, type) == ERROR)
        return ERROR;


    assert(tbl->numOfTypes-- >= 0);

    return OK;
}

static _Bool isItemEqual(const listNode *node, const void *arg) {
    typeItem *item = containerOf(node, typeItem, node);

    return isStringEqual(TYPE_ITEM_TYPE(item), (char *)arg);
}
typeItem * typeTableSearch(typeTable *tbl, const char *typeName) {
    if (tbl->numOfTypes < 1)
        return NULL;

    listNode *node = listNodeSearch(&tbl->items.node, isItemEqual, typeName);
    return containerOf(node, typeItem, node);
}

static _Status itemRelBuild(listNode *head, void *arg) {
    typeTable *tbl = (typeTable *)arg;
    typeItem *item = containerOf(head, typeItem, node);

    typeItem *defItem = typeTableSearch(tbl, TYPE_ITEM_DEF_VAL(item));

    RELEASE_MEM(item->typeDef);

    item->ref = defItem;

    return OK;
}
int typeTableRelBuild(typeTable *tbl) {
    if (tbl->numOfTypes < 2)
        return 0;

    if (listNodeTravel(&tbl->items.node, itemRelBuild, tbl) == FALSE)
        return -1;

    return 0;
}
