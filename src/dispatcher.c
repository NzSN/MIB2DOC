/* Created by Aydenlin at 2017/9/12 */

#include "type.h"
#include "dispatcher.h"
#include <stdlib.h>

/* Declaration Section */
#define DECISION_FORMULA(DIS_MODE, DIS_MODE) ({ \
    int mode_ = DIS_MODE;   \
    int type_ = DIS_TYPE;   \
                            \
    ((3 - type_) / 2 * 3) * abs(mode_ - 1) + (type_ * mode_);  \
})

static int dispatchMakeChoice(dispatch_mode dMode, dispatch_type dType);

/* Definition Section */
errorType dispatch(dispatch_mode dMode, dispatch_type dType, params_t *param) {
    errorType ret = ok; 

    switch (dispatchMakeChoice(dMode, dType)) {
        case DISPATCH_PARAM_STAGE:
            appendElement_el(&elistHead, buildElement(*(int *)param->firstParam, 
                                         (char *)param->secondParam));
            break;
        case MIBTREE_GENERATION:
            deal_with(*(int *)param->firstParam);
            break;
        case SYMBOL_COLLECTING:
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
   return DECISION_FORMULA(dMode, dTYpe);
}

