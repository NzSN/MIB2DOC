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

/* Global */
switchingState swState;
extern int importStackIndex;
extern YY_BUFFER_STATE importStack[MAX_INCLUDE_DEPTH];


/* Definition Section */
int dispatchInit() {
    dispatchMode = DISPATCH_MODE_DOC_GENERATING;
    return ok;
}

errorType dispatch(dispatch_type dType, params_t *param) {
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
            switchToModule(param);
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

}

static int collectInfoInit(char *modName, char *sString, collectInfo *cInfo) {

}

char * switch_CurrentMod(char *modName, int len) {}

/* End of file */
