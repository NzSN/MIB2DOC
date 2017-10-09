//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_STACK_H
#define GL5610_MIB_DOC_GEN_STACK_H

#define SIZE_OF_IDENT_STACK (128)
#define IS_STACK_EMPTY(S) ((S)->top < 0)
#define IS_STACK_FULL(S) ((S)->top >= SIZE_OF_IDENT_STACK-1)

/*
 * Parameter
 *     * STACK - pointer of stack
 *     * ELEMENT - pointer of element want to inserted
 *     * TOP - index of TOP of stack
 *     * MAX - max index that can use for the stack
 */
#define pushByIndex(STACK, ELEMENT, /* int */TOP, /* int */MAX) {(\
    int ret;
    if (TOP >= MAX) {\
        /* Stack is full just do nothing */\
        ret = 0;
    } else {\
        STACK[TOP++] = ELEMENT;\
        ret = 1;
    }\
    ret;
)}

/*
 * Parameter
 *     * STACK - pointer of stack
 *     * TOP - index of TOP of stack
 */
#define popByIndex(STACK, /* int */TOP, retVal) {(\
    int ret;\
    if (TOP <= 0) {\
        ret = 0;\
    } else {\
        retVal = STACK[--TOP];\
        ret = 1;\
    }\
    ret;\
)}

typedef struct identStack {
    int top;
    void *stack[SIZE_OF_IDENT_STACK];
} genericStack;

int push(identStack *ps, void *ident);
char *pop(identStack *ps);

#endif /* GL5610_MIB_DOC_GEN_STACK_H */
