//
// Created by ayden on 2017/5/3.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "mibTreeObjTree.h"
#include "docGenerate.h"
#include "queue.h"
#include "util.h"
#include "type.h"

#include "mibTreeGen.h"
#include "typeTable.h"

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
static int docGenerate(void *arg, mibObjectTreeNode *root);

int documentGen(mibTreeHead *treeHead, FILE *writeTo) {
    
    mibTree *tree; 
    
    if (isNullPtr(treeHead) || isNullPtr(writeTo))
        return ERROR;

    // Check that is only one tree here
    assert(treeHead->numOfTree == 1);    

    tree = mibTreeHeadFirstTree(treeHead); 

    latexHeaderGen(); 

    travel_MibTree(tree->root, docGenerate, writeTo);

    latexTailGen();

    return 0;
}

static int docGenerate(void *arg, mibObjectTreeNode *node) {
    FILE *writeTo;
    char *secname, *oid, *parent;
    tableInfo *info;
    mibObjectTreeNode *pNode;
    mibNodeInfoQueue *pQueue = &infoQueue;
    
    if (isNullPtr(beginFrom)) {      
        return 0;
    }

    writeTo = (FILE *) arg;

    pNode = search_MibTree(&mibObjectTreeRoot, beginFrom);
    if (isNullPtr(pNode)) 
        return -1; 
    beginOid = strlen(getOidFromInfo(pNode));
    laTexStrBuffer = (char *) malloc(SIZE_OF_LATEX_BUFFER);

    switch(makeDecision(node)) {
        case TABLE:
            info = (tableInfo *)malloc(sizeof(tableInfo));
            infoPacket(info, node);
            
            parent = getIdentFromInfo(node->parent);
            // fixme:Should be recognize a table via type of it
            // but not the name of it.
            if (isMibNodeType_ENTRY(node->parent))
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
