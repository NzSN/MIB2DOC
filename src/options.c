/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "options.h"
#include "list.h"
#include "string.h"

/* Declaration */
static int paramMapping(char *param);
static void mappingTableInit();

/* Global Var */
options_t optionsManager;

/* Local Var */
typedef enum optionNumber {
    SourceMibFile = 0,
    TargetPdfFile,
    UNIQUE_PARAM, /* params above this line is unique another is not */
    IncludePath,
    NumOfOptions
} optionNumber;

static char * mappingTable[NumOfOptions];

/* Local Functions */
static void helpInfoPrint();

int optionsInit(int argc, char *argv[]) {
    char *param;
    char *paramVal;
    incPathList *incNode;
    int i=0, paramIndex = 0;

    if (isNullPtr(argv)) {
        return ERROR_NULL_REF;
    }

    if (argc == 1) {
        helpInfoPrint();
        return ERROR_GENERIC;
    }

    mappingTableInit();

    /*
     * Note : should deal with error that no paramVal for
     *        an option, such as only give a "-s" but not
     *        with the value should after the "-s".
     */
    while (param = argv[i++]) {
        if ((paramIndex = paramMapping(param)) < UNIQUE_PARAM) {
            if (mappingTable[paramIndex] == null) {
                return ERROR_GENERIC;
            }
        }
        switch(paramMapping(param)) {
            case SourceMibFile:
                paramVal = argv[i++];
                optionsManager.sourceMibFilePath = paramVal;
                mappingTable[SourceMibFile] = null;
                break;
            case TargetPdfFile:
                if (TargetPdfFile < UNIQUE_PARAM && optionsManager.targetPdfPATH != null)
                    return ERROR_GENERIC;
                paramVal = argv[i++];
                optionsManager.targetPdfPATH = paramVal;
                mappingTable[TargetPdfFile] = null;
                break;
            case IncludePath:
                paramVal = argv[i++];
                incNode = (incPathList *)malloc(sizeof(incPathList));
                memset(incNode, 0, sizeof(incPathList));
                incNode->path = paramVal;
                listNodeAppend(&optionsManager.includePath.node, &incNode->node);
                break;
            default:
                helpInfoPrint();
                return ERROR_WRONG_IDX;
        }

        if (i > argc-1)
            break;
    }
}

static void helpInfoPrint() {}

static void mappingTableInit() {
    mappingTable[SourceMibFile] = "-s";
    mappingTable[TargetPdfFile] = "-t";
    mappingTable[IncludePath] = "-I";

    return;
}

static int paramMapping(char *param) {
    int index = 0;

    if (isNullPtr(param)) {
        return ERROR_NULL_REF;
    }

    while (index < NumOfOptions) {
        if (strncmp(param, mappingTable[index], strlen(param)) == 0) {
            break;
        }
        index++;
    }
    return index;
}

const char * getOption_SourceMibFilePath() {
    return optionsManager.sourceMibFilePath;
}

const char * getOption_targetPdfPath() {
    return optionsManager.targetPdfPATH;
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

    if (isNullPtr(index)) {
        return null;
    }
    if (i <= 0) {
        return null;
    }

    head = &optionsManager.includePath;
    node = &head->node;

    while (i--) {
        node = listNodeNext(&head->node);
    }

    if (node == null) {
        return null;
    }

    head = containerOf(node, incPathList, node);

    (*index)++;
    return (const char *)head->path;
}

/* options.c */
