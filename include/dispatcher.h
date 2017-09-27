#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "type.h"
typedef enum dispatch_type {
    DISPATCH_PARAM_STAGE = 0,
    MIBTREE_GENERATION,
    SYMBOL_COLLECTING,
    SWITCH_TO_INC_BUFFER,
    IGNORE
} dispatch_type;

typedef enum dispatch_mode {
    DISPATCH_MODE_SYMBOL_COLLECTING = 0,
    DISPATCH_MODE_DOC_GENERATING = 1
} dispatch_mode;

int dispatch(int type, params_t * param);

#endif /* _DISPATCHER_H_ */

