/* list.c 
 * 
 * Todo: (1) refactoring list module to provide a generic linear structure
 *       (2) Type name of some type define in this module is so specific such as
 *           the type dispatchParam the name of type hurt generic of it, cause
 *           a it contain a <void *> pointer inside its structure it can be more
 *           generic than present.
 */

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

/***********
 * List    *
 ***********/
list * listConst(void) {
    list *l = (list *)zMalloc(sizeof(list));
    return l;
}

static _Status listReleaseHelper(listNode *head, void *arg) {
    list *l = arg;
    l->release(containerOf(head, listEntry, node));
    return OK;
}
void listRelease(list *l) {
    listIter iter;
    listEntry *current, *next;

    if (isNullPtr(l))
        return /* void */;

    if (l->entry)
        listNodeTravel(&l->entry->node, listReleaseHelper, (void *)l);

    return /* void */;
}

static _Status listCopyHelper(listNode *head, void *arg) {
    list *l_copy = arg;
    listEntry *current = containerOf(head, listEntry, node);
    listAppend(l_copy, l_copy->copy(current->element));
    return OK;
}
list * listCopy(list *l) {
    if (isNullPtr(l))
        return NULL;

    list *l_copy = (list *)zMalloc(sizeof(list));

    listSetSize(l_copy, 0);

    listSetCopyMethod(l_copy, l->copy);
    listSetReleaseMethod(l_copy, l->release);
    listSetEqualMethod(l_copy, l->isEqual);

    if (l->entry)
        listNodeTravel(&l->entry->node, listCopyHelper, (void *)l_copy);

    return l_copy;
}

int listAssignment(list *l, list *r) {
    listEntry entry = { 0 };

    if (isNullPtr(l) || isNullPtr(r))
        return ERROR;

    listSetSize(l, 0);

    listSetCopyMethod(l, r->copy);
    listSetReleaseMethod(l, r->release);
    listSetEqualMethod(l, r->isEqual);

    // Release entries of list l
    if (l->entry) listNodeTravel(&l->entry->node, listReleaseHelper, (void *)l);
    // Copy entries into list l
    listNodeTravel(&r->entry->node, listCopyHelper, (void *)l);

    return OK;
}

_Bool listEqualHelper(const listNode *node_l, const listNode *node_r, void *arg) {
    list *l = arg;
    listEntry *entry_l = containerOf(node_l, listEntry, node);
    listEntry *entry_r = containerOf(node_r, listEntry, node);

    return l->isEqual(entry_l->element, entry_r->element);
}
int listIsEqual(list *l, list *r) {
    if (isNullPtr(l) || isNullPtr(r))
        return false;

    if (l->size != r->size)
        return false;

    return listNodeIsEqual_extra(&l->entry->node, &r->entry->node, (void *)l, listEqualHelper);
}

int listConcate(list *l, list *r) {
    if (isNullPtr(l) || isNullPtr(r))
        return ERROR;

    // Append first entry of r onto last entry of l
    listNodeAppend(&l->entry->node, &r->entry->node);
    l->size += r->size;

    return OK;
}

int listAppend(list *l, void *elem) {
    if (isNullPtr(l) || isNullPtr(elem))
        return ERROR;

    listEntry *newEntry = (listEntry *)zMalloc(sizeof(listEntry));
    newEntry->element = elem;

    if (l->entry == null) {
        l->entry = newEntry;
        ++l->size;
        return OK;
    }

    listNodeAppend(&l->entry->node, &newEntry->node);
    ++l->size;

    return OK;
}

void * listRetriveTail(list *l) {
    if (isNullPtr(l) || isNullPtr(l->entry))
        return NULL;

    listNode *tail = listNodeTail(&l->entry->node);
    listEntry *lastEntry = containerOf(tail, listEntry, node);
    void *elem = lastEntry->element;

    if (--l->size == 1)
        l->entry = NULL;

    listNodeDelete(tail);
    RELEASE_MEM(lastEntry);

    return elem;
}

