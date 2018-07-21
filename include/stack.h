//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_STACK_H
#define GL5610_MIB_DOC_GEN_STACK_H

#define IS_STACK_EMPTY(S) ((S)->top < 0)
#define IS_STACK_FULL(S) ((S)->top >= SIZE_OF_IDENT_STACK-1)

/*
 * Parameter
 *     * STACK - pointer of stack
 *     * ELEMENT - pointer of element want to inserted
 *     * TOP - index of TOP of stack
 *     * MAX - max index that can use for the stack
 */
#define pushByIndex(STACK_BASE, ELEMENT, /* int */TOP, /* int */BUFFER_SIZE, /* int */UNIT_SIZE) ({\
    int ret;\
    if (TOP >= STACK_BASE+BUFFER_SIZE || TOP+UNIT_SIZE >= STACK_BASE+BUFFER_SIZE) {\
        /* Stack is full just do nothing */\
        ret = 0;\
    } else {\
        TOP += UNIT_SIZE;\
        memcpy(STACK_BASE, ELEMENT, UNIT_SIZE); \
        ret = 1;\
    }\
    ret;\
})

/*
 * Parameter
 *     * STACK - pointer of stack
 *     * TOP - index of TOP of stack
 */
#define popByIndex(STACK_BASE, /* int */TOP, UNIT, UNIT_SIZE) ({\
    int ret;\
    if (TOP <= 0 || TOP-UNIT_SIZE <= 0) {\
        ret = 0;\
    } else {\
        memcpy(UNIT, TOP-UNIT_SIZE, UNIT_SIZE);\
        TOP -= UNIT_SIZE;\
        ret = 1;\
    }\
    ret;\
})

// Note: genericStack can only buffer
// only one class of objects, several class
// of objects buffer into one genericStack 
// is not supported.
typedef struct identStack {
    unsigned char *top;
    int bufferSize;
    int unitSize;
    unsigned char *base;
    unsigned char *buffer;
} genericStack;

int genericStackConstruct(genericStack *gStack, int bufferSize, int unitSize);
int push(genericStack *ps, void *unit);
int pop(genericStack *ps, void *unit);

#endif /* GL5610_MIB_DOC_GEN_STACK_H */
