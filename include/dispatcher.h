#include "type.h"
#include "stack.h"
#include "symbolTbl.h"
#include "list.h"
#include "hash.h"
#include "lexer.h"

#ifndef _DISPATCHER_H_
#define _DISPATCHER_H_

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

enum {
    SWITCHING_INC_PURPOSE,
    SWITCHING_GEN_PURPOSE
};

typedef struct switchInfo {
    char *fileName;
    /* Kind of purposes:
     *   SWITCHING_INC_PURPOSE: The purpose of parser is to find the symbol
     *                          that import by import statements and then 
     *                          include into symbol table.
     *   SWITCH_GEN_PURPOSE: The purpose of parser is to translate MIB file
     *                       into tex format. */
    int purpose;
    collectInfo importInfo;
    YY_BUFFER_STATE bufferInfo;
} switchInfo;

#define MAX_INCLUDE_DEPTH 10
typedef struct switchingState {
    // Current state of switching
    int state;
    // Number of modules in stack
    int counter;
    // Current processing module info
    switchInfo currentSwitchInfo;
    // Stack of struct switchInfo
    genericStack swStack;
} switchingState;

// switchingState reference macros
#define SW_STATE(SW) ((SW)->state)
#define SW_STATE_SET(SW, STATE) ((SW)->state = STATE)

/* Refresh switchingState: 
 *   This procedure is a solutions to resolve
 *   dismatch of purpose between global purpose
 *   and purpose of current info, you should call
 *   this procedure to resolve such situation after
 *   module switching. */
#define SW_STATE_REFRESH(SW) ({\
    if (SW.currentSwitchInfo.purpose == SWITCHING_INC_PURPOSE) {\
        SW.state = DISPATCH_MODE_SYMBOL_COLLECTING;\
    } else if (SW.currentSwitchInfo.purpose == SWITCHING_GEN_PURPOSE) {\
        SW.state = DISPATCH_MODE_DOC_GENERATING;\
    }\
    0;\
})

#define SW_COUNTER(SW) ((SW)->counter)
#define SW_COUNTER_SET(SW, COUNT) ((SW)->counter = COUNT)

#define SW_CUR_SWITCH_INFO(SW) (&(SW)->currentSwitchInfo)

#define SW_CUR_FILE_NAME(SW) (SW_CUR_SWITCH_INFO(SW)->fileName)
#define SW_CUR_SET_FILE_NAME(SW, NAME) (SW_CUR_SWITCH_INFO(SW)->fileName = NAME) 

#define SW_CUR_IMPORT(SW) (&SW_CUR_SWITCH_INFO(SW)->importInfo)
#define SW_CUR_SET_SYMBOLS(SW, SYM) (SW_CUR_IMPORT(SW)->symbols = SYM)
#define SW_CUR_BUFFER_INFO(SW) (SW_CUR_SWITCH_INFO(SW)->bufferInfo)
#define SW_SET_CUR_BUFFER_INFO(SW, BUFFER) (SW_CUR_SWITCH_INFO(SW)->bufferInfo = BUFFER)
#define SW_CUR_PURPOSE(SW) (SW_CUR_SWITCH_INFO(SW)->purpose)
#define SW_CUR_SET_PURPOSE(SW, PUR) (SW_CUR_SWITCH_INFO(SW)->purpose = PUR)

#define SW_STACK(SW) (&(SW)->swStack)
#define SW_STACK_BASE(SW) ((SW)->swStack.base)
#define SW_STACK_TOP(SW) ((SW)->swStack.top)
#define SW_STACK_BUFFER_SIZE(SW) ((SW)->swStack.bufferSize)
#define SW_STACK_UNIT_SIZE(SW) ((SW)->swStack.unitSize)

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
dispatchParam * dispatchParamNext(dispatchParam *disparam);
int disParamRelease(dispatchParam *disParam, int (*destruct)(void *));
int disParamRelease_Static(dispatchParam *disParam, int (*destruct)(void *));

YY_BUFFER_STATE getCurrentBufferState();
char * switch_CurrentMod();
int dispatch(dispatch_type dType, dispatchParam * param);
int switchToModule(switchingState *swState, char* moduleName);

// collectInfo functions
int collectInfoInit(collectInfo *cInfo, char *modName);
collectInfo * collectInfoConst(char *modName);
int collectInfo_add(collectInfo *cInfo, char *symbol);
int collectInfo_del(collectInfo *cInfo, char *symbol);
int collectInfo_exists(collectInfo *cInfo, char *symbol);
int collectInfo_release(collectInfo *cInfo);

int importWorks(genericStack *importInfoStack);
switchingState * getCurSwInfo();

#ifdef MIB2DOC_UNIT_TESTING

void dispatcher__COLLECT_INFO(void **state);
void dispatcher__SYMBOLTABLE(void **state);

#endif /* MIB2DOC_UNIT_TESTING */

#endif /* _DISPATCHER_H_ */