int listPush(list *l, void *elem) {
    if (isNullPtr(l) || isNullPtr(elem))
        return ERROR;

    listEntry *newEntry = (listEntry *)zMalloc(sizeof(listEntry));
    newEntry->element = elem;

    if (l->entry == NULL) {
        ++l->size;
        l->entry = newEntry;
        return OK;
    }

    listNodeAppend(&l->entry->node, &newEntry->node);
    l->entry = newEntry;

    ++l->size;

    return OK;
}

void * listPop(list *l) {
    if (isNullPtr(l) || l->size == 0)
        return NULL;

    listEntry *firstEntry = l->entry;
    void *elem = firstEntry->element;

    l->entry = containerOf(firstEntry->node.next, listEntry, node);

    if (--l->size == 0)
        l->entry = NULL;

    listNodeDelete(firstEntry);
    RELEASE_MEM(firstEntry);

    return elem;
}

listIter listGetIter(list *l) {
    listIter iter = { 0 };

    if (isNullPtr(l))
        return iter;

    iter.entry = l->entry;
    iter.l = l;

    return iter;
}

listIter listPredecessor(listIter li) {
    listIter iter;

    iter.l = li.l;
    if (li.entry && li.entry->node.prev)
        iter.entry = containerOf(li.entry->node.next, listEntry, node);

    return iter;
}

listIter listSuccessor(listIter li) {
    listIter iter;

    iter.l = li.l;
    if (li.entry && li.entry->node.prev)
        iter.entry = containerOf(li.entry->node.prev, listEntry, node);

    return iter;
}

listEntry * listPrev(listIter *li) {
    if (isNullPtr(li)) return NULL;

    listEntry *current = li->entry;
    if (current)
        if (current->node.prev)
            li->entry = containerOf(current->node.prev, listEntry, node);
        else
            li->entry = NULL;

    return current;
}

listEntry * listNext(listIter *li) {
    if (isNullPtr(li)) return NULL;

    listEntry *current = li->entry;
    if (current)
        if (current->node.next)
            li->entry = containerOf(current->node.next, listEntry, node);
        else
            li->entry = NULL;

    return current;
}

listEntry * listSource(listIter li) {
    return li.entry;
}

int listSink(listIter li_l, listIter li_r) {
    if (li_l.entry && li_r.entry) {
        li_l.l->release(li_l.entry->element);
        li_l.entry->element = li_l.l->copy(li_r.entry->element);
    }
    return OK;
}

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

_Bool listNodeIsEqual_extra(const listNode *first, const listNode *second,
                            void *arg, listNodeEqualCheck_extra equalCheck) {

    if (isNullPtr(first) || isNullPtr(second) || isNullPtr(equalCheck))
        return ERROR;

    _Bool isEqual;
    const listNode *current_first = first;
    const listNode *current_second = second;

    while (isNonNullPtr(current_first) && isNonNullPtr(current_second)) {
        isEqual = equalCheck(current_first, current_second, arg);
        if (!isEqual) return FALSE;

        current_first = listNodeNext(current_first);
        current_second = listNodeNext(current_second);
    }
    return TRUE;
}

