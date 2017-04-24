//
// Created by ayden on 2017/4/19.
//

#include <mem.h>
#include "../include/type.h"
#include "../include/queue.h"
#include "../include/tree.h"
#include "../include/stack.h"

/* Declaration */
static int sectionLatex(char *secName, char *OID, FILE *writeTo);

extern Queue dataQueue;
char currentTable[64];
extern mibObjectTreeNode root;

void deal_with(int type) {
    switch (type) {
        case OBJECT:
            break;
        case TRAP:
            break;
        case OBJECT_IDENTIFIER:
            break;
        case SEQUENCE:
            break;
        default:
            break;
    }
}

void deal_with_object(FILE *writeTo) {
    char *ident, *type, *rw, *desc, *mount, *suffix;
    identStack stack;
    mibObjectTreeNode *parent;
    memset(&stack, 0, sizeof(&stack));

    ident = getQueue(&dataQueue);
    type = getQueue(&dataQueue);
    rw = getQueue(&dataQueue);
    desc = getQueue(&dataQueue);
    mount = getQueue(&dataQueue);
    suffix = getQueue(&dataQueue);

    if (strlen(currentTable) == 0) {
        /* no table is processing, need to build table */
        push(&stack, mount);
        while (parent = getParent_mt(&root, mount)) {
            push(&stack, parent->ident);
            push(&stack, parent->oid);
        }
        while (IS_STACK_EMPTY(&stack)) {
            sectionLatex(pop(&stack), pop(&stack), writeTo);
        }
    }

    /* A node belong to the table we are processing */
    if (strncmp(currentTable, mount, SIZE_OF_CURRENT_TABLE) == 0) {

    } else {
        /* Need to build a new table */

    }
}
void deal_with_trap() {}
void deal_with_objIdent() {}
void deal_with_sequence() {}


static int tablePrint(char *ident, char *oid, char *rw, char *detail, FILE *output) {
    if (IS_PTR_NULL(ident) || IS_PTR_NULL(oid) ||
        IS_PTR_NULL(rw) || IS_PTR_NULL(detail))
        return -1;
}

static int sectionLatex(char *secName, char *OID, FILE *writeTo) {
    fprintf(writeTo, "\\section*{%s (%s)}\n", secName, OID);
}