//
// Created by ayden on 2017/4/19.
//

#include <mem.h>
#include <malloc.h>
#include "../include/type.h"
#include "../include/queue.h"
#include "../include/mibTreeObjTree.h"
#include "../include/stack.h"

/* Global */
mibObjectTreeNode root;

/* Declaration */
static int sectionLatex(char *secName, char *OID, int depth, FILE *writeTo);
static int mibTreeLeaveAdd(char *ident, char *type, char *rw, char *desc, char *parent, char *suffix);
static int mibTreeNodeAdd(char *ident, char *oid, char *parent);

extern Queue dataQueue;
char currentTable[64];
extern mibObjectTreeNode root;

void deal_with(int type) {
    switch (type) {
        case OBJECT:
            deal_with_object();
            break;
        case TRAP:
            break;
        case OBJECT_IDENTIFIER:
            deal_with_objIdent();
            break;
        case SEQUENCE:
            break;
        default:
            break;
    }
}

int deal_with_object() {
    char *ident, *type, *rw, *desc, *parent, *suffix, *oid;

    ident = getQueue(&dataQueue);
    type = getQueue(&dataQueue);
    rw = getQueue(&dataQueue);
    desc = getQueue(&dataQueue);
    parent = getQueue(&dataQueue);
    suffix = getQueue(&dataQueue);

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(type) ||
            IS_PTR_NULL(rw) || IS_PTR_NULL(desc) ||
            IS_PTR_NULL(desc) || IS_PTR_NULL(parent) ||
            IS_PTR_NULL(suffix)) {
        return -1;
    }

    oid = oidComplement(parent, suffix);
    mibTreeLeaveAdd(ident, type, rw, desc, parent, oid);

    RELEASE_PTR(suffix);
    RELEASE_PTR(parent);
    return 0;
}

int deal_with_objIdent() {
    char *ident, *parent, *suffix, *oid;

    ident = getQueue(&dataQueue);
    parent = getQueue(&dataQueue);
    suffix = getQueue(&dataQueue);

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(parent) || IS_PTR_NULL(suffix))
        return -1;

    oid = oidComplement(parent, suffix);
    mibTreeNodeAdd(ident, oid, parent);

    RELEASE_PTR(suffix);
    RELEASE_PTR(parent);
    return 0;
}

void deal_with_trap() {
    char *ident, *parent, *suffix, *oid, *desc;

    ident = getQueue(&dataQueue);


}
void deal_with_sequence() {}

static int tablePrint(char *ident, char *oid, char *rw, char *detail, FILE *output) {
    if (IS_PTR_NULL(ident) || IS_PTR_NULL(oid) ||
        IS_PTR_NULL(rw) || IS_PTR_NULL(detail))
        return -1;
}

static int sectionLatex(char *secName, char *OID, int depth, FILE *writeTo) {
    enum {
        section = 1,
        subsection,
        subsubsection,
        subsubsubsection,
        paragraph,
        subparagraph
    };
    char *prefix;
    int i;

    for (i=0; i < SIZE_OF_SECTION_RECORD; i++) {
        if (strncmp(sectionRecord[i], secName, strlen(secName)) == 0)
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
        case subsubsubsection:
            break;
        case paragraph:
            break;
        default:
            break;
    }
    fprintf(writeTo, "\\%s {%s (%s)}\n", prefix, secName, OID);

    return 0;
}

static int mibTreeLeaveAdd(char *ident, char *type, char *rw, char *desc, char *parent, char *oid) {
    mibObjectTreeNode *obj;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(type) || IS_PTR_NULL(rw) ||
            IS_PTR_NULL(desc) || IS_PTR_NULL(parent) || IS_PTR_NULL(oid)) {
        return -1;
    }

    obj = mibLeaveBuild(ident, type, rw, desc, oid);
    if (IS_PTR_NULL(obj))
        return -1;

    insert_mot(mibObjectTreeRoot, obj, parent);

}

static int mibTreeNodeAdd(char *ident, char *oid, char *parent) {
    mibObjectTreeNode *obj;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(oid))
        return -1;

    obj = mibNodeBuild(ident, oid);
    if (IS_PTR_NULL(obj))
        return -1;

    insert_mot(mibObjectTreeRoot, obj, parent);
}

static char * oidComplement(char *parent, char *suffix) {
    char *oid;
    mibObjectTreeNode *parentNode;
    mibNodeInfo *nodeInfo;

    oid = (char *)malloc(SIZE_OF_OID_STRING);
    memset(oid, 0, SIZE_OF_OID_STRING);
    parentNode = search_mot(mibObjectTreeRoot, parent);

    if (parentNode == NULL)
        return NULL;
    nodeInfo = parentNode->info;

    strncpy(oid, nodeInfo->oid, SIZE_OF_OID_STRING);
    strncat(oid, suffix, strlen(suffix));

    return oid;
}