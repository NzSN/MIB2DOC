//
// Created by ayden on 17-5-1.
//

#ifndef UNIT_TEST_LSIT_H
#define UNIT_TEST_LSIT_H

typedef struct elementList {
    int key;
    char *content;
    struct elementList *next;
} elementList;

typedef struct listNode {
    struct listNode next;
    struct listNode prev;
} listNode;

/* ListNode */
listNode *listPrev(listNode *node);
listNode *listNext(listNode *node);

/* Element List */
elementList * buildElement(int key, char *content);
elementList * getElement_el(elementList *el, int key);
int appendElement_el(elementList *el, elementList *next);
void flushAll_el(elementList *el);
int reset_el(elementList *el);

/* params_t */
params_t * buildParam(void *arg);
params_t * paramListAppend(params_t *head, params_t *pl);
params_t * paramListGet(params_t **head);

/* symbol_t and SymbolTable */
int symbolModuleAdd(symbolTable *stbl, symbolTable *new);
int symbolAdd(char *modName, symbol_t *newSym);
int symbolModSearching(char *modIdent);
int symbolSearching(char *symIdent);


#endif //UNIT_TEST_LSIT_H
