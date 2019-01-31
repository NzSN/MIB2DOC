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
static void debugging(dispatchParam *param);
static int dispatchMakeChoice(dispatch_type dType);
static int lexBufferSwitching(char *newModule);
static int switchToModule(char *moduleName, char *sCollection);
static int switchInit();

/* Global */
extern slice sliceContainer;
dispatch_mode dispatchMode;
static unsigned char isNeedSwitchInit = TRUE;
switchingState swState;

/* Definition Section */
int dispatchInit() {
    switchInit();
    dispatchMode = DISPATCH_MODE_DOC_GENERATING;
    return ERROR_NONE;
}

int dispatch(dispatch_type disType, dispatchParam *param) {
    unsigned long key;
    char *val, *moduleName, *sCollection;
    errorType ret =  ERROR_NONE;

    if (isNullPtr(param)) {
        return ERROR_NULL_REF;
    }
    
    switch (dispatchMakeChoice(disType)) {
        case DISPATCH_PARAM_STAGE:
            key = (unsigned long)disParamGet(disParamRetrive(&param));
            val = (char *)disParamGet(disParamRetrive(&param));
            sliceStore(&sliceContainer, sliceConstruct(key, val));
            break;
        case MIBTREE_GENERATION:
            mibObjGen((unsigned long)disParamGet(disParamRetrive(&param)));
            break;
        case SYMBOL_COLLECTING:
            symbolCollecting((unsigned long)disParamGet(disParamRetrive(&param)), param);
            break;
        case SWITCH_TO_INC_BUFFER:
            moduleName = (char *)disParamGet(disParamRetrive(&param));
            sCollection = (char *)disParamGet(disParamRetrive(&param));
            ret = switchToModule(moduleName, sCollection);
            break;
        case IGNORE:
            /* Do nothing */
            break;
        case DEBUGING:
            debugging(param);
            default:
            ret = ERROR_WRONG_IDX;
    }
    return ret;
}

static void debugging(dispatchParam *param) {
    dispatchParam *result;
    unsigned long key;
    char *val;
    
    result = disParamRetrive(&param);
    if (result == NULL)
        return;
    key = (unsigned long)result->param;
    if (key == 1)
        key = 1;
    result = disParamRetrive(&param);
    if (result == NULL)
        return; 

    val = (char *)result->param;

    if (IS_VALID_SLICE_TYPE(key)) {
        printf("DEBUGGING\n");
        printf("Key:value Pair is %lu:%s\n", key, val);
    }
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
    if (dispatchMode == DISPATCH_MODE_DEBUG)
        choice = DEBUGING;

    return choice;
}

// Following codes is added for import feature.
static int switchInit() {
    int retVal;

    swState.state = DISPATCH_MODE_DOC_GENERATING;
    swState.counter = 0;

    memset(SW_CUR_BUFFER_INFO_REF(swState), 0, sizeof(YY_BUFFER_STATE));
    memset(SW_CUR_IMPORT_REF(swState), 0, sizeof(collectInfo));
    genericStackConstruct(&swState.swStack, 128 * sizeof(switchInfo), sizeof(switchInfo));

    isNeedSwitchInit = FALSE;
    return retVal;
}

static int switchToModule(char *moduleName, char *sCollection) {
    collectInfo *cInfo;

    if (isNullPtr(moduleName) || isNullPtr(sCollection)) {
        return null;
    }

    // Step 1: Push currentSwitchInfo into stack
    push(SW_STACK_REF(swState), SW_CUR_SWITCH_INFO_REF(swState)); 

    // Step 2: Update currentSwitchInfo  
    if (lexBufferSwitching(moduleName) == ERROR_GENERIC)
        /* Terminate program */
        return ABORT;
    memcpy(SW_CUR_BUFFER_INFO_REF(swState), getCurrentBufferState(), sizeof(YY_BUFFER_STATE));

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

int collectInfoInit(char *modName, char *sString, collectInfo *cInfo) {
    identList *head;

    if (isNullPtr(modName) || isNullPtr(sString) || isNullPtr(cInfo))
        return -1;

    head = (identList *)malloc(sizeof(identList));
    stringToIdentList(sString, head, SYMBOL_SEPERATOR);

    cInfo->modName = modName;
    cInfo->symbols = head;

    return 0;
}

int collectInfo_add(collectInfo *cInfo, char *symbol) {
    if (isNullPtr(cInfo))
        return FALSE;
    return appendSymToIdentList(cInfo->symbols, symbol);   
}

int collectInfo_del(collectInfo *cInfo, char *symbol) {
    if (isNullPtr(cInfo))
        return FALSE;
    return rmSymFromIdentList(cInfo->symbols, symbol);
}

identList * identListConstruct(identList *iList, char *symbolIdent) {
    if (isNullPtr(iList) || isNullPtr(symbolIdent))
        return NULL;
    memset(iList, 0, sizeof(identList));
    iList->symName = symbolIdent;
    return iList;
}

int appendSymToIdentList(identList *listHead, char *symbolIdent) {
    identList *iList;

    if (isNullPtr(listHead) || isNullPtr(symbolIdent))
        return FALSE;
    
    iList = (identList *)malloc(sizeof(identList));
    listNodeAppend(&listHead->node, &identListConstruct(iList, symbolIdent)->node);
    
    return TRUE;
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



#ifdef MIB2DOC_UNIT_TESTING

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

int identListTesting() {
    char *str = "Hello, World";
    identList *iList = (identList *)malloc(sizeof(identList)); 
    
    iList = identListConstruct(iList, str);
    appendSymToIdentList(iList, "TT");
    assert_string_equal(iList->symName, str);
}

int collectInfoTesting() {

}

#endif /* MIB2DOC_UNIT_TESTING */

/* End of file */

