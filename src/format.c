/* format.c */

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "queue.h"
#include "format.h"
#include "util.h"

/* Latex format */
#define laTexStrBufferLen 25
static char *laTexStrBuffer;
static unsigned laTexStrBuffLenAcc = 0;

enum {
    section = 1,
    subsection = 2,
    subsubsection = 3,
    paragraph = 4,
    subparagraph = 5
};


int docGenInLatex(mibObjectTreeNode *from, char *filePath, formatInfo *info) {
    if (isNullPtr(from) || isNullPtr(filePath))
        return ERROR;
    
     
}

static int latexHeader(FILE *file) {
    if (isNullPtr(file))
       return ERROR; 

    fprintf(file, "\\documentclass{ctexart}\n",
                  "\\usepackage{float}\n",
                  "\\usepackage{longtable}\n",
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

}

static char * latexTableItem(tableInfo *info, int index) {
    int isNeedExpand = FALSE, strLen;
    char *format = "%d & %s & %s & %s & %s & %s & %s";

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
    
    sprintf(laTexStrBuffer, format, index, 
                                    info->identifier,
                                    info->desc,
                                    info->oid,
                                    info->rw,
                                    info->type,
                                    " ");
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
    info->length += strlen(info->desc);

    info->type = long2Short(((mibLeaveInfo *)(node->info))->type);
    info->length += strlen(info->type);

    info->rw = ((mibLeaveInfo *)(node->info))->rw;
    info->length += strlen(info->rw);

    return OK;
}

/* Plain text format */

