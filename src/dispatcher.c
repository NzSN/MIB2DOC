/* Created by Aydenlin at 2017/9/12 */

#include "type.h"
#include "stack.h"
#include "dispatcher.h"
#include "mibTreeGen.h"
#include <stdlib.h>
#include <list.h>
#include <options.h>
#include <util.h>
#include "hash.h"

/* Declaration Section */
static void debugging(dispatchParam *param);
static int dispatchMakeChoice(dispatch_type dType);
static int lexBufferSwitching(char *newModule);
static int switchToModule(char *moduleName, char *sCollection);
static int symbolHashing(void *key);
static int switchInit();

/* Global */
extern slice sliceContainer;
dispatch_mode dispatchMode;
static unsigned char isNeedSwitchInit = TRUE;
switchingState swState;

typedef struct {
    pair_key_base base;
    char *symbolIndex;
} symbolKey;

typedef struct {
    pair_val_base base;
    char *symbol;
} symbolVal;

/* Definition Section */
int dispatchInit() {
    switchInit();
    dispatchMode = DISPATCH_MODE_DOC_GENERATING;
    return ERROR_NONE;
}

int dispatch(dispatch_type disType, dispatchParam *param) {
    unsigned long key;
    char *val, *moduleName, *sCollection;
    errorType ret =  ERROR_NONE;

    if (isNullPtr(param)) {
        return ERROR_NULL_REF;
    }
    
    switch (dispatchMakeChoice(disType)) {
        case DISPATCH_PARAM_STAGE:
            key = (unsigned long)disParamGet(disParamRetrive(&param));
            val = (char *)disParamGet(disParamRetrive(&param));
            sliceStore(&sliceContainer, sliceConstruct(key, val));
            break;
        case MIBTREE_GENERATION:
            mibObjGen((unsigned long)disParamGet(disParamRetrive(&param)));
            break;
        case SYMBOL_COLLECTING:
            symbolCollecting((unsigned long)disParamGet(disParamRetrive(&param)), param);
            break;
        case SWITCH_TO_INC_BUFFER:
            moduleName = (char *)disParamGet(disParamRetrive(&param));
            sCollection = (char *)disParamGet(disParamRetrive(&param));
            ret = switchToModule(moduleName, sCollection);
            break;
        case IGNORE:
            /* Do nothing */
            break;
        case DEBUGING:
            debugging(param);
            default:
            ret = ERROR_WRONG_IDX;
    }
    return ret;
}

static void debugging(dispatchParam *param) {
    dispatchParam *result;
    unsigned long key;
    char *val;
    
    result = disParamRetrive(&param);
    if (result == NULL)
        return;
    key = (unsigned long)result->param;
    if (key == 1)
        key = 1;
    result = disParamRetrive(&param);
    if (result == NULL)
        return; 

    val = (char *)result->param;

    if (IS_VALID_SLICE_TYPE(key)) {
        printf("DEBUGGING\n");
        printf("Key:value Pair is %lu:%s\n", key, val);
    }
}

static int dispatchMakeChoice(dispatch_type dType) {
    int choice = -1;

    if (dispatchMode == DISPATCH_MODE_DOC_GENERATING) {
        choice = dType;
    } else if (dispatchMode == DISPATCH_MODE_SYMBOL_COLLECTING) {
        if (dType == DISPATCH_PARAM_STAGE || dType == MIBTREE_GENERATION) {
            choice = SYMBOL_COLLECTING;
        } else {
            /* SYMBOL_COLLECTING, SWITCH_TO_INC_BUFFER no redirect */
            choice = dType;
        }
    }
    if (dispatchMode == DISPATCH_MODE_DEBUG)
        choice = DEBUGING;

    return choice;
}

// Following codes is added for import feature.
static int switchInit() {
    int retVal;

    swState.state = DISPATCH_MODE_DOC_GENERATING;
    swState.counter = 0;

    memset(SW_CUR_BUFFER_INFO_REF(swState), 0, sizeof(YY_BUFFER_STATE));
    memset(SW_CUR_IMPORT_REF(swState), 0, sizeof(collectInfo));
    genericStackConstruct(&swState.swStack, 128 * sizeof(switchInfo), sizeof(switchInfo));

    isNeedSwitchInit = FALSE;
    return retVal;
}

static int switchToModule(char *moduleName, char *sCollection) {
    collectInfo *cInfo;

    if (isNullPtr(moduleName) || isNullPtr(sCollection)) {
        return null;
    }

    // Step 1: Push currentSwitchInfo into stack
    push(SW_STACK_REF(swState), SW_CUR_SWITCH_INFO_REF(swState)); 

    // Step 2: Update currentSwitchInfo  
    if (lexBufferSwitching(moduleName) == ERROR_GENERIC)
        /* Terminate program */
        return ABORT;
    memcpy(SW_CUR_BUFFER_INFO_REF(swState), getCurrentBufferState(), sizeof(YY_BUFFER_STATE));

    return 0;
}

static int lexBufferSwitching(char *newModule) {
    int index = 0;
    const char *path;
    char *targetModulePath;

    while (path = getOption_includePath(&index)) {
        targetModulePath = (char *)malloc(strlen(path)+strlen(newModule)+1);
        strncpy(targetModulePath, path, strlen(path));
        strncat(targetModulePath, newModule, strlen(newModule));

        yyin = fopen(targetModulePath, "r");
        if (!yyin) {
            continue;
        }
        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
    }
    if (path == null) {
        return ERROR_GENERIC;
    }
    return ERROR_NONE;
}

