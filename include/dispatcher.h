#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

typedef enum dispatch_type {
    DISPATCH_PARAM_STAGE,
    MIBTREE_GENERATION,
    SYMBOL_SEARCHING
} dispatch_type;

typedef struct params_t {
    int firstParam;
    int secondParam;
    int thirdParam;
} params_t;

int dispatch(int type, params_t * param);

#endif /* _DISPATCHER_H_ */
