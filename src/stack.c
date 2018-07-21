//
// Created by ayden on 2017/4/24.
//

#include "stack.h"
#include "type.h"

int genericStackConstruct(genericStack *gStack, int bufferSize, int unitSize) {
    if (isNullPtr(gStack)) {
        return -1;
    }
    gStack->unitSize = unitSize;
    gStack->bufferSize = bufferSize;
    gStack->buffer = (unsigned char *)malloc(bufferSize);
    gStack->base = gStack->buffer;
    gStack->top = gStack->buffer;
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

    if (isNullPtr(gStack) || isNullPtr(unit))
        return -1;

    popByIndex(gStack->base, gStack->top, unit, gStack->unitSize);
    
    return 0;
}


