//
// Created by ayden on 17-5-1.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "type.h"
#include "dispatcher.h"
#include "symbolTbl.h"

enum {
    SYM_TRAVEL_CONTINUE = 10,
    SYM_TRAVEL_STOP = 11
};

/************
 * ListNode *
 ************/
listNode *listNodeNext(listNode *sliNode) {
    if (isNullPtr(sliNode) || isNullPtr(sliNode->next))
        return NULL;  
    return sliNode->next;
}

listNode * listNodePrev(listNode *sliNode) {
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

listNode * listNodeTail(listNode *head) {
    if (isNullPtr(head)) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return head;
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

int listNodeTravel(listNode *head, listNodeTask func, void *arg) {
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
    slice *sli = (slice *)malloc(sizeof(slice));

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

    sli = sliceNext(sli);

    sliceRelease(sli);

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
     
    ret  = (dispatchParam *)malloc(sizeof(dispatchParam));
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

    copy = (symbol_k *)malloc(sizeof(symbol_k));
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

    pKey = (symbol_k *)malloc(sizeof(symbol_k));

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
     
    newTable = (symbolTable *)malloc(sizeof(symbolTable));
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

    pSym = (symbol_t *)malloc(sizeof(symbol_t));
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

    pSym = (symbol_t *)malloc(sizeof(symbol_t));
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

#ifdef MIB2DOC_UNIT_TESTING

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

#endif /* MIB2DOC_UNIT_TESTING */

/* end of list.c */

