/* Created by Aydenlin at 2017/9/12 */

#include "dispatcher.h"

int dispatch(dispatch_type type, params_t *param) {
    
    switch (type) {
        case DISPATCH_PARAM_STAGE:
            appendElement_el(&elistHead, 
                buildElement(param->firstParam, param->secondParam));
            break;
        case MIBTREE_GENERATION:
            deal_with(param->firstParam);
            break;
        case SYMBOL_SEARCHING:
            break;
    }
}

