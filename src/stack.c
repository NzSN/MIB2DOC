//
// Created by ayden on 2017/4/24.
//

#include "stack.h"
#include "type.h"

int push(genericStack *ps, void *ident) {
    int top;
    if (isNullPtr(ps) || isNullPtr(ident))
        return -1;
    top = ps->top;
    pushByIndex(ps->stack, ident, top, SIZE_OF_IDENT_STACK-1);
    return 0;
}

void *pop(genericStack *ps) {
    int top;
    void *retVal;

    if (isNullPtr(ps))
        return NULL;

    top = ps->top;
    popByIndex(ps->stack, top, retVal);
    return retVal;
}

