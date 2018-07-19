//
// Created by ayden on 17-5-1.
//

#include <stddef.h>
#include <malloc.h>
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
listNode *listNodePrev(listNode *sliNode) {
    if (isNullPtr(sliNode) || isNullPtr(sliNode->next))
        return NULL;
    return sliNode->next;
}

listNode * listNodeNext(listNode *sliNode) {
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
    } else {
        middle = head->next;
        head->next = sliNode;
        sliNode->next = middle;
    }
    return sliNode;
}

listNode *listNodeDelete(listNode *sliNode) {
    if (isNullPtr(sliNode)) {
        return NULL;
    }
    if (sliNode->prev != NULL && sliNode->next != NULL) {
        /* Middle */
        sliNode->prev->next = sliNode->next;
        sliNode->next->prev = sliNode->prev;
    } else if(sliNode->prev != NULL) {
        /* Tail */
        sliNode->next->prev = NULL;
    } else if (sliNode->next != NULL) {
        /* Head */
        sliNode->prev->next = NULL;
    } else {
        /* Single sliNode */
    }
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
    if (isNullPtr(sli) || isNullPtr(sli->sliNode.prev)) {
        return NULL;
    }
    return containerOf(sli->sliNode.prev, slice, sliNode);
}

slice * sliceNext(slice *sli) {
    if (isNullPtr(sli) || isNullPtr(sli->sliNode.next)) {
        return NULL;
    }
    return containerOf(sli->sliNode.next, slice, sliNode);
}

slice * sliceGet(slice *sliHead, int sliKey) {
    if (isNullPtr(sliHead)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }

    while (sliHead != NULL) {
        if (sliHead->sliKey == sliKey)
            return sliHead;
        sliHead = sliceNext(sliHead);
    }
    return NULL;
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

bool sliceRelease_STATIC(slice *sli) {
    slice *pSli;
    slice *pSli_next;

    if (isNullPtr(sli)) 
        return FALSE;
    pSli = sli;
    while (pSli != NULL) {
        pSli_next = sliceNext(pSli); 
        if (pSli->sliKey != 0)
            sliceDestruct(pSli);
        else {
            sli->sliNode.next = NULL;
            sli->sliNode.prev = NULL;
        }
        pSli = pSli_next;
    } 
}

bool sliceRelease(slice *sli) {
    slice *pSli;
    slice *pSli_next;

    if (isNullPtr(sli)) {
        return FALSE;
    }
    for (pSli = sli; pSli != NULL; pSli = pSli_next) {
        pSli_next = sliceNext(pSli);
        sliceDestruct(pSli);
    }
    return TRUE;
}

bool sliceReset(slice *sli) {
    if (isNullPtr(sli)) {
        return FALSE;
    }
    sliceRelease(sliceNext(sli));
    sli->sliNode.next = NULL;

    return TRUE;
}

/**************************************
 * dispatchParam Operation function define *
 **************************************/

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
    return containerOf(disparam->node.prev, dispatchParam, node);
}

