/* util.c */

#include <stdlib.h>
#include "util.h"

int stringToIdentList(char *str, hashMap *symbolMap, char seperator) {
    enum { identRecognize, identTracking };
    int index = 0, index_ = 0, flag = identRecognize,
        beginPoint[256] = { 0 };
    char *strCopy = null;
    identList *listTmp;

    if (isNullPtr(str) || isNullPtr(symbolMap)) {
        return ERROR_NULL_REF;
    }

    strCopy = strdup(str);
    if (strCopy == null) {
        return -1;
    }

    beginPoint[index_++] = 0;
    while (strCopy[index] != null) {
        if (flag == identRecognize && strCopy[index] == seperator) {
            strCopy[index] = null;
            flag = identTracking;
            index++;
        } else if (flag == identTracking && strCopy[index] == cSpace) {
            index++;
        } else {
            beginPoint[index_++] = index;
            flag = identRecognize;
        }
    }

    index = 0;
    while (index_-- > 0) {
        /*
        listTmp = (identList *)malloc(sizeof(identList));
        memset(listTmp, null, sizeof(identList));
        listTmp->symName = &strCopy[beginPoint[index]];
        list->node.next = &listTmp->node;
        index++;
        */
    }

    return ERROR_NONE;
}

int tableRecognize(char *buffer, int size) {
    int ret, index;
    char current;

S_init:
    index = -1;
    ret = -1;
    goto S_0;

S_0:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'T')
        goto S_1;
    else
        goto S_0;

S_1:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'a')
        goto S_2;
    else
        goto S_0;

S_2:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'b')
        goto S_3;
    else
        goto S_0;

S_3:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'l')
        goto S_4;
    else
        goto S_0;

S_4:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'e')
        goto S_accept;
    else
        goto S_0;

S_accept:
    ret = 1;
    goto S_finished;

S_out:
    ret = 0;

S_finished:
    return ret;
}

int entryRecognize(char *buffer, int size) {
    int ret, index;
    char current;

S_init:
    index = -1;
    ret = -1;
    goto S_0;

S_0:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'E')
        goto S_1;
    else
        goto S_0;

S_1:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'n')
        goto S_2;
    else
        goto S_0;

S_2:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 't')
        goto S_3;
    else
        goto S_0;

S_3:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'r')
        goto S_4;
    else
        goto S_0;

S_4:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'y')
        goto S_accept;
    else
        goto S_0;

S_accept:
    ret = 1;
    goto S_finished;

S_out:
    ret = 0;

S_finished:
    return ret;
}

char * randStr(int length) {
    if (length <= 0)
        return NULL;

    char *str = (char *)malloc(length);
    int begin = 'a', end = 'z', idx = 0;
    int randNum, distance = end - begin;

    while (idx < length) {
        randNum = rand() % distance;
        randNum += begin;
        str[idx] = randNum;
        idx++;
    }
    return str;
}

char * numberToStr(int num) {
    char *str = (char *)malloc(20);
    memset(str, 0, 20);

    sprintf(str, "%d", num);
    return str;
}

void * Malloc(size_t size) {
    void *mem = malloc(size);
    if (isNullPtr(mem)) abort();

    return mem;
}

_Bool isStringEqual(const char *strLeft, const char *strRight) {
    if (isNullPtr(strLeft) || isNullPtr(strRight))
        return FALSE;

    size_t len = strlen(strLeft);
    _Bool isEqual;

    isEqual = len == strlen(strRight) && ! strncmp(strLeft, strRight, len);

    return isEqual;
}

/* util.c */

