//
// Created by ayden on 17-5-1.
//

#include <malloc.h>
#include <string.h>
#include "list.h"
#include "type.h"

/*******************************************
 *  Element list Operation function define *
 *******************************************/

elementList * buildElement(int key, char *content) {
    elementList *newElement = (elementList *)malloc(sizeof(elementList));

    memset(newElement, 0, sizeof(elementList));
    newElement->key = key;
    newElement->content = content;

    return newElement;
}

elementList * getElement_el(elementList *el, int key) {
    elementList *pel;

    if (IS_PTR_NULL(el))
        return NULL;

    for (pel = el; pel != NULL; pel = pel->next) {
        if (pel->key == key)
            return pel;
    }
    return NULL;
}

int appendElement_el(elementList *el, elementList *next) {
    elementList *pel;

    if (IS_PTR_NULL(el))
        return -1;

    for (pel = el; pel != NULL; pel = pel->next) {
        if (pel->next == NULL) {
            pel->next = next;
            return 0;
        }
    }
    return 1;
}

void flushAll_el(elementList *el) {
    elementList *pel;
    elementList *nPel;

    el->next = NULL;
    for (pel = el->next; pel != NULL;) {
        nPel = pel->next;
        RELEASE_PTR(pel);
        pel = nPel;
    }
}

int reset_el(elementList *el) {
    if (IS_PTR_NULL(el))
        return -1;
    el->next = NULL;
}


/**************************************
 * params_t Operation function define *
 **************************************/

 /*
  * append node to tail of list
  */
params_t * buildParam(void *arg) {
    params_t *ret;
   
    ret  = (params_t *)malloc(sizeof(params_t));    
    memset(ret, 0, sizeof(params_t));
    ret->param = arg; 

    return ret;
}

params_t * paramListAppend(params_t *head, params_t *pl) {
    if (head == NULL || pl == NULL) {
        return NULL;
    }

    while (head->next != NULL) {
        head = head->next;
    }
    head->next = pl;
    return pl;
}

/*
 * Get node from head fo list
 * after that the head will be
 * removed from list.
 */
params_t * paramListGet(params_t **head) {
    params_t *ret;

    if (IS_PTR_NULL(head) || IS_PTR_NULL(*head)) {
        return NULL;
    }
    
    ret = *head;
    *head = *head->next;

    return ret;
}

/*****************************************************
 * symbol_t and symbolTabl operation function define *
 *****************************************************/
int symbolModuleAdd(symbolTable *stbl, symbolTable *new) {
    int ret = ok;

    
    if (IS_PTR_NULL(stbl) || IS_PTR_NULL(modName)) {
        ret = -1;
        return ret;
    }

    while (stbl->next != NULL) {
        stbl = stbl->next;
    }
    stbl->next = new;
    return ret;
}

int symbolAdd(char *modName, symbol_t *newSym) {
    int ret;
    symbolTable *pSymTbl = &symTable;
    symbol_t pSym;
    
    if (IS_PTR_NULL(modName) || IS_PTR_NULL(newSym)) {
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

    if (IS_PTR_NULL(symIdent)) {
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