dispatchParam * dispatchParamNext(dispatchParam *disparam) {
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

/*
 * Get sliNode from the head from the list
 * after that the head will be
 * removed from list.
 */
dispatchParam * disParamRetrive(dispatchParam **head) {
    dispatchParam *ret;

    if (isNullPtr(head) || isNullPtr(*head)) {
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

symbolTable * symbolTableConstruct(char *name) {
    symbolTable *newTable;

    if (isNullPtr(name)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }
    newTable = (symbolTable *)malloc(sizeof(symbolTable));
    memset(newTable, 0, sizeof(symbolTable));
    newTable->modName = name;
    return newTable;
}

symbolTable * symbolTablePrev(symbolTable *tbl) {
    return containerOf(tbl->symTblNode.prev, symbolTable, symTblNode);
}

symbolTable * symbolTableNext(symbolTable *tbl) {
    return containerOf(tbl->symTblNode.next, symbolTable, symTblNode);
}

/* Return :
 *  @NULL - if mib2docError == ERROR_NULL_REF means nul pointer
 *          otherwise symbol table not found.
 *  @pointer - pointer that point the table specify by modName
 */
symbolTable * symbolTableSearch(symbolTable *tblRoot, char *modName) {
    if (isNullPtr(tblRoot) || isNullPtr(modName)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }
    while (tblRoot) {
        if (!strncmp(tblRoot->modName, modName, strlen(modName))) {
            return tblRoot;
        }
        tblRoot = symbolTableNext(tblRoot);
    }
    mib2docError = SYM_TABLE_NOT_FOUND;
    return NULL;
}

symbolTable * symbolModuleAdd(symbolTable *tblRoot, symbolTable *newTbl) {
    if (isNullPtr(tblRoot) || isNullPtr(newTbl)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }

    listNodeInsert(listNodeTail(&tblRoot->symTblNode), &newTbl->symTblNode);
    return newTbl;
}

symbol_t * symbolPrev(symbol_t *sym) {
    return containerOf(sym->symNode.prev, symbol_t, symNode);
}

symbol_t * symbolNext(symbol_t *sym) {
    return containerOf(sym->symNode.next, symbol_t, symNode);
}

symbol_t * symbolAdd(symbolTable *symTbl, symbol_t *newSym, char *modName) {
    symbolTable *pSymTbl;
    symbol_t *pSym;

    if (isNullPtr(modName) || isNullPtr(newSym) || isNullPtr(symTbl)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }
    if ((pSymTbl = symbolTableSearch(symTbl, modName)) == NULL) {
        if (mib2docError == SYM_TABLE_NOT_FOUND)
            pSymTbl = symbolModuleAdd(symTbl, symbolTableConstruct(modName));
    }

    pSym = pSymTbl->symbol;
    if (!symbolSearching(pSymTbl, newSym->symIdent)) {
        if (mib2docError == SYM_NOT_FOUND)
            listNodeInsert(listNodeTail(&pSym->symNode), &newSym->symNode);
            return pSym;
    }
    return NULL;
}

symbol_t * symbolTravel(symbolTable *symTblRoot, int (*func)(symbol_t *sym, void *arg), void *arg) {
    int retVal;
    symbol_t *ret = NULL;
    symbolTable *pSymTbl;
    symbol_t *pSym;

    if (isNullPtr(symTblRoot)) {
        mib2docError = ERROR_NULL_REF;
        return ret;
    }

    pSymTbl = symTblRoot;

    while (symbolTableNext(pSymTbl) != NULL) {
        pSym = pSymTbl->symbol;

        while (pSym != NULL) {
            retVal = func(pSym, arg);
            if (retVal == SYM_TRAVEL_CONTINUE) {
                continue;
            } else if (retVal == SYM_TRAVEL_STOP) {
                return pSym;
            } else if (retVal == ERROR_NULL_REF) {
                return NULL;
            }
            pSym = symbolNext(pSym);
        }
        pSymTbl = symbolTableNext(pSymTbl);
    }
    mib2docError = SYM_NOT_FOUND;

FINISHED:
    return ret;
}

static int symbolIsParentEqual(symbol_t *sym, void *arg) {
    char *parent;
    symbol_t *pSym;
    symbol_t *foundSym;

    if (isNullPtr(sym) || isNullPtr(arg)) {
        return ERROR_NULL_REF;
    }

    pSym = (symbol_t *)arg;
    parent = pSym->symIdent;

    if (!strncmp(parent, sym->symIdent, strlen(parent))) {
        foundSym = (symbol_t *)malloc(sizeof(symbol_t));
        memcpy(foundSym, sym, sizeof(symbol_t));
        pSym->symNode.next = &foundSym->symNode;
    }

    return SYM_TRAVEL_CONTINUE;
}

int symbolSearching(symbolTable *symTblRoot, char *sym) {
    return 0;
}

int symbolSearchingByParent(symbolTable *symTblRoot, char *parent, symbol_t *sym) {
    if (isNullPtr(symTblRoot) || isNullPtr(parent) || isNullPtr(sym)) {
        return ERROR_NULL_REF;
    }

    memset(sym, 0, sizeof(symbol_t));
    sym->symIdent = parent;
    symbolTravel(symTblRoot, symbolIsParentEqual, (void *)sym);

    if (sym->symNode.next != NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

/* end of list.c */
