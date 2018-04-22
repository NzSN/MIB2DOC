//
// Created by ayden on 17-5-1.
//

#ifndef _MIB2DOC_LIST_H_
#define _MIB2DOC_LIST_H_

#include "type.h"
#include <stdbool.h>

typedef struct listNode {
    struct listNode *next;
    struct listNode *prev;
} listNode;

typedef struct slice {
    int sliKey;
    char *sliVal;
    listNode sliNode;
} slice;

/* ListNode */
typedef void (*listNodeTask)(listNode *head, void *arg);
listNode *listNodePrev(listNode *node);
listNode *listNodeNext(listNode *node);
listNode *listNodeInsert(listNode *head, listNode *node);
bool listNodeIsEmpty(listNode *node);
int listNodeTravel(listNode *head, listNodeTask func, void *arg);

/* Slice */
slice * sliceConstruct(int sliKey, char *sliVal);
slice * sliceGet(slice *sli, int sliKey);
int sliceStore(slice *sli, slice *next);
bool sliceRelease(slice *sli);
bool sliceReset(slice *sli);

/* dispatchParam */
dispatchParam * disParamConstruct(void *arg);
dispatchParam * disParamStore(dispatchParam *head, dispatchParam *pl);
dispatchParam * disParamRetrive(dispatchParam **head);

/* symbol_t and SymbolTable */
symbolTable * symbolModuleAdd(symbolTable *tblRoot, symbolTable *newTbl);
symbol_t * symbolAdd(symbolTable *symTbl, symbol_t *newSym, char *modName);
int symbolModSearching(char *modIdent);
int symbolSearching(char *symIdent);


#endif /* _MIB2DOC_LIST_H_ */
