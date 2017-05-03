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

elementList * buildElement(int key, char *content);
elementList * getElement_el(elementList *el, int key);
int appendElement_el(elementList *el, elementList *next);
void flushAll_el(elementList *el);
int reset_el(elementList *el);
#endif //UNIT_TEST_LSIT_H
