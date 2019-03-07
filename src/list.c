//
// Created by ayden on 17-5-1.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "list.h"
#include "type.h"
#include "dispatcher.h"
#include "symbolTbl.h"
#include "typeTable.h"

enum {
    SYM_TRAVEL_CONTINUE = 10,
    SYM_TRAVEL_STOP = 11
};

/************
 * ListNode *
 ************/
listNode *listNodeNext(const listNode *sliNode) {
    if (isNullPtr(sliNode) || isNullPtr(sliNode->next))
        return NULL;  
    return sliNode->next;
}

listNode * listNodePrev(const listNode *sliNode) {
    if (isNullPtr(sliNode) || isNullPtr(sliNode->prev))
        return NULL;
    return sliNode->prev;
}

listNode * listNodeInsert(listNode *head, listNode *sliNode) {
    listNode *middle;

    if (isNullPtr(head) || isNullPtr(sliNode)) {
        return NULL;
    }
    if (head->next == NULL) {
        head->next = sliNode;
        sliNode->prev = head;
    } else {
        middle = head->next;
        head->next = sliNode;
        sliNode->next = middle;
    }
    return sliNode;
}

listNode * listNodeDelete(listNode *sliNode) {
    if (isNullPtr(sliNode)) {
        return NULL;
    }

    if (sliNode->prev != NULL && sliNode->next != NULL) {
        /* Middle */
        sliNode->prev->next = sliNode->next;
        sliNode->next->prev = sliNode->prev;
    } else if(sliNode->prev != NULL) {
        /* Tail */
        sliNode->prev->next = NULL;
    } else if (sliNode->next != NULL) {
        /* Head */
        sliNode->next->prev = NULL;
    } else {
        /* Single sliNode */
    }
    
    sliNode->next = NULL;
    sliNode->prev = NULL;

    return sliNode;
}

listNode * listNodeTail(const listNode *head) {
    if (isNullPtr(head)) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return (listNode *)head;
}

listNode * listNodeAppend(listNode *listH, listNode *listN) {
    if (isNullPtr(listH) || isNullPtr(listN)) {
        return NULL; 
    }
    listH = listNodeTail(listH);
    listNodeInsert(listH, listN);
    return listN;
}

