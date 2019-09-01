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

#include "format.h"
#include "mibTreeGen.h"
#include "typeTable.h"

#include "options.h"

/* Defines */
#define SIZE_OF_LATEX_BUFFER 256

/* Global */
char *beginFrom;
int beginOid;
int isInitilized;

/* Local */
typedef int (*docGenInFormat)(mibObjectTreeNode *, FILE *file, formatInfo *info);
static docGenInFormat docGenRoutine[] = {
    // Latex format
    docGenInLatex
};

int documentGen(mibTreeHead *treeHead, char *filePath) {

    mibTree *tree;

    if (isNullPtr(treeHead) || isNullPtr(filePath))
        return ERROR;

    if (treeHead->numOfTree == 0) return OK;

    tree = mibTreeHeadFirstTree(treeHead);

    /* Get begin node If user not specified
     * a begin node default to the root of tree */
    beginFrom = optMngGetOptVal(optionsManager, OPT_KEY_BEGIN_FROM);
    if (isNullPtr(beginFrom))
        beginFrom = tree->rootName;

    mibObjectTreeNode *beginNode = search_MibTree(tree->root, beginFrom);
    if (isNullPtr(beginNode)) {
        abortWithMsg("Can't find begin node: %s\n", beginFrom);
    }

    beginOid = strlen(getOidFromInfo(beginNode));

    FILE *file = fopen(filePath, "w+");
    if (isNullPtr(file))
        abortWithMsg("Can't open file %s\n", filePath);

    formatInfo info;
    int status;

    info.beginOid = beginOid;

    // Todo: choose format via option
    status = docGenRoutine[0](beginNode, file, &info);

    fclose(file);

    return status;
}
