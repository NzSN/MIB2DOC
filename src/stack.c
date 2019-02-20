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

    if (pushByIndex(gStack->base, unit, gStack->top, gStack->bufferSize, gStack->unitSize) == 0)
        return -1;
    return 0;
}

int pop(genericStack *gStack, void *unit) {
    if (isNullPtr(gStack) || isNullPtr(unit))
        return -1;

    if (popByIndex(gStack->base, gStack->top, unit, gStack->unitSize) == 0)
        return -1;
    return 0;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

int push_wrapper(genericStack *stack, int range) {
    int idx = 0;
    while (idx < range) {
        if (push(stack, &idx) == -1) {
            return -1; 
        }
        idx++;
    }

    return 0;
}

int pop_wrapper(genericStack *stack, int range) {
    int idx = 0, ret;
    while (idx < range) {
        if (pop(stack, &ret) == -1) {
            return -1; 
        } 
        assert_int_equal(ret, 10 - idx - 1);
        idx++;
    }
    return 0;
}

int stack__STACK_PUSH_POP() {
    int ret, value = 4, idx = 0;
    genericStack gStack, *pStack = &gStack;
    genericStackConstruct(pStack, 10, sizeof(int));
    
    // push & pop in normal range. 
    while (idx < 100) {
        push_wrapper(pStack, 10); 
        pop_wrapper(pStack, 10); 
        idx++;
    }

    assert_int_equal(pStack->top , 0);

    // push overflow testing 
    ret = push_wrapper(pStack, 11);  
    assert_int_equal(ret, -1); 
    // pop overflow testing
    ret = pop_wrapper(pStack, 11);
    assert_int_equal(ret, -1); 

    assert_int_equal(pStack->top, 0);
}

#endif /* MIB2DOC_UNIT_TESTING */