bool listNodeIsEmpty(listNode *head) {
    int i = 0;
    
    if (isNullPtr(head)) {
        return FALSE;
    }
    if (head->next != NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

listNode * listNodeSearch(listNode *head, listNodeCmp cmpOp, void *arg) {
    if (isNullPtr(head) || isNullPtr(cmpOp) || isNullPtr(arg))
        return NULL;
    
    listNode *currentNode = head;

    while (isNonNullPtr(currentNode)) {
        if (cmpOp(currentNode, arg) == TRUE) 
            break; 
        currentNode = listNodeNext(currentNode);
    }
    
    return currentNode;
}

_Bool listNodeIsEqual(const listNode *first, const listNode *second, listNodeEqualCheck equalCheck) {
    if (isNullPtr(first) || isNullPtr(second) || isNullPtr(equalCheck))
        return ERROR;

    _Bool isEqual;
    const listNode *current_first = first;
    const listNode *current_second = second;

    while (isNonNullPtr(current_first)) {
        isEqual = equalCheck(current_first, current_second); 
        if (!isEqual) return FALSE;
        
        current_first = listNodeNext(current_first);
        current_second = listNodeNext(current_second);
    }

    return TRUE;
}

_Status listNodeTravel(listNode *head, listNodeTask func, void *arg) {
    if (isNullPtr(head) || isNullPtr(func) || isNullPtr(arg)) {
        return ERROR_NULL_REF;
    }
    while (head != NULL) {
        func(head, arg);
        head = listNodeNext(head);
    }
    return ERROR_NONE;
}

/*******************************************
 *  Element list Operation function define *
 *******************************************/
slice * sliceConstruct(int sliKey, char *sliVal) {
    slice *sli = (slice *)Malloc(sizeof(slice));

    sli->sliKey = sliKey;
    sli->sliVal = sliVal;
    sli->sliNode.next = NULL;
    sli->sliNode.prev = NULL;
    return sli;
}

bool sliceDestruct(slice *sli) {
    if (isNullPtr(sli)) {
        return FALSE;
    }
    if (!isNullPtr(sli->sliVal)) {
        RELEASE_MEM(sli->sliVal);
    }
    RELEASE_MEM(sli);
    return TRUE;
}

slice * slicePrev(slice *sli) {
    if (!isNullPtr(sli) && isNullPtr(sli->sliNode.prev)) {
        return NULL;
    }
    return containerOf(sli->sliNode.prev, slice, sliNode);
}

slice * sliceNext(slice *sli) {
    if (!isNullPtr(sli) && isNullPtr(sli->sliNode.next)) {
        return NULL;
    }
    return containerOf(sli->sliNode.next, slice, sliNode);
}

slice * sliceGet(slice *sliHead, int sliKey) {
    if (isNullPtr(sliHead)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }

    for (; sliHead != NULL; sliHead = sliceNext(sliHead)) {
        if (sliHead->sliKey == sliKey)
            return sliHead;
    }
    return NULL;
}

char * sliceGetVal(slice *sliHead, int sliKey) {
    slice *sli;

    if (sli = sliceGet(sliHead, sliKey))
        return sli->sliVal;
    return NULL;
}

char *sliceRetriVal(slice *sliHead, int sliKey) {
    slice *sli;
    char *val;
    
    if (sli = sliceGet(sliHead, sliKey)) {
        val = sli->sliVal; 
        sli->sliVal = NULL;
    }
    return val; 
}

int sliceStore(slice *sliHead, slice *newSli) {
    if (isNullPtr(sliHead) || isNullPtr(newSli)) {
        return ERROR_NULL_REF;
    } 

    if (sliceGet(sliHead, newSli->sliKey)) {
        return ERROR_NONE;
    }
    
    while (sliHead != NULL) {
        if (sliceNext(sliHead) == NULL) {
            sliHead->sliNode.next = &newSli->sliNode;
            return ERROR_NONE;
        }
        sliHead = sliceNext(sliHead);
    }
    return TRUE;
}

bool sliceRelease(slice *sli) {
    slice *pSli;
    slice *pSli_next;

    if (isNullPtr(sli)) 
        return FALSE;
    
    for (pSli = sli; pSli != NULL; pSli = pSli_next) {
        pSli_next = sliceNext(pSli);
        sliceDestruct(pSli);
    }
    return TRUE;
}

bool sliceRelease_STATIC(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli)) 
        return FALSE;

    sliceRelease(sliceNext(sli));

    sli->sliNode.next = NULL;
    sli->sliNode.prev = NULL;

    return TRUE;
}

bool sliceReset(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli)) {
        return FALSE;
    }
    while (sli != NULL) {
        pSli = sli;
        sli = sliceNext(sli);
        RELEASE_MEM(pSli); 
    }
    return TRUE;
}

bool sliceReset_STATIC(slice *sli) {
    slice *pSli;
    
    if (isNullPtr(sli))
        return FALSE;

    if (sliceReset(sliceNext(sli))) {
        memset(&sli->sliNode, 0, sizeof(listNode));     
    }
    return TRUE;
}

/*******************************************
 * dispatchParam Operation function define *
 *******************************************/

 /*
  * append sliNode to tail of list
  */
dispatchParam * disParamConstruct(void *param) {
    dispatchParam *ret;
     
    if (isNullPtr(param)) {
        return NULL;
    }
     
    ret  = (dispatchParam *)Malloc(sizeof(dispatchParam));
    memset(ret, 0, sizeof(dispatchParam));
    ret->param = param;
    
    return ret;
}

dispatchParam * dispatchParamPrev(dispatchParam *disparam) {
    listNode *prev = disparam->node.prev;
    if (isNullPtr(prev) || isNullPtr(disparam))
        return NULL;
    return containerOf(prev, dispatchParam, node);
}

dispatchParam * dispatchParamNext(dispatchParam *disparam) {
    if (isNullPtr(disparam) || isNullPtr(disparam->node.next))
        return NULL;
    return containerOf(disparam->node.next, dispatchParam, node);
}

dispatchParam * disParamStore(dispatchParam *head, dispatchParam *new) {

    if (head == NULL || new == NULL) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }

    listNodeInsert(listNodeTail(&head->node), &new->node);
    return new;
}

void * disParamGet(dispatchParam *disparam) {
    if (disparam == NULL) 
        return NULL;
    return disparam->param;
}

// fixme: should provide a version of release function that
//        allow customer to provide destruction function for
//        <dispatchParam->param>
int disParamRelease(dispatchParam *disParam, int (*destruct)(void *)) {
    dispatchParam *current, *released;
   
    if (isNullPtr(disParam))
        return FALSE;

    released = disParam;
    current = dispatchParamNext(disParam);
    
    do {
        if (!isNullPtr(destruct))
            destruct(released->param);
        RELEASE_MEM(released);
        released = current;
    } while (current = dispatchParamNext(current));

    return TRUE;
}

