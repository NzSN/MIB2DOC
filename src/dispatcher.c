/* Created by Aydenlin at 2017/9/12 */

#include "type.h"
#include "stack.h"
#include "dispatcher.h"
#include "mibTreeGen.h"
#include <stdlib.h>
#include <list.h>
#include <options.h>
#include <util.h>

/* Declaration Section */
static int dispatchMakeChoice(dispatch_type dType);
static int lexBufferSwitching(char *newModule);
static int switchToModule(dispatchParam *param);
static int collectInfoInit(char *modName, char *sString, collectInfo *cInfo);
static int switchInit();
/* Global */
extern slice sliceContainer;
dispatch_mode dispatchMode;
static unsigned char isNeedSwitchInit = TRUE;
switchingState swState;

/* Definition Section */
int dispatchInit() {
    dispatchMode = DISPATCH_MODE_DOC_GENERATING;
    return ERROR_NONE;
}

int dispatch(dispatch_type disType, dispatchParam *param) {
    errorType ret =  ERROR_NONE;

    if (isNullPtr(param)) {
        return ERROR_NULL_REF;
    }
    
    switch (dispatchMakeChoice(disType)) {
        case DISPATCH_PARAM_STAGE:
            sliceStore(&sliceContainer,
                sliceConstruct((unsigned long)disParamRetrive(&param)->param,
                    (char *)disParamRetrive(&param)->param));
            break;
        case MIBTREE_GENERATION:
            mibObjGen((unsigned long)disParamRetrive(&param)->param);
            break;
        case SYMBOL_COLLECTING:
            symbolCollecting((unsigned long)disParamRetrive(&param)->param, param);
            break;
        case SWITCH_TO_INC_BUFFER:
            if (isNeedSwitchInit)
                switchInit();
            ret = switchToModule(param);
            break;
        case IGNORE:
            /* Do nothing */
            break;
        default:
            ret = ERROR_WRONG_IDX;
    }
    return ret;
}

static int dispatchMakeChoice(dispatch_type dType) {
    int choice = -1;

    if (dispatchMode == DISPATCH_MODE_DOC_GENERATING) {
        choice = dType;
    } else if (dispatchMode == DISPATCH_MODE_SYMBOL_COLLECTING) {
        if (dType == DISPATCH_PARAM_STAGE || dType == MIBTREE_GENERATION) {
            choice = SYMBOL_COLLECTING;
        } else {
            /* SYMBOL_COLLECTING, SWITCH_TO_INC_BUFFER no redirect */
            choice = dType;
        }
    }

    return choice;
}

// Following codes is added for import feature.
static int switchInit() {
    int retVal;

    swState.state = DISPATCH_MODE_SYMBOL_COLLECTING;
    swState.counter = 0;
    memcpy(SW_CUR_BUFFER_INFO_REF(swState), getCurrentBufferState(), sizeof(YY_BUFFER_STATE));
    memset(SW_CUR_IMPORT_REF(swState), 0, sizeof(collectInfo));
    genericStackConstruct(&swState.swStack, 128 * sizeof(switchInfo), sizeof(switchInfo));

    isNeedSwitchInit = FALSE;
    return retVal;
}

static int switchToModule(dispatchParam *param) {
    char *moduleName;
    char *sCollection;
    collectInfo *cInfo;

    if (isNullPtr(param)) {
        return null;
    }

    moduleName = (char *)disParamRetrive(&param)->param;
    sCollection = (char *)disParamRetrive(&param)->param;

    // Step 1: Push currentSwitchInfo into stack
    pushByIndex(SW_STACK_BASE(swState), SW_CUR_BUFFER_INFO_REF(swState), SW_STACK_TOP(swState), SW_STACK_BUFFER_SIZE(swState), SW_STACK_UNIT_SIZE(swState));
    
    // Step 2: Update currentSwitchInfo
    

    if (lexBufferSwitching(moduleName) == ERROR_GENERIC) {
        /* Terminate whole system */
        return ABORT;
    }
    return 0;
}

static int lexBufferSwitching(char *newModule) {
    int index = 0;
    const char *path;
    char *targetModulePath;

    while (path = getOption_includePath(&index)) {
        targetModulePath = (char *)malloc(strlen(path)+strlen(newModule)+1);
        strncpy(targetModulePath, path, strlen(path));
        strncat(targetModulePath, newModule, strlen(newModule));

        yyin = fopen(targetModulePath, "r");
        if (!yyin) {
            continue;
        }
        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    }
    if (path == null) {
        return ERROR_GENERIC;
    }
    return ERROR_NONE;
}

char * switch_CurrentMod(char *modName, int len) {
    return SW_CUR_IMPORT(swState).modName;
}

/**************************
 *  collectInfo functions *
 **************************/
#define SYMBOL_SEPERATOR ','

static int collectInfoInit(char *modName, char *sString, collectInfo *cInfo) {
    identList *head;

    if (isNullPtr(modName) || isNullPtr(sString) || isNullPtr(cInfo))
        return -1;

    head = (identList *)malloc(sizeof(identList));
    stringToIdentList(sString, head, SYMBOL_SEPERATOR);

    cInfo->modName = modName;
    cInfo->symbols = head;

    return 0;
}

int rmSymFromIdentList(identList *listHead, char *symbolIdent) {
    identList *listProcessing;

    if (isNullPtr(listHead)) {
        return ERROR_NULL_REF;
    }

    listProcessing = listHead;

    while (listHead) {
        if (!strncmp(listProcessing->symName, symbolIdent, strlen(symbolIdent))) {
            /* delete this node from lsit */
            listNodeDelete(&listProcessing->node);
            if (listProcessing == listHead && listHead->node.next != NULL) {
                SW_CUR_IMPORT(swState).symbols = containerOf(listHead->node.next, identList, node);
            }
            RELEASE_MEM(listProcessing->symName);
            RELEASE_MEM(listProcessing);
            break;
        }
        listHead = containerOf(listHead->node.next, identList, node);
    }
    return ERROR_NONE;
}

/* End of file */
