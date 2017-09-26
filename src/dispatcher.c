/* Created by Aydenlin at 2017/9/12 */

#include "type.h"
#include "dispatcher.h"
#include <stdlib.h>

/* Declaration Section */
dispatch_mode dispatchMode;
#define DECISION_FORMULA(DIS_MODE, DIS_TYPE) ({ \
    int mode_ = DIS_MODE;   \
    int type_ = DIS_TYPE;   \
                            \
    ((3 - type_) / 2 * 3) * abs(mode_ - 1) + (type_ * mode_);  \
})

static int dispatchMakeChoice(dispatch_mode dMode, dispatch_type dType);

/* Definition Section */
int dispatchInit() {
    dispatchMode = DISPATCH_MODE_DOC_GENERATING;
    return ok;
}
errorType dispatch(dispatch_type dType, params_t *param) {
    errorType ret = ok; 
    
    switch (dispatchMakeChoice(dMode, dType)) {
        case DISPATCH_PARAM_STAGE:
            appendElement_el(&elistHead, buildElement(*CAST((int *), paramListGet(&param)->param), 
                                         CAST((char *), paramListGet(&param)->param)));
            break;
        case MIBTREE_GENERATION:
            deal_with(*CAST((int *), paramListGet(&param)->param));
            break;
        case SYMBOL_COLLECTING:
            symbolCollecting(CAST(int, paramListGet(&param)->param), paramListGet(&param)->param);
            break;
        case IGNORE:
            /* Do nothing */
            break;
        default:
            ret = error_wrong_index;
    }
    
    return ret; 
}

static int dispatchMakeChoice(dispatch_mode dMode, dispatch_type dType) {
   return DECISION_FORMULA(dispatchMode, dType);
}

