/* util.c */

#include "util.h"

int stringToIdentList(char *str, identList *list, char seperator) {
    int index = 0;
    int index_ = 0;
    int beginPoint[256] = {0};
    identList *listTmp;

    if (IS_PTR_NULL(str) || IS_PTR_NULL(list)) {
        return error_null_reference;
    }

    beginPoint[index_++] = 0;
    while (str[index] != null) {
        if (str[index] == seperator) {
            str[index] = null;
            if (str[index+1] != null) {
                beginPoint[index_++] = index+1;
            } else {
                break;
            }
        }
    }

    index = 0;
    while (index_--) {
        listTmp = (identList *)malloc(sizeof(identList));
        memset(listTmp, null, sizeof(identList));
        listTmp->symName = (char *)malloc(strlen(str+beginPoint[index]));
        memset(listTmp->symName, null, strlen(str+beginPoint[index]));
        strncpy(listTmp->symName, str+beginPoint[index], strlen(str+beginPoint[index]));
        list->next.next = &listTmp->next;
        index++;
    }

    return ok;
}

/* util.c */
