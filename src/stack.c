//
// Created by ayden on 2017/4/24.
//

#include "stack.h"
#include "type.h"

int genericStackConstruct(genericStack *gStack, int bufferSize, int unitSize) {
    if (isNullPtr(gStack)) {
        return -1;
    }
    gStack->top = 0;
    gStack->unitSize = unitSize;
    gStack->bufferSize = bufferSize;
    gStack->buffer = (unsigned char *)malloc(size);
    gStack->base = gStack->buffer + bufferSize;
    return 0;
}

int push(genericStack *gStack, void *unit) {
    if (isNullPtr(gStack) || isNullPtr(unit))
        return -1;

    pushByIndex(gStack->base, unit, gStack->top, 
            gStack->bufferSize-1, gStack->unitSize);
    return 0;
}

int pop(genericStack *gStack, void *unit) {
    int top;

    if (isNullPtr(ps))
        return -1;

    top = ps->top;
    popByIndex(gStack->base, gStack->top, unit, gStack->unitSize);
    return 0;
}

