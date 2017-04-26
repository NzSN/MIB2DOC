//
// Created by ayden on 2017/4/24.
//

#include "../include/stack.h"
#include "../include/type.h"

int push(identStack *ps, char *ident) {
    int top;
    if (IS_PTR_NULL(ps) || IS_PTR_NULL(ident))
        return -1;
    if (IS_STACK_FULL(ps))
        return -2;
    top = ps->top;
    top++;
    ps->stack[top] = ident;
    ps->top = top;
    return 0;
}

char *pop(identStack *ps) {
    int top;
    char *ident;

    if (IS_PTR_NULL(ps))
        return NULL;
    
    top = ps->top;
    
    if (IS_STACK_EMPTY(ps))
        return NULL;
    else
        ident = ps->stack[top];
    top--;
    ps->top = top;
    return ident;
}
