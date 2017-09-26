#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

typedef enum dispatch_type {
    DISPATCH_PARAM_STAGE = 0,
    MIBTREE_GENERATION,
    SYMBOL_COLLECTING,
    IGNORE
} dispatch_type;

typedef enum dispatch_mode {
    DISPATCH_MODE_SYMBOL_COLLECTING = 0,
    DISPATCH_MODE_DOC_GENERATING = 1
} dispatch_mode;

typedef struct params_t {
    void *firstParam;
    void *secondParam;
    void *thirdParam;
} params_t;

int dispatch(int type, params_t * param);

#endif /* _DISPATCHER_H_ */