char * switch_CurrentMod(char *modName, int len) {
    return SW_CUR_IMPORT(swState).modName;
}

/**************************
 *  collectInfo functions *
 **************************/
#define SYMBOL_SEPERATOR ','

static int symbolHashing(void *key) { 
    int hashVal = 0;
    char *keyString;
    symbolKey *sKey = (symbolKey *)key;  

    if (isNullPtr(key))
        return -1;

    keyString = sKey->symbolIndex;

    int index = 0, size, value;      
    size = strlen(keyString);
    
    while (index < size) {
        value = keyString[index];
        hashVal += (value << 5) + value; 
        ++index;
    }

    return hashVal;
}

static int symbolKeyEqual(void *lSymbol, void *rSymbol) {
    int lLength, rLength;
    symbolKey *lSymbol_ = (symbolKey *)lSymbol;
    symbolKey *rSymbol_ = (symbolKey *)rSymbol;
    
    char *lStr = lSymbol_->symbolIndex;
    char *rStr = rSymbol_->symbolIndex;
    
    if (isNullPtr(lStr) || isNullPtr(rStr))
        return FALSE;

    lLength = lLength > rLength ? lLength : rLength;
    return ! strncmp(lStr, rStr, lLength);
}

static int symbolValEqual(void *lSymbol, void *rSymbol) {
    int lLength, rLength;
    symbolVal *lSymbol_ = (symbolVal *)lSymbol;
    symbolVal *rSymbol_ = (symbolVal *)rSymbol;
    
    if (isNullPtr(lSymbol) || isNullPtr(rSymbol))
        return FALSE;

    char *lStr = lSymbol_->symbol; 
    char *rStr = rSymbol_->symbol;
    
    if (isNullPtr(lStr) || isNullPtr(rStr))
        return FALSE;

    lLength = lLength > rLength ? lLength : rLength;
    return ! strncmp(lStr, rStr, lLength);
}

static void * symbolKeyValue(void *symbol) {
    symbolKey *symKey = (symbolKey *)symbol;

    if (isNullPtr(symbol))
        return NULL;

    return symKey->symbolIndex;
}

static void * symbolValValue(void *symbol) {
    symbolVal *symVal = (symbolVal *)symbol;
    return symVal->symbol;
}

static int symbolKeyRelease(void *symbol) {
    symbolKey *symKey = (symbolKey *)symbol;
    
    if (isNullPtr(symbol))
        return FALSE;

    if (! isNullPtr(symKey->symbolIndex))
        RELEASE_MEM(symKey->symbolIndex);

    RELEASE_MEM(symKey);

    return TRUE;
} 

static int symbolValRelease(void *symbol) {
    symbolVal *symVal = (symbolVal *)symbol;

    if (isNullPtr(symbol))
        return FALSE;

    if (! isNullPtr(symVal->symbol))
        RELEASE_MEM(symVal->symbol);

    RELEASE_MEM(symVal);

    return TRUE;
}

symbolKey * symbolKeyConst(char *symbols) {
    pair_key_base *base;
    symbolKey *key = (symbolKey *)malloc(sizeof(symbolKey)); 

    base = &key->base;
    
    // base init
    base->isEqual = symbolKeyEqual; 
    base->release = symbolKeyRelease; 
    base->value = symbolKeyValue;

    key->symbolIndex = symbols; 
    
    return key;
}

symbolVal * symbolValConst(char *symbol) {
    pair_val_base *base;
    symbolVal *val = (symbolVal *)malloc(sizeof(symbolVal));

    base = &val->base;

    // base init
    base->isEqual = symbolValEqual;
    base->release = symbolValRelease;
    base->value = symbolValValue;

    val->symbol = symbol;

    return val;
}

int collectInfoInit(char *modName, char *sString, collectInfo *cInfo) {
    hashMap *symbolMap;

    if (isNullPtr(modName) || isNullPtr(sString) || isNullPtr(cInfo))
        return -1;

    symbolMap = hashMapConstruct(100, symbolHashing);

    stringToIdentList(sString, symbolMap, SYMBOL_SEPERATOR);

    cInfo->modName = modName;
    cInfo->symbols = symbolMap;

    return 0;
}

int collectInfo_add(collectInfo *cInfo, char *symbol) {
    pair_kv pair;

    if (isNullPtr(cInfo))
        return FALSE;
    
    PAIR_KEY_SET(pair, (pair_key_base *)symbolKeyConst(symbol));
    PAIR_VAL_SET(pair, (pair_val_base *)symbolValConst(symbol));
    return hashMapPut(cInfo->symbols, pair);
}

int collectInfo_del(collectInfo *cInfo, char *symbol) {
    pair_kv pair;

    if (isNullPtr(cInfo))
        return FALSE;

    PAIR_KEY_SET(pair, (pair_key_base *)symbolKeyConst(symbol));
    PAIR_VAL_SET(pair, (pair_val_base *)symbolValConst(symbol));
    return hashMapDelete(cInfo->symbols, pair);
}

#ifdef MIB2DOC_UNIT_TESTING

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

int identListTesting() {
    return 0;
}

int collectInfoTesting() {
    return 0;
}

#endif /* MIB2DOC_UNIT_TESTING */

/* End of file */

