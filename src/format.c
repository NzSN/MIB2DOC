/* format.c */

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "queue.h"
#include "format.h"
#include "util.h"
#include "mibTreeObjTree.h"

/* Latex format */
#define laTexStrBufferLen 25
static char *laTexStrBuffer;
static unsigned laTexStrBuffLenAcc = 0;
mibNodeInfoQueue infoQueue;

enum {
    section = 1,
    subsection = 2,
    subsubsection = 3,
    paragraph = 4,
    subparagraph = 5
};

enum {
    TABLE = 1,
    COLLECTING,
    SECTION
};

static int latexHeader(FILE *file);
static int latexTail(FILE *file);
static int latexSection(char *secName, char *oid, FILE *file, formatInfo *info);
static int latexTable(mibNodeInfoQueue *queue, char *parent, FILE *file);
static char * latexTableItem(tableInfo *info, int index);
static char * long2Short(char *str);
static int infoPacket(tableInfo *info, mibObjectTreeNode *node);
static int latexGen(void *arg, mibObjectTreeNode *node);
static int makeDecision(mibObjectTreeNode *node);

int docGenInLatex(mibObjectTreeNode *from, FILE *file, formatInfo *info) {
    if (isNullPtr(from) || isNullPtr(file) || isNullPtr(info))
        return ERROR;

    void *args[2] = { file, info };

    latexHeader(file);

    travel_MibTree(from, latexGen, args);

    latexTail(file);

    return OK;
}

static int latexGen(void *arg, mibObjectTreeNode *node) {
    if (isNullPtr(arg) || isNullPtr(node))
        return ERROR;

    void **args = arg;
    FILE *file = args[0];
    formatInfo *fInfo = args[1];

    tableInfo *info;
    char *parent, *oid, *secName;

    switch(makeDecision(node)) {
        case TABLE:
            info = (tableInfo *)Malloc(sizeof(tableInfo));
            infoPacket(info, node);

            parent = getIdentFromInfo(node->parent);
            if (isMibNodeType_ENTRY(node->parent))
                parent = getIdentFromInfo(node->parent->parent);

            appendQueue(&infoQueue, info);
            latexTable(&infoQueue, parent, file);
            break;

        case COLLECTING:
            info = (tableInfo *)Malloc(sizeof(tableInfo));
            infoPacket(info, node);
            appendQueue(&infoQueue, info);
            break;
        case SECTION:
            secName = getIdentFromInfo(node);
            oid = getOidFromInfo(node);
            latexSection(secName, oid, file, fInfo);
            break;
        default:
            break; 
    }

    return OK;
}

static int makeDecision(mibObjectTreeNode *node) {
    int decision;

    _Bool isNeedCollecting =
        /* If sibling is exists and it's not a table node */
        (node->sibling != null && !isMibNodeType_TABLE(node->sibling)) ||
        /* If it's an entry node */
        isMibNodeType_ENTRY(node);

    /* Action of SECTION: Create section imediatly.
     * Action of COLLECTING: Temporarily store node info.
     * Action of TABLE: Store info of current node and print out
     *                  infos that stored in buffer. */
    if (node->isNode || isMibNodeType_TABLE(node)) {
        decision = SECTION;
    } else if (isNeedCollecting) {
        decision = COLLECTING;
    } else {
        decision = TABLE;
    }

    return decision;
}

static int latexHeader(FILE *file) {
    if (isNullPtr(file))
       return ERROR;

    fprintf(file, "\\documentclass{ctexart}\n"
                  "\\usepackage{float}\n"
                  "\\usepackage{longtable}\n"
                  "\\begin{document}\n");
    fprintf(file, "\n\n\n");

    return OK;
}

static int latexTail(FILE *file) {
    if (isNullPtr(file))
        return ERROR;

    fprintf(file, "\\end{document}\n");

    return OK;
}

