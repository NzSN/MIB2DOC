#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

#include "type.h"
#include "stack.h"
#include "list.h"
#include "hash.h"
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
    IGNORE,
    DEBUGING
} dispatch_type;

typedef enum dispatch_mode {
    DISPATCH_MODE_SYMBOL_COLLECTING = 0,
    DISPATCH_MODE_DOC_GENERATING = 1,
    DISPATCH_MODE_DEBUG = 2
} dispatch_mode;

typedef struct identList {
    int found;
    char *symName;
    listNode node;
} identList;

typedef struct collectInfo {
    char *modName;
    hashMap *symbols;
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

// switchingState reference macros
#define SW_CUR_SWITCH_INFO(SW_STATE) (SW_STATE.currentSwitchInfo)
#define SW_CUR_SWITCH_INFO_REF(SW_STATE) (&SW_STATE.currentSwitchInfo)

#define SW_CUR_IMPORT(SW_STATE) (SW_CUR_SWITCH_INFO(SW_STATE).importInfo)
#define SW_CUR_IMPORT_REF(SW_STATE) (&SW_CUR_IMPORT(SW_STATE))
#define SW_CUR_BUFFER_INFO(SW_STATE) (SW_CUR_SWITCH_INFO(SW_STATE).bufferInfo)
#define SW_CUR_BUFFER_INFO_REF(SW_STATE) (&SW_CUR_BUFFER_INFO(SW_STATE))

#define SW_STACK(SW_STATE) (SW_STATE.swStack)
#define SW_STACK_REF(SW_STATE) (&SW_STATE.swStack)
#define SW_STACK_BASE(SW_STATE) (SW_STATE.swStack.base)
#define SW_STACK_TOP(SW_STATE) (SW_STATE.swStack.top)
#define SW_STACK_BUFFER_SIZE(SW_STATE) (SW_STATE.swStack.bufferSize)
#define SW_STACK_UNIT_SIZE(SW_STATE) (SW_STATE.swStack.unitSize)

// identList macros
#define IDENT_FOUND (1)
#define IDENT_NOT_FOUND (0)

extern dispatch_mode dispatchMode;
extern switchingState swState;

int dispatchInit();

dispatchParam * disParamConstruct(void *arg);
dispatchParam * disParamStore(dispatchParam *head, dispatchParam *pl);
dispatchParam * disParamRetrive(dispatchParam **head);
void * disParamGet(dispatchParam *disparam);

YY_BUFFER_STATE getCurrentBufferState();
char * switch_CurrentMod(char *modName, int len);
int dispatch(dispatch_type dType, dispatchParam * param);
int appendSymToIdentList(identList *listHead, char *symbolIdent);
int rmSymFromIdentList(identList *listHead, char *symbolIdent);

// collectInfo functions
int collectInfoInit(char *modName, char *sString, collectInfo *cInfo);
int collectInfo_add(collectInfo *cInfo, char *symbol);
int collectInfo_del(collectInfo *cInfo, char *symbol);

#ifdef MIB2DOC_UNIT_TESTING

int identListTesting();
int collectInfoTesting();

#endif /* MIB2DOC_UNIT_TESTING */

#endif /* _DISPATCHER_H_ */

