//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_STACK_H
#define GL5610_MIB_DOC_GEN_STACK_H

#define SIZE_OF_IDENT_STACK (128)
#define IS_STACK_EMPTY(S) ((S)->top == 0)
#define IS_STACK_FULL(S) ((S)->top > SIZE_OF_IDENT_STACK-2)

typedef struct identStack {
    int top;
    void *stack[SIZE_OF_IDENT_STACK];
} genericStack;

int push(identStack *ps, char *ident);
char *pop(identStack *ps);

#endif /* GL5610_MIB_DOC_GEN_STACK_H */
