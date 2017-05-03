//
// Created by ayden on 2017/5/3.
//

#include <stdio.h>
#include <mem.h>
#include <malloc.h>
#include "../include/mibTreeObjTree.h"
#include "../include/docGenerate.h"
#include "../include/queue.h"

/* Defines */
#define SIZE_OF_LATEX_BUFFER 256

/* Global */
char *beginFrom;
int beginOid;

/* enumeration */
enum {
    TABLE = 1,
    COLLECTING,
    SECTION
};

/* Local */
char *laTexStrBuffer;
mibNodeInfoQueue infoQueue;
static int makeDecision(mibObjectTreeNode *node);
static int sectionLatex(char *secName, char *OID, FILE *writeTo);
static int tableLaTex(mibNodeInfoQueue *info, char *parent, FILE *writeTo);
static char * genTableItemLaTex(tableInfo *info, int index);
static int infoPacket(tableInfo *info, mibObjectTreeNode *node);

int docGenerate(void *arg, mibObjectTreeNode *node) {
    FILE *writeTo;
    char *secname, *oid, *parent;
    tableInfo info;
    mibObjectTreeNode *pNode;

    if (!beginFrom)
        return -1;

    /* Initialization */
    writeTo = (FILE *)arg;
    pNode = search_mot(&mibObjectTreeRoot, beginFrom);
    beginOid = strlen(getOidFromInfo(pNode));
    laTexStrBuffer = (char *)malloc(SIZE_OF_LATEX_BUFFER);

    switch(makeDecision(node)) {
        case TABLE:
            tableLaTex(&infoQueue, parent, writeTo);
            break;
        case COLLECTING:
            if (!parent)
                parent = getIdentFromInfo(node->parent);
            infoPacket(&info, node);
            appendQueue(&infoQueue, &info);
            break;
        case SECTION:
            secname = getIdentFromInfo(node);
            oid = getOidFromInfo(node);
            sectionLatex(secname, oid, writeTo);
            break;
        default:
            break;
    }

    return 0;
}

static int makeDecision(mibObjectTreeNode *node) {

}

static int infoPacket(tableInfo *info, mibObjectTreeNode *node) {
    info->identifier = getIdentFromInfo(node);
    info->oid = getOidFromInfo(node);

    if (node->isNode)
        return 1;

    info->desc = ((mibLeaveInfo *)(node->info))->desc;
    info->type = ((mibLeaveInfo *)(node->info))->type;
    info->rw = ((mibLeaveInfo *)(node->info))->rw;

    return 0;
}

static int sectionLatex(char *secName, char *OID, FILE *writeTo) {
    enum {
        section = 1,
        subsection = 2 ,
        subsubsection = 3,
        subsubsubsection = 4,
        paragraph = 5,
        subparagraph = 6
    };

    int depth;
    char *prefix;

    depth = (strlen(OID) - beginOid) / 2;

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
        case subsubsubsection:
            prefix = "subsubsubsection";
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

    fprintf(writeTo, "\\%s {%s (%s)}\n", prefix, secName, OID);

    return 0;
}

static int tableLaTex(mibNodeInfoQueue *queue, char *parent, FILE *writeTo) {
    int i, count, index;

    if (IS_PTR_NULL(queue) || IS_PTR_NULL(writeTo))
        return -1;

    count = queue->count;
    index = 0;

    fprintf(writeTo, "\\begin{table}[!hbp]\n"
                     "\\newcommand{\\tabincell}[2]{\\begin{tabular}{@{}#1@{}}#2\\end{tabular}}\n"
                     "\\centerline {\n"
                     "\\begin{tabular} {|c|c|c|c|c|c|c|}\n"
                     "\\hline\n"
                     "Index & Name & Desc & OID & RW & Type & Detail \\\\ \n");
    for (i=0; i<count; i++) {
        fprintf(writeTo, "%s\n", genTableItemLaTex((tableInfo *)getQueue(queue), index));
        fprintf(writeTo, "\\hline\n");
    }

    fprintf(writeTo, "\\end{tabular}\n");
    fprintf(writeTo, "}\n");
    fprintf(writeTo, "\\caption{%s}\n", parent);
    fprintf(writeTo, "\\end{table}\n");

    parent = NULL;
    return 0;
}

static char * genTableItemLaTex(tableInfo *info, int index) {

    if (IS_PTR_NULL(info))
        return NULL;

    memset(laTexStrBuffer, 0, SIZE_OF_LATEX_BUFFER);
    sprintf(laTexStrBuffer, "%d & %s & %s & %s & %s & %s & %s",
            index, info->identifier, " ", info->oid, info->rw, info->type, " ");

    return laTexStrBuffer;
}