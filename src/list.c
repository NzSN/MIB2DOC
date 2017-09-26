//
// Created by ayden on 17-5-1.
//

#include <malloc.h>
#include <string.h>
#include "list.h"
#include "type.h"

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


/* Operation for params_t */
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

params_t * paramListGet(params_t **head) {
    params_t *ret;

    if (head == NULL || *head == NULL) {
        return NULL;
    }
    
    ret = *head;
    *head = *head->next;

    return ret;
}