//
// Created by ayden on 2017/4/19.
//

#include <mem.h>
#include <tgmath.h>
#include "../include/type.h"
#include "../include/queue.h"
#include "../include/tree.h"
#include "../include/stack.h"

/* Declaration */
static int sectionLatex(char *secName, char *OID, int depth, FILE *writeTo);

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

    ident = getQueue(&dataQueue);
    type = getQueue(&dataQueue);
    rw = getQueue(&dataQueue);
    desc = getQueue(&dataQueue);
    mount = getQueue(&dataQueue);
    suffix = getQueue(&dataQueue);

    if (strlen(currentTable) == 0) {
        /* no table is processing, need to build table */
        sectionHandler(mount, writeTo);
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

static int sectionHandler(char *mount, FILE *writeTo) {
    int depth = 1;
    identStack stack;
    mibObjectTreeNode *parent;
    memset(&stack, 0, sizeof(&stack));

    push(&stack, mount);
    /*
     * First, put every section name into stack use to print later
     */
    while (parent = getParent_mt(&root, mount)) {
        push(&stack, parent->ident);
        push(&stack, parent->oid);
    }
    /* Second, print section */
    while (IS_STACK_EMPTY(&stack)) {
        sectionLatex(pop(&stack), pop(&stack), depth, writeTo);
        depth++;
    }
}

static int sectionLatex(char *secName, char *OID, int depth, FILE *writeTo) {
    enum {
        section = 1,
        subsection,
        subsubsection,
        paragraph,
        subparagraph
    };
    char *prefix;
    int i;

    for (i=0; i < SIZE_OF_SECTION_RECORD; i++) {
        if (strncmp(sectionRecord[i], secName, sizeof(secName)) == 0)
            return 0;
        if (sectionRecord[i] == NULL)
            sectionRecord[i] = secName;
    }

    /*
     * Pending casue I should define a custome section command
     * in laTex
     */
    switch (depth) {
        case section:
            break;
        case subsection:
            break;
        case subsubsection:
            break;
        case paragraph:
            break;
        case subsubsection:
            break;
        default:
            break;
    }
    fprintf(writeTo, "\\%s {%s (%s)}\n", prefix, secName, OID);

    return 0;
}

static int tableHandler(char *name, char *oid, char *rw, char *type, char *desc) {

}