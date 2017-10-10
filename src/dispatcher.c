/* Created by Aydenlin at 2017/9/12 */

#include "type.h"
#include "stack.h"
#include "dispatcher.h"
#include "mibTreeGen.h"
#include <stdlib.h>


/* Declaration Section */
dispatch_mode dispatchMode;
static int dispatchMakeChoice(dispatch_type dType);
static int switchToModule(params_t *param);
extern elementList elistHead;
static int collectInfoInit(char *modName, char *sString, collectInfo *cInfo);
/* Global */
switchingState swState;

/* Definition Section */
int dispatchInit() {
    dispatchMode = DISPATCH_MODE_DOC_GENERATING;
    return ok;
}

int dispatch(dispatch_type dType, params_t *param) {

    errorType ret = ok;

    switch (dispatchMakeChoice(dType)) {
        case DISPATCH_PARAM_STAGE:
            appendElement_el(&elistHead,
                            buildElement((int)paramListGet(&param)->param,
                            paramListGet(&param)->param));
            break;
        case MIBTREE_GENERATION:
            deal_with((int)paramListGet(&param)->param);
            break;
        case SYMBOL_COLLECTING:
            symbolCollecting((int)paramListGet(&param)->param, param);
            break;
        case SWITCH_TO_INC_BUFFER:
            ret = switchToModule(param);
        case IGNORE:
            /* Do nothing */
            break;
        default:
            ret = error_wrong_index;
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

static int switchToModule(params_t *param) {
    char *moduleName;
    char *sCollection;
    collectInfo *cInfo;

    if (IS_PTR_NULL(param)) {
        return null;
    }

    moduleName = paramListGet(&param);
    sCollection = paramListGet(&param);

    cInfo = (collectInfo *)malloc(sizeof(collectInfo));
    collectInfoInit(moduleName, sCollection, cInfo);

    push(swState.modStack, cInfo);
    pushByIndex(swState.importStack,
                getCurrentBufferState(),
                swState.importStackIndex,
                MAX_INCLUDE_DEPTH-1);
    if (lexBufferSwitching(moduleName) == error) {
        /* Terminate whole system */
        return abort_terminate;
    }

    return 0;
}

int lexBufferSwitching(char *newModule) {
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
        return error;
    }

    return ok;
}

char * switch_CurrentMod(char *modName, int len) {
    return swState.currentModule;
}

/**************************
 *  collectInfo functions *
 **************************/
#define SYMBOL_SEPERATOR ','

static int collectInfoInit(char *modName, char *sString, collectInfo *cInfo) {
      identList *head;

      if (IS_PTR_NULL(modName) || IS_PTR_NULL(sString) || IS_PTR_NULL(cInfo))
          return -1;

      head = (identList *)malloc(sizeof(identList));
      stringToIdentList(sString, head, SYMBOL_SEPERATOR);

      cInfo->modName = modName;
      cInfo->symbols = head;

      return 0;
}

/* End of file */