int disParamRelease_Static(dispatchParam *disParam, int (*destruct)(void *)) {
    int ret;

    if (isNullPtr(disParam))
        return FALSE;
    
    ret = disParamRelease(dispatchParamNext(disParam), destruct);
    memset(disParam, 0, sizeof(dispatchParam));

    return ret; 
}

/*
 * Get sliNode from the head from the list
 * after that the head will be
 * removed from list.
 */
dispatchParam * disParamRetrive(dispatchParam **head) {
    dispatchParam *ret;

    if (!isNullPtr(head) && isNullPtr(*head)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }

    ret = *head;
    *head = dispatchParamNext(*head);

    return ret;
}

/* symbol_t and symbolTable operation function define */
symErrorCode symTblError;
extern symbolTable symTable;

typedef struct {
    pair_key_base base;
    char *ident;
} symbol_k;

static symbol_k * symbolKInit(symbol_k *key, char *name);

static symbol_k * symbolKCopy(symbol_k *key) {
    symbol_k *copy;

    if (isNullPtr(key))
        return NULL;

    copy = (symbol_k *)Malloc(sizeof(symbol_k));
    return symbolKInit(copy, strdup(key->ident));
}

static int symbolKRelease(symbol_k *key) {
    symbol_k *pKey = key;
 
    if (isNullPtr(key))
        return FALSE;          
    
    RELEASE_MEM(pKey->ident);
    RELEASE_MEM(pKey);

    return TRUE;
}

static symbol_k * symbolKValue(symbol_k *key) {
    if (isNullPtr(key)) 
        return NULL;

    return (void *)key->ident;
}

static int symbolKIsEqual(symbol_k *lKey, symbol_k *rKey) {
    if (isNullPtr(lKey) || isNullPtr(rKey))
        return FALSE;

    return !strncmp(lKey->ident, rKey->ident, strlen(lKey->ident));
}

static symbol_k * symbolKInit(symbol_k *key, char *name) {
    if (isNullPtr(key) || isNullPtr(name))
        return NULL;

    key->ident = name;
    key->base.isEqual = (pairKeyIsEqual)symbolKIsEqual;
    key->base.release = (pairKeyRelease)symbolKRelease;
    key->base.value = (pairKeyValue)symbolKValue;
    key->base.copy = (pairKeyCopy)symbolKCopy;
    return key;
}

static symbol_k * symbolKConst(char *name) {
    symbol_k *pKey;
    
    if (isNullPtr(name))
        return NULL;

    pKey = (symbol_k *)Malloc(sizeof(symbol_k));

    return symbolKInit(pKey, name);
}

static int symbolTblHash(symbol_k *key) {
    char *keyStr;
    int hashVal, size, index, value;

    if (isNullPtr(key))
        return FALSE;

    keyStr = key->ident; 

    hashVal = 0;
    size = strlen(keyStr);

    for (index = 0; index < size; ++index) {
        value = keyStr[index];
        hashVal += HASH_DEFAULT_METHOD(value); 
    } 
    return hashVal;
}

int symTableInit() {
    symbolTableInit(&symTable);
    symbolTableAdd(&symTable, symbolNodeConst("iso", "N/A", "1"));
    return OK;
}

symbolTable * symbolTableInit(symbolTable *tbl) {
    if (isNullPtr(tbl))
        return NULL;
    
    tbl->numOfSymbols = 0;
    tbl->symbolMap = hashMapConstruct(SIZE_OF_SYMBOL_TBL, (hashFunction)symbolTblHash);

    return tbl;
}

symbolTable * symbolTableConstruct() {
    symbolTable *newTable;
     
    newTable = (symbolTable *)Malloc(sizeof(symbolTable));
    return symbolTableInit(newTable);
}

int symbolTableRelease(symbolTable *tbl) {
    if (isNullPtr(tbl))
        return FALSE;

    hashMapRelease(tbl->symbolMap);
    RELEASE_MEM(tbl);

    return TRUE;
}

int symbolTableRelease_static(symbolTable *tbl) {
    if (isNullPtr(tbl))
        return FALSE;

    hashMapRelease(tbl->symbolMap);

    return TRUE;
}

