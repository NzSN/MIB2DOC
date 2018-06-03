#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "type.h"
#include "stack.h"
#include "list.h"
#include "lexer.h"


typedef struct dispatchParam {
    void *param;
    listNode node;
} dispatchParam;

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

typedef struct collectInfo {
    char *modName;
    identList *symbols;
} collectInfo;

typedef struct switchInfo {
    collectInfo importInfo;
    YY_BUFFER_STATE bufferInfo;
} switchInfo;

#define MAX_INCLUDE_DEPTH 10
typedef struct switchingState {
    // Include status or Non-Include status
    int state;
    // Number of modules in stack
    int counter;
    // Current processing module info
    switchInfo currentSwitchInfo;
    // Stack of struct switchInfo
    genericStack swStack;
} switchingState;

extern dispatch_mode dispatchMode;
extern switchingState swState;

dispatchParam * disParamConstruct(void *arg);
dispatchParam * disParamStore(dispatchParam *head, dispatchParam *pl);
dispatchParam * disParamRetrive(dispatchParam **head);

YY_BUFFER_STATE * getCurrentBufferState();
char * switch_CurrentMod(char *modName, int len);
int dispatch(dispatch_type dType, dispatchParam * param);
int rmSymFromIdentList(identList *listHead, char *symbolIdent);

#endif /* _DISPATCHER_H_ */
