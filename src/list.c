//
// Created by ayden on 17-5-1.
//

#include <malloc.h>
#include <string.h>
#include "../include/list.h"
#include "../include/type.h"

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