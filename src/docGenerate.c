//
// Created by ayden on 2017/5/3.
//

#include <stdio.h>
#include <memory.h>
#include <malloc.h>
#include "mibTreeObjTree.h"
#include "docGenerate.h"
#include "queue.h"

/* Defines */
#define SIZE_OF_LATEX_BUFFER 256

/* Global */
char *beginFrom;
int beginOid;
int isInitilized;
/* enumeration */
enum {
    TABLE = 1,
    COLLECTING,
    SECTION
};
extern mibObjectTreeNode mibObjectTreeRoot;

/* Local */
char *laTexStrBuffer;
mibNodeInfoQueue infoQueue;
static int makeDecision(mibObjectTreeNode *node);
static int sectionGen_Latex(char *secName, char *OID, FILE *writeTo);
static int tableGen_Latex(mibNodeInfoQueue *info, char *parent, FILE *writeTo);
static char * tableItemGen_Latex(tableInfo *info, int index);
static int infoPacket(tableInfo *info, mibObjectTreeNode *node);
char *long2Short(char *str);
static int latexHeaderGen();
static int latexTailGen();
int documentGen(mibObjectTreeNode *root, FILE *writeTo);
static int docGenerate(void *arg, mibObjectTreeNode *root);

/* static ** for test */ int tableRecognize(char *buffer, int size);
/* static ** for test */ int entryRecognize(char *buffer, int size);

int documentGen(mibObjectTreeNode *root, FILE *writeTo) {
    latexHeaderGen();
    travel_MibTree(root, docGenerate, writeTo);
    latexTailGen();

    return 0;
}

static int docGenerate(void *arg, mibObjectTreeNode *node) {
    FILE *writeTo;
    char *secname, *oid, *parent;
    tableInfo *info;
    mibObjectTreeNode *pNode;
    mibNodeInfoQueue *pQueue = &infoQueue;

    if (!beginFrom)
        return 0;

    writeTo = (FILE *) arg;
    pNode = search_MibTree(&mibObjectTreeRoot, beginFrom);
    beginOid = strlen(getOidFromInfo(pNode));
    laTexStrBuffer = (char *) malloc(SIZE_OF_LATEX_BUFFER);

    switch(makeDecision(node)) {
        case TABLE:
            info = (tableInfo *)malloc(sizeof(tableInfo));
            infoPacket(info, node);
            parent = getIdentFromInfo(node->parent);
            // fixme:Should be recognize a table via type of it
            // but not the name of it.
            if (entryRecognize(parent, strlen(parent)))
                parent = getIdentFromInfo(node->parent->parent);
            appendQueue(&infoQueue, info);
            tableGen_Latex(&infoQueue, parent, writeTo);
            break;
        case COLLECTING:
            info = (tableInfo *)malloc(sizeof(tableInfo));
            infoPacket(info, node);
            appendQueue(&infoQueue, info);
            break;
        case SECTION:
            secname = getIdentFromInfo(node);
            oid = getOidFromInfo(node);
            sectionGen_Latex(secname, oid, writeTo);
            break;
        default:
            break;
    }

    return 0;
}

// Generate contents before tables of mib nodes.
static int latexHeaderGen() {}

// Generate contents after tables of mib nodes.
static int latexTailGen() {}

static int makeDecision(mibObjectTreeNode *node) {
    int decision;
    char *ident = getIdentFromInfo(node);

    if (node->isNode || tableRecognize(ident, strlen(ident))) {
        decision = SECTION;
    } else if (node->sibling != NULL || entryRecognize(ident, strlen(ident))) {
        decision = COLLECTING;
    } else {
        decision = TABLE;
    }
    return decision;
}

static int infoPacket(tableInfo *info, mibObjectTreeNode *node) {
    info->identifier = getIdentFromInfo(node);
    info->oid = getOidFromInfo(node);

    if (node->isNode)
        return 1;

    info->desc = ((mibLeaveInfo *)(node->info))->desc;
    info->type = long2Short(((mibLeaveInfo *)(node->info))->type);
    info->rw = ((mibLeaveInfo *)(node->info))->rw;

    return 0;
}

