//
// Created by ayden on 17-5-2.
//

#include <malloc.h>
#include <string.h>
#include "../include/lexDeal.h"
#include "../include/type.h"

char * strDuplicate(char *src) {
    size_t size;
    char *dst;

    if (IS_PTR_NULL(src))
        return NULL;

    size = strlen(src)+1;
    dst = (char *)malloc(size);
    memset(dst, 0, size);
    strncpy(dst, src, size);

    return dst;
}