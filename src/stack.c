//
// Created by ayden on 2017/4/24.
//

#include "stack.h"
#include "type.h"

int push(identStack *ps, void *ident) {
    int top;
    if (IS_PTR_NULL(ps) || IS_PTR_NULL(ident))
        return -1;
    top = ps->top
    pushByIndex(ps->stack, ident, top, SIZE_OF_IDENT_STACK-1)
    return 0;
}

void *pop(identStack *ps) {
    int top;
    void *retVal;

    if (IS_PTR_NULL(ps))
        return NULL;

    top = ps->top;
    popByIndex(ps->stack, top, retVal);
    return retVal;
}
