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
    gStack->buffer = (unsigned char *)malloc(bufferSize * unitSize);
    gStack->base = gStack->buffer + bufferSize * unitSize;
    return 0;
}

int push(genericStack *gStack, void *unit) {
    if (isNullPtr(gStack) || isNullPtr(unit))
        return -1;

    pushByIndex(gStack->base, unit, gStack->top, gStack->bufferSize, gStack->unitSize);
    return 0;
}

int pop(genericStack *gStack, void *unit) {
    if (isNullPtr(gStack) || isNullPtr(unit))
        return -1;

    popByIndex(gStack->base, gStack->top, unit, gStack->unitSize);
    return 0;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

int stack__STACK_PUSH_POP() {
    int ret, value = 4, idx = 0;
    genericStack gStack, *pStack = &gStack;
    genericStackConstruct(pStack, 10, sizeof(int));
    
    while (idx < 10) {
        push(pStack, &value);
        idx++;
    }
    
    idx = 0;
    while (idx < 10) { 
        pop(pStack, &ret);
        idx++;
    }

    assert_int_equal(pStack->top , 0);
    assert_int_equal(ret, 4);
}

#endif /* MIB2DOC_UNIT_TESTING */


