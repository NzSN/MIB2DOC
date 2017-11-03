//
// Created by ayden on 17-5-1.
//

#include <malloc.h>
#include <string.h>
#include "list.h"
#include "type.h"

/************
 * ListNode *
 ************/
listNode *listNodePrev(listNode *node) {
    if (isNullPtr(node) || isNullPtr(node->next))
        return NULL;
    return node->next;
}

listNode * listNodeNext(listNode *node) {
    if (isNullPtr(node) || isNullPtr(node->prev))
        return NULL;
    return node->prev;
}

listNode * listNodeInsert(listNode *head, listNode *node) {
    listNode *middle;

    if (isNullPtr(head) || isNullPtr(node)) {
        return NULL;
    }
    if (head->next == NULL) {
        head->next = node;
    } else {
        middle = head->next;
        head->next = node;
        node->next = middle;
    }
    return node;
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

/*******************************************
 *  Element list Operation function define *
 *******************************************/

slice * sliceConstruct(int sliKey, char *sliVal) {
    slice *sli = (slice *)malloc(sizeof(slice));

    memset(newSlice, 0, sizeof(slice));
    sli->sliKey = sliKey;
    sli->sliVal = sliVal;

    return sli;
}

bool sliceDestruct(slice *sli) {
    if (isNullPtr(sli)) {
        return FALSE;
    }
    if (isNullPtr(sli->sliVal)) {
        RELEASE_MEM(sli->sliVal);
    }
    RELEASE_MEM(sli);
    return TRUE;
}

slice * slicePrev(slice *sli) {
    if (isNullPtr(sli)) {
        return NULL;
    }
    return containerOf(sli->node.prev, slice, node);
}

slice * sliceNext(slice *sli) {
    if (isNullPtr(sli)) {
        return NULL;
    }
    return containerOf(sli->node.next, slice, node);
}

slice * sliceGet(slice *sli, int sliKey) {
    for (; sli != NULL; sli = sliceNext(sli)) {
        if (sli->sliKey == sliKey)
            return sli;
    }
    return NULL;
}

int sliceStore(slice *sli, slice *next) {
    for (; sli != NULL; sli = sliceNext(sli)) {
        if (sliceNext(sli) == NULL) {
            sli->node.next = &next->node;
            return 0;
        }
    }
    return 1;
}

bool sliceRelease(slice *sli) {
    slice *pSli;
    slice *pSli_next;

    if (isNullPtr(sli)) {
        return FALSE;
    }
    for (pSli = sli; pSli != NULL; pSli = pSli_next;) {
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
    sli->node.next = NULL;

    return TRUE;
}


/**************************************
 * dispatchParam Operation function define *
 **************************************/

 /*
  * append node to tail of list
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
    return cointainerOf(disparam->node.next, dispatchParam, node);
}

dispatchParam * disParamStore(dispatchParam *head, dispatchParam *pl) {
    if (head == NULL || pl == NULL) {
        return NULL;
    }

    while (head->next != NULL) {
        head = dispatchParamNext(head);
    }
    head->node.next = &pl->node;
    return pl;
}

/*
 * Get node from head fo list
 * after that the head will be
 * removed from list.
 */
dispatchParam * disParamGet(dispatchParam **head) {
    dispatchParam *ret;

    if (isNullPtr(head) || isNullPtr(*head)) {
        return NULL;
    }

    ret = *head;
    *head = dispatchParamNext(*head);

    return ret;
}

/*****************************************************
 * symbol_t and symbolTable operation function define *
 *****************************************************/
extern symbolTable symTable;

symbolTable * symbolTableConstruct(char *name) {
    symbolTable *newTable;

    if (isNullPtr(name)) {
        return NULL;
    }
    newTable = (symbolTable *)malloc(sizeof(symbolTable));
    memset(newTable, NULL, sizeof(symbolTable));
    newTable->modName = name;
    return newTable;
}

symbolTable * symbolTablePrev(symbolTable *tbl) {
    return containerOf(tbl->node.prev, symbolTable, node);
}

symbolTable * symbolTableNext(symbolTable *tbl) {
    return containerOf(tbl->node.next, symbolTable, node);
}

int symbolModuleAdd(symbolTable *tblRoot, symbolTable *newTbl) {
    int ret = ok;

    if (isNullPtr(tblRoot) || isNullPtr(newTbl)) {
        ret = -1;
        return ret;
    }

    while (!isNullPtr(tblRoot->node.next)) {
        tblRoot = tsymTableNext(tblRoot);
    }

    tblRoot->node.next = newTbl->node;

    return ret;
}

int symbolAdd(char *modName, symbol_t *newSym) {
    int ret;
    symbolTable *pSymTbl = &symTable;
    symbol_t *pSym;

    if (isNullPtr(modName) || isNullPtr(newSym)) {
        ret = -1;
        return ret;
    }

    while (!strncmp(modName, pSymTbl->modName, strlen(modName))) {
        pSymTbl = pSymTbl->next;
    }

    /* no module match the modName */
    if (pSymTbl == NULL) {
        return 1;
    }

    pSym = pSymTbl->symbol;

    while (pSym->next != NULL) {
        pSym = pSym->next;
    }

    pSym->next = newSym;
    return ret;
}

int symbolModSearching(char *modIdent) {}

/*
 *  return value:
 *      0 -- not exist
 *      1 -- exists
 */
int symbolSearching(char *symIdent) {
    int ret = 0;
    symbolTable *pSymTbl = &symTable;
    symbol_t *pSym;

    if (isNullPtr(symIdent)) {
        ret = -1;
        return ret;
    }

    while (pSymTbl->next != NULL) {
        pSym = pSymTbl->symbol;

        while (pSym->next != NULL) {
            if (!strncmp(symIdent, pSym->identifier, strlen(symIdent))) {
                ret = 1;
                goto FINISHED;
            }
            pSym = pSym->next;
        }
        pSymTbl = pSymTbl->next;
    }

FINISHED:
    return ret;
}