static int sectionGen_Latex(char *secName, char *OID, FILE *writeTo) {
    enum {
        section = 1,
        subsection = 2,
        subsubsection = 3,
        paragraph = 4,
        subparagraph = 5
    };

    int depth;
    char *prefix;

    depth = (strlen(OID) - beginOid) / 2 + 1;

    switch (depth) {
        case section:
            prefix = "section";
            break;
        case subsection:
            prefix = "subsection";
            break;
        case subsubsection:
            prefix = "subsubsection";
            break;
        case paragraph:
            prefix = "paragraph";
            break;
        case subparagraph:
            prefix = "subparagraph";
            break;
        default:
            prefix = "subparagraph";
    }

    fprintf(writeTo, "\\%s {%s (%s)}.\n", prefix, secName, OID);
    return 0;
}

static int tableGen_Latex(mibNodeInfoQueue *queue, char *parent, FILE *writeTo) {
    int i, count, index;

    if (isNullPtr(queue) || isNullPtr(writeTo))
        return -1;

    count = queue->count;
    index = 1;

    fprintf(writeTo, "\\begin{table}[H]\n"
                     "\\centerline {\n"
                     "\\begin{tabular} {|c|c|c|c|c|c|c|}\n"
                     "\\hline\n"
                     "Index & Name & Desc & OID & RW & Type & Detail \\\\ \n"
                     "\\hline\n");
    for (i=0; i<count; i++, index++) {
        fprintf(writeTo, "%s \\\\\n", tableItemGen_Latex((tableInfo *)getQueue(queue), index));
        fprintf(writeTo, "\\hline\n");
    }

    fprintf(writeTo, "\\end{tabular}\n");
    fprintf(writeTo, "}\n");
    fprintf(writeTo, "\\caption{%s}\n", parent);
    fprintf(writeTo, "\\end{table}\n");

    parent = NULL;
    return 0;
}

static char * tableItemGen_Latex(tableInfo *info, int index) {

    if (isNullPtr(info))
        return NULL;

    memset(laTexStrBuffer, 0, SIZE_OF_LATEX_BUFFER);
    sprintf(laTexStrBuffer, "%d & %s & %s & %s & %s & %s & %s",
            index, info->identifier, info->desc, info->oid, info->rw, info->type, " ");

    return laTexStrBuffer;
}

static int tableRecognize(char *buffer, int size) {
    int ret, index;
    char current;

S_init:
    index = -1;
    ret = -1;
    goto S_0;

S_0:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'T')
        goto S_1;
    else
        goto S_0;

S_1:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'a')
        goto S_2;
    else
        goto S_0;

S_2:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'b')
        goto S_3;
    else
        goto S_0;

S_3:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'l')
        goto S_4;
    else
        goto S_0;

S_4:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'e')
        goto S_accept;
    else
        goto S_0;

S_accept:
    ret = 1;
    goto S_finished;

S_out:
    ret = 0;

S_finished:
    return ret;
}


static int entryRecognize(char *buffer, int size) {
    int ret, index;
    char current;

    S_init:
    index = -1;
    ret = -1;
    goto S_0;

    S_0:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'E')
        goto S_1;
    else
        goto S_0;

    S_1:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'n')
        goto S_2;
    else
        goto S_0;

    S_2:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 't')
        goto S_3;
    else
        goto S_0;

    S_3:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'r')
        goto S_4;
    else
        goto S_0;

    S_4:
    if (index < size)
        index++;
    else
        goto S_out;

    current = buffer[index];

    if (current == 'y')
        goto S_accept;
    else
        goto S_0;

    S_accept:
    ret = 1;
    goto S_finished;

    S_out:
    ret = 0;

    S_finished:
    return ret;
}

char * long2Short(char *str) {
    if (isNullPtr(str))
        return NULL;

    if (strncmp(str, "INTEGER", strlen("INTEGER")) == 0) {
        return "INT";
    }

    if (strncmp(str, "OCTET", strlen("OCTET")) == 0) {
        return "OCT";
    }

    if (entryRecognize(str, strlen(str))) {
        return "Entry";
    }

    return str;
}