static int latexSection(char *secName, char *oid, FILE *file, formatInfo *info) {
    int depth;
    char *prefix;

    if (isNullPtr(secName) || isNullPtr(oid) || isNullPtr(file))
       return ERROR;

    depth = (strlen(oid) - info->beginOid) / 2 + 1;

    switch(depth) {
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
    fprintf(file, "\\%s {%s (%s)}.\n", prefix, secName, oid);

    return OK;
}

static int latexTable(mibNodeInfoQueue *queue, char *parent, FILE *file) {
    int i, count, index;

    if (isNullPtr(queue) || isNullPtr(parent) || isNullPtr(file))
       return ERROR;

    index = 1;
    count = queue->count;

    fprintf(file, "% Table Begin\n"
                  "\\begin{center}\n"
                  "\\begin{longtable}{|l|l|l|l|l|l|l|}\n");
    fprintf(file, "\\caption{%s}\\\\ \n", parent);
    // endfirst
    fprintf(file, "\\hline\n"
                  "\\multicolumn{1}{|c|}{\\textbf{Index}} & "
                  "\\multicolumn{1}{c|}{\\textbf{Name}} & "
                  "\\multicolumn{1}{c|}{\\textbf{Oid}} & "
                  "\\multicolumn{1}{c|}{\\textbf{RW}} & "
                  "\\multicolumn{1}{c|}{\\textbf{Type}} & "
                  "\\multicolumn{1}{|c|}{\\textbf{Detail}} \\\\ \n"
                  "\\hline\n"
                  "\\endfirsthead\n");

    // endhead
    fprintf(file, "\\hline\n"
                  "\\multicolumn{6}{|c|}{%s}\\\\ \n"
                  "\\hline\n"
                  "\\multicolumn{1}{|c|}{\\textbf{Index}} & "
                  "\\multicolumn{1}{c|}{\\textbf{Name}} & "
                  "\\multicolumn{1}{c|}{\\textbf{Oid}} & "
                  "\\multicolumn{1}{c|}{\\textbf{RW}} & "
                  "\\multicolumn{1}{c|}{\\textbf{Type}} & "
                  "\\multicolumn{1}{|c|}{\\textbf{Detail}} \\\\ \n"
                  "\\hline\n"
                  "\\endhead\n", parent);

    // endfoot
    fprintf(file, "\\hline\n"
                  "\\multicolumn{6}{|c|}{Continue to next} \\\\ \n"
                  "\\hline\n"
                  "\\endfoot\n");

    // endlastfoot
    fprintf(file, "\\hline\n"
                  "\\multicolumn{6}{|c|}{Last page of table} \\\\"
                  "\\hline\n"
                  "\\endlastfoot\n\n");

    tableInfo *info;
    for (i = 0; i < count; ++i, ++index) {
        info = (tableInfo *)getQueue(queue);
        fprintf(file, "%s \\\\ \n", latexTableItem(info, index));
        fprintf(file, "\\hline\n");
    }

    fprintf(file, "\\end{longtable}\n"
                  "\\end{center}\n");
    fprintf(file, "% Table End\n");

    return OK;
}

static char * latexTableItem(tableInfo *info, int index) {
    int isNeedExpand = FALSE, strLen;
    char *format = "%d & %s & %s & %s & %s & %s";

    if (isNullPtr(info) || index < 0)
       return null;

    strLen = info->length + 3 + 3 * 6 + 1;

    if (laTexStrBuffLenAcc < strLen) {
       isNeedExpand = TRUE;

        while (laTexStrBuffLenAcc < strLen)
            laTexStrBuffLenAcc += laTexStrBufferLen;
    }

    if (isNeedExpand) {
        if (!isNullPtr(laTexStrBuffer))
            RELEASE_MEM(laTexStrBuffer);
        laTexStrBuffer = (char *)Malloc(laTexStrBuffLenAcc);
    }

    sprintf(laTexStrBuffer,
            format, index,
            info->identifier,
            info->oid,
            info->rw, info->type, " ");

    return laTexStrBuffer;
}

static char * long2Short(char *str) {
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

static int infoPacket(tableInfo *info, mibObjectTreeNode *node) {
    if (isNullPtr(info) || isNullPtr(node))
        return ERROR;

    info->identifier = getIdentFromInfo(node);
    info->length += strlen(info->identifier);

    info->oid = getOidFromInfo(node);
    info->length += strlen(info->oid);

    if (node->isNode) return OK;

    info->desc = ((mibLeaveInfo *)(node->info))->desc;
    if (info->desc)
        info->length += strlen(info->desc);

    info->type = long2Short(((mibLeaveInfo *)(node->info))->type);
    info->length += strlen(info->type);

    info->rw = ((mibLeaveInfo *)(node->info))->rw;
    info->length += strlen(info->rw);

    return OK;
}

/* Plain text format */

