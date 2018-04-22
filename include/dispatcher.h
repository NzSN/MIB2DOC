#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "type.h"
#include "stack.h"
#include "list.h"

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

typedef struct identList {
    int found;
    char *symName;
    listNode node;
} identList;

extern struct yy_buffer_state;
typedef struct yy_buffer_state * YY_BUFFER_STATE;

typedef struct collectInfo {
    char *modName;
    identList *symbols;
} collectInfo;

#define MAX_INCLUDE_DEPTH 10
typedef struct switchingState {
    /* Include status or Non-Include status */
    int state;
    /* Number of modules in stack */
    int counter;
    /* Stack of struct collectInfo */
    genericStack *modStack;
    char *currentModule;
    /* Buffer stack */
    YY_BUFFER_STATE importStack[MAX_INCLUDE_DEPTH];
    int importStackIndex;
} switchingState;

extern dispatch_mode dispatchMode;

YY_BUFFER_STATE * getCurrentBufferState();
char * switch_CurrentMod(char *modName, int len);
int dispatch(dispatch_type dType, dispatchParam * param);
int rmSymFromIdentList(identList *listHead, char *symbolIdent);
#endif /* _DISPATCHER_H_ */