_Status listNodeTravel(listNode *head, listNodeTask func, void *arg) {
    listNode *next;

    if (isNullPtr(head) || isNullPtr(func)) {
        return ERROR_NULL_REF;
    }

    next = listNodeNext(head);

    while (head != NULL) {
        func(head, arg);

        head = next;
        next = listNodeNext(next);
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

slice * sliceGet(slice *sliHead, int sliKey) {
    if (isNullPtr(sliHead)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }
    
    sliceIter iter = sliceGetIter(sliHead);
    slice *current = sliceNext(&iter);

    while (current != NULL) {
        if (current->sliKey == sliKey)
            break;
        current = sliceNext(&iter);
    }
    return current;
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

    return sliceStore_without_key(sliHead, newSli);
}

int sliceStore_without_key(slice *sliHead, slice *newSli) {
    if (isNullPtr(sliHead) || isNullPtr(newSli))
        return ERROR;

    sliceIter iter = sliceGetIter(sliHead);
    while (sliHead != NULL) {
        if (IS_LAST_SLICE_NODE(sliHead)) {
            sliHead->sliNode.next = &newSli->sliNode;
            newSli->sliNode.prev = &sliHead->sliNode;
            return OK;
        }
        sliHead = sliceNext(&iter);
    }
    return OK;
}

int sliceAssignment(slice *sl_l, slice *sl_r) {
    if (isNullPtr(sl_l) || isNullPtr(sl_r))
        return ERROR;

    sl_l->sliKey = sl_r->sliKey;
    sl_l->sliVal = sl_r->sliVal;

    return OK;
}

bool sliceRelease(slice *sli) {
    slice *pSli;
    slice *pSli_next;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    for (pSli = sliceNext(&iter); pSli != NULL; pSli = pSli_next) {
        pSli_next = sliceNext(&iter);
        sliceDestruct(pSli);
    }
    return TRUE;
}

bool sliceRelease_STATIC(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    iter = sliceSuccessor(iter);

    sliceRelease(sliceSource(iter));

    sli->sliNode.next = NULL;
    sli->sliNode.prev = NULL;

    return TRUE;
}

bool sliceReset(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    while (sli != NULL) {
        pSli = sli;
        sli = sliceNext(&iter);
        RELEASE_MEM(pSli);
    }
    return TRUE;
}

bool sliceReset_STATIC(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    if (sliceReset(sliceNext(&iter))) {
        memset(&sli->sliNode, 0, sizeof(listNode));
    }
    return TRUE;
}

sliceIter sliceGetIter(slice *sl) {
    sliceIter si = {
        .sl = sl,
        .node = &sl->sliNode
    };
    return si;
}

sliceIter sliceSuccessor(sliceIter si) {
    si.node = listNodeNext(si.node);
    return si;
}

sliceIter slicePredecessor(sliceIter si) {
    si.node = listNodePrev(si.node);
    return si;
}

slice * sliceNext(sliceIter *si) {
    if (isNullPtr(si) || isNullPtr(si->node))
        return null;

    listNode *current = si->node;
    si->node = listNodeNext(current);
    return containerOf(current, slice, sliNode);
}

slice * slicePrev(sliceIter *si) {
    if (isNullPtr(si) || isNullPtr(si->node->prev))
        return null;
    si->node = listNodePrev(si->node);
    return containerOf(si->node, slice, sliNode);
}

slice * sliceSource(sliceIter si) {
    if (isNullPtr(si.node)) return NULL;
    return containerOf(si.node, slice, sliNode);
}

int sliceSink(sliceIter si_l, sliceIter si_r) {
    if (isNullPtr(si_l.node) || isNullPtr(si_r.node))
        return ERROR;

    slice *sl_l = sliceSource(si_l);
    slice *sl_r = sliceSource(si_r);
    sliceAssignment(sl_l, sl_r);

    return OK;
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
    ++tbl->numOfSymbols;

    return hashMapPut(tbl->symbolMap, (pair_key_base *)key, (pair_val_base *)sym);
}

int symbolTableDelete(symbolTable *tbl, char *symIdent) {
    symbol_k key;

    if (isNullPtr(tbl) || isNullPtr(symIdent))
        return FALSE;
    symbolKInit(&key, symIdent);
    --tbl->numOfSymbols;
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

int typeItemAppend(typeItem *items, char *typeName, typeCate cate, const void *ref) {
    if (isNullPtr(items) || isNullPtr(typeName))
        return ERROR;

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

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void insert_range_to_list(list *l, int from, int to) {
    while (from <= to) {
        listAppend(l, numberToStr(from++));
    }
}

listEntry * peek_next_list(listIter *iter) {
    return listNext(iter);
}

void listRelease_Test(void *elem) {
    free(elem);
}

void * listCopy_Test(void *elem) {
    char *orig = (char *)elem;
    char *copy;

    copy = (char *)zMalloc(strlen(orig));
    strncpy(copy, orig, strlen(orig));

    return copy;
}

int listIsEqual_Test(void *l, void *r) {
    char *l_c = (char *)l;
    char *r_c = (char *)r;
    return TRUE;
}

void list__Generic_List(void **state) {
    int num = 0;
    listEntry *entry;
    list *l = listConst();

    listSetReleaseMethod(l, listRelease_Test);
    listSetCopyMethod(l, listCopy_Test);
    listSetEqualMethod(l, listIsEqual_Test);

    // Insert 1000 element into list.
    insert_range_to_list(l, 0, 1000);
    // Peek into entries and check is entries valid
    listIter iter = listGetIter(l);
    while (entry = peek_next_list(&iter)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // Copy validity
    num = 0;
    list *l_copy = listCopy(l);
    listIter iter_copy = listGetIter(l_copy);

    entry = peek_next_list(&iter_copy);
    assert_non_null(entry);
    assert_string_equal(entry->element, numberToStr(num++));

    while (entry = peek_next_list(&iter_copy)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // Assignment checking
    num = 0;
    list *l_assign = listConst();

    listAssignment(l_assign, l);

    listIter iter_assign = listGetIter(l_assign);

    while (entry = peek_next_list(&iter_assign)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // Equal checking
    assert_int_equal(listIsEqual(l_assign, l_copy), TRUE);
    assert_int_equal(listIsEqual(l_assign, l), TRUE);
    assert_int_equal(listIsEqual(l, l_copy), TRUE);
    assert_int_equal(listIsEqual(l, l), TRUE);

    // Should be equal after insert equal value into both list
    listAppend(l, "YY");
    listAppend(l_copy, "YY");
    assert_int_equal(listIsEqual(l, l_copy), TRUE);

    // Should be false in this case
    listAppend(l, "TT");
    assert_int_equal(listIsEqual(l, l_copy), FALSE);

    // List concate operation
    list *l_concate = listConst();
    list *r_concate = listConst();

    insert_range_to_list(l_concate, 0, 4);
    insert_range_to_list(r_concate, 5, 9);

    listConcate(l_concate, r_concate);
    iter = listGetIter(l_concate);

    assert_int_equal(l_concate->size, 10);

    num = 0;
    while (entry = peek_next_list(&iter)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // listRetriveTail testing
    num = 9;
    char *elem;
    while (elem = listRetriveTail(l_concate)) {
        assert_string_equal(elem, numberToStr(num));
        assert_int_equal(l_concate->size, num--);
    }

    // listPush and listPop testing
    list *l_push = listConst();
    listPush(l_push, numberToStr(1));
    listPush(l_push, numberToStr(2));
    assert_string_equal(listPop(l_push), numberToStr(2));
    assert_int_equal(l_push->size, 1);
    assert_string_equal(listPop(l_push), numberToStr(1));
    assert_int_equal(l_push->size, 0);
}

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

    typeTableAdd(tbl, strdup("INTEGER"), CATE_BUILD_IN, NULL);
    isExists = typeTableIsTypeExists(tbl, "INTEGER");

    assert_int_equal(isExists, TRUE);

    typeTableDel(tbl, "INTEGER");
    isExists = typeTableIsTypeExists(tbl, "INTEGER");
    assert_int_equal(isExists, FALSE);

    _Bool isEqual;
    typeTable *tbl_ = typeTableConst();

    typeTableAdd(tbl, strdup("INTEGER"), CATE_BUILD_IN, NULL);
    typeTableAdd(tbl_, strdup("INTEGER"), CATE_BUILD_IN, NULL);
    isEqual = typeTableIsEqual(tbl, tbl_);
    assert_int_equal(isEqual, TRUE);

    int idx = 0;
    while (idx < 100) {
        typeTableAdd(tbl, numberToStr(idx), CATE_BUILD_IN, NULL);
        ++idx;
    }

    idx = 0;
    while (idx < 100) {
        isExists = typeTableIsTypeExists(tbl, numberToStr(idx));
        assert_int_equal(isExists, TRUE);
        ++idx;
    }
}

#endif /* MIB2DOC_UNIT_TESTING */

/* end of list.c */

