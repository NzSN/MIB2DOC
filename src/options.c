/* options.c */

#include <stdio.h>
#include "options.h"

/* Declaration */
static int paramMapping(char *param);
static void mappingTableInit();

/* Global Var */
options_t optionsManager;

/* Local Var */
typedef enum optionNumber {
    SourceMibFile = 0,
    TargetPdfFile,
    IncludePath,
    NumOfOptions
} optionNumber;

static char * mappingTable[NumOfOptions];

int optionsInit(char *argv[], int argc) {
    char *param;
    char *paramVal;
    incPathList *incNode;
    int i=0;

    if (IS_PTR_NULL(argv)) {
        return -1;
    }

    if (argc == 0) {
        printf("usage : mib2doc [options] <mibfilePath>");
        return -1;
    }

    mappingTableInit();

    /*
     * Note : should deal with error that no paramVal for
     *        an option, such as only give a "-s" but not
     *        with the value should after the "-s".
     */
    while (param = argv[i++]) {
        switch(paramMapping(param)) {
            case SourceMibFile:
                /* Deal with param */
                paramVal = argv[i++];
                optionsManager.SourceMibFile = paramVal;
                break;
            case TargetPdfFile:
                /* Deal with param */
                paramVal = argv[i++];
                optionsManager.targetPdfPATH = paramVal;
                break;
            case includePath:
                /* Deal with inc path */
                paramVal = argv[i++];
                incNode = (incPathList *)malloc(sizeof(incPathList));
                memset(incNode, 0, sizeof(incPathList));
                incNode->path = paramVal;
                listAppend(&optionsManager->includePath.node, &incNode->node);
                break;
        }

        if (i > argc-1)
            break;
    }
}

static void mappingTableInit() {
    mappingTable[SourceMibFile] = "-s";
    mappingTable[TargetPdfFile] = "-t";
    mappingTable[includePath] = "-I";

    return;
}

static int paramMapping(char *param) {
    int index = 0;

    if (IS_PTR_NULL(param)) {
        return -1;
    }

    while (index < NumOfOptions) {
        if (strncmp(param, mappingTable[index], strlen(param)) == 0) {
            break;
        }
        index++;
    }
    return index;
}

char * getOption_SourceMibFilePath() {
    return optionManager.sourceMibFilePath;
}

char * getOption_targetPdfPath() {
    return optionManager.targetPdfPATH;
}

/*
 * Parameter
 *     - index : is used to track where to
 *               start to get next include path
 * Return Value
 *     - null : no more include path to get
 *     - string : a string that describe the path
 *                of include path.
 *     - val < 0 : error
 */
const char * getOption_includePath(int *index) {
    int i = *index;
    incPathList *head;
    listNode *node;
    if (IS_PTR_NULL(index) || i < 0) {
        return -1;
    }

    head = &optionManager.includePath;

    while (i--) {
        node = listNext(&head->node);
    }
    head = containerOf(node, incPathList, node);

    *index++;
    return (const char *)head.path;
}

/* options.c */
