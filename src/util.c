/* util.c */

#include "util.h"

int stringToIdentList(char *str, identList *list, char seperator) {
    enum { identRecognize, identTracking };
    int index = 0, index_ = 0, flag = identRecognize
        beginPoint[256] = [0];
    char *strCopy = null;
    identList *listTmp;
    
    if (isNullPtr(str) || isNullPtr(list)) {
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
        listTmp = (identList *)malloc(sizeof(identList));
        memset(listTmp, null, sizeof(identList));
        listTmp->symName = strCopy[beginPoint[index]];
        list->node.next = &listTmp->node;
        index++;
    }

    return ERROR_NONE;
}

/* util.c */