symbol_t * symbolTableSearch(symbolTable *tbl, char *sym) {
    symbol_k key;
    hashMap *symMap;

    if (isNullPtr(tbl) || isNullPtr(sym))
       return NULL; 
    
    symbolKInit(&key, sym); 

    symMap = tbl->symbolMap;
    return (symbol_t *)hashMapGet(symMap, (pair_key_base *)&key);
}

int symbolTableAdd(symbolTable *tbl, symbol_t *sym) {
    symbol_k *key;

    if (isNullPtr(tbl) || isNullPtr(sym)) 
        return FALSE; 
     
    key = symbolKConst(strdup(sym->symIdent));

    return hashMapPut(tbl->symbolMap, (pair_key_base *)key, (pair_val_base *)sym);
}

int symbolTableDelete(symbolTable *tbl, char *symIdent) {
    symbol_k key;

    if (isNullPtr(tbl) || isNullPtr(symIdent))
        return FALSE; 
    symbolKInit(&key, symIdent);
    
    return hashMapDelete(tbl->symbolMap, (pair_key_base *)&key);
}

inline static int nodeMetaRelease(nodeMeta_t *nm) {
    if (isNullPtr(nm))
        return FALSE;

    RELEASE_MEM(nm->parentIdent);
    RELEASE_MEM(nm->suffix);

    return TRUE;
}

inline static int leaveMetaRelease(leaveMeta_t *lm) {
    if (isNullPtr(lm))
        return FALSE;

    RELEASE_MEM(lm->parent);
    RELEASE_MEM(lm->suffix);
    RELEASE_MEM(lm->type);
    RELEASE_MEM(lm->permission);
    
    return TRUE;
}

symbol_t * symbolCopy(symbol_t *origin) {
    nodeMeta_t *node;
    leaveMeta_t *leave;
    if (isNullPtr(origin))
        return NULL;
    
    if (origin->symType == SYMBOL_TYPE_NODE) {
        node = &origin->symInfo.nodeMeta;
        return symbolNodeConst(strdup(origin->symIdent),
            strdup(node->parentIdent), strdup(node->suffix)); 
    } else if (origin->symType = SYMBOL_TYPE_LEAVE) {
        leave = &origin->symInfo.leaveMeta;
        return symbolLeaveConst(strdup(origin->symIdent),
            strdup(leave->parent), strdup(leave->suffix),
            strdup(leave->type), strdup(leave->suffix));  
    }
}

int symbolRelease(symbol_t *sym) {
    if (isNullPtr(sym))
        return FALSE;
    
    if (sym->symType == SYMBOL_TYPE_NODE)
        nodeMetaRelease(&sym->symInfo.nodeMeta);
    else
        leaveMetaRelease(&sym->symInfo.leaveMeta);

    RELEASE_MEM(sym->symIdent);     
    RELEASE_MEM(sym);
    
    return TRUE;  
}

char * symbolIdent(symbol_t *sym) {
    if (isNullPtr(sym))
        return NULL;
    return sym->symIdent;
}

int symbolIsEqual(symbol_t *symLeft, symbol_t *symRight) {
    if (isNullPtr(symLeft) || isNullPtr(symRight))
        return FALSE;
    return !strncmp(symLeft->symIdent, symRight->symIdent, strlen(symLeft->symIdent));
}

symbol_t * symbolNodeConst(char *ident, char *parent, char *suffix) {
    symbol_t *pSym;

    if (isNullPtr(ident) || isNullPtr(parent) || isNullPtr(suffix))
       return NULL; 

    pSym = (symbol_t *)Malloc(sizeof(symbol_t));
    pSym->symType = SYMBOL_TYPE_NODE;
    pSym->symIdent = ident;
    pSym->symInfo.nodeMeta.parentIdent = parent;
    pSym->symInfo.nodeMeta.suffix = suffix;

    pSym->base.release = (pairValRelease)symbolRelease;
    pSym->base.value = (pairValValue)symbolIdent;
    pSym->base.isEqual = (pairValIsEqual)symbolIsEqual;
    pSym->base.copy = (pairValCopy)symbolCopy;

    return pSym;
}

symbol_t * symbolLeaveConst(char *ident, char *parent, char *suffix, char *type, char *perm) {
    symbol_t *pSym;

    if (isNullPtr(ident) || isNullPtr(parent) || isNullPtr(suffix) ||
        isNullPtr(type) || isNullPtr(perm))
        return NULL;

    pSym = (symbol_t *)Malloc(sizeof(symbol_t));
    pSym->symType = SYMBOL_TYPE_LEAVE;
    pSym->symIdent = ident;
    
    pSym->symInfo.leaveMeta.parent = parent;
    pSym->symInfo.leaveMeta.suffix = suffix;
    pSym->symInfo.leaveMeta.type = type;
    pSym->symInfo.leaveMeta.permission = perm; 

    pSym->base.release = (pairValRelease)symbolRelease;
    pSym->base.value = (pairValValue)symbolIdent;
    pSym->base.isEqual = (pairValIsEqual)symbolIsEqual;
    pSym->base.copy = (pairValCopy)symbolCopy;

    return pSym;
}

// typeItem
typeItem * typeItemConst();
typeItem * typeItemCopy(typeItem *item);
int typeItemDestruct(typeItem *item);
int typeItemAssignment(typeItem *lval, const typeItem *rval);
int typeItemIsEqual(const typeItem *first, const typeItem *sec);

typeItem * typeItemSearch(const typeItem *items, const char *typeName);
int typeItemAppend(typeItem *items, char *typeName);
int typeItemDel(typeItem *items, char *typeName);

typeItem * typeItemPrev(const typeItem *item);
typeItem * typeItemNext(const typeItem *item);
typeItem * typeItemTail(const typeItem *item);

typeItem * typeItemConst() {
    typeItem *item = (typeItem *)Malloc(sizeof(typeItem));

    memset(item, 0, sizeof(typeItem));

    return item;
}

typeItem * typeItemCopy(typeItem *item) {
    if (isNullPtr(item)) return NULL;
    
    typeItem *item_copy = typeItemConst();
    item_copy->type = strdup(item->type);  

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

    lval->type = strdup(rval->type);

    return OK;
}

typeItem * typeItemSearch(const typeItem *items, const char *typeName) {
    if (isNullPtr(items) || isNullPtr(typeName))
        return NULL; 

    for (; isNonNullPtr(items); items = typeItemNext(items))
        if (isStringEqual(items->type, typeName)) break;

    return (typeItem *)items;
}

int typeItemIsEqual(const typeItem *first, const typeItem *sec) {
    if (isNullPtr(first) || isNullPtr(sec))
        return ERROR;
    
    return isStringEqual(first->type, sec->type);
}

int typeItemAppend(typeItem *items, char *typeName) {
    if (isNullPtr(items) || isNullPtr(typeName))
        return ERROR;

    typeItem *lastItem = typeItemTail(items);
    typeItem *newItem = typeItemConst();

    newItem->type = typeName;     

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


// typeTable
typeTable * typeTableConst() {
    typeTable *tbl = (typeTable *)Malloc(sizeof(typeTable));
    
    memset(tbl, 0, sizeof(typeTable));

    return tbl;    
}

typeTable * typeTableCopy(const typeTable *tbl) {}

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

int typeTableAssignment(typeTable *lval, const typeTable *rval) {}

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
    
    listNodeIsEqual(&firstTbl->items.node, &secTbl->items.node, typeTableEqualChecke);

    return OK;
}

int typeTableIsTypeExists(const typeTable *tbl, const char *type) {
    if (isNullPtr(tbl) || isNullPtr(type))
        return ERROR;
    
    _Bool isExists;
    typeItem *found = typeItemSearch(&tbl->items, type);
    if (isNonNullPtr(found)) {
        isExists = TRUE; 
    } else {
        isExists = FALSE; 
    }

    return isExists;
} 

int typeTableAdd(typeTable *tbl, char *type) {
    if (isNullPtr(tbl) || isNullPtr(type)) 
        return ERROR;

    if (typeItemAppend(&tbl->items, type) == ERROR)
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

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void list_symbolTable(void **state) {
    symbol_t *symbol, *found;
    symbolTable symTbl; 
    
    symbolTableInit(&symTbl);    
    
    symbol = symbolNodeConst("B", "A", "1");
    symbolTableAdd(&symTbl, symbol);

    found = symbolTableSearch(&symTbl, "B");
    assert_non_null(found);

    assert_string_equal(found->symIdent, "B");
}

void list__TYPE_TABLE(void **state) {
    _Bool isExists;
    typeTable *tbl = typeTableConst();         

    typeTableAdd(tbl, strdup("INTEGER"));
    isExists = typeTableIsTypeExists(tbl, "INTEGER");

    assert_int_equal(isExists, TRUE);

    typeTableDel(tbl, "INTEGER");
    isExists = typeTableIsTypeExists(tbl, "INTEGER");
    assert_int_equal(isExists, FALSE); 
}

#endif /* MIB2DOC_UNIT_TESTING */

/* end of list.c */

