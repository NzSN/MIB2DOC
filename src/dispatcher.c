/* Created by Aydenlin at 2017/9/12 */

#include <string.h>
#include "dispatcher.h"
#include "hash.h"
#include "mibTreeGen.h"
#include "stack.h"
#include "type.h"
#include <list.h>
#include <options.h>
#include <stdlib.h>
#include <util.h>

/* Declaration Section */
static void debugging(dispatchParam* param);
static int dispatchMakeChoice(dispatch_type dType);
static char * lexBufferSwitching(char* newModule);
static int symbolHashing(void* key);
static int switchInit();

/* Global */
extern slice sliceContainer;
static unsigned char isNeedSwitchInit = TRUE;
switchingState swState;

typedef struct {
    pair_key_base base;
    char* symbolIndex;
} symbolKey;

typedef struct {
    pair_val_base base;
    char* symbol;
} symbolVal;

static symbolKey * symbolKeyInit(symbolKey *key, char *symbols);
static symbolVal * symbolValInit(symbolVal *val, char *symbol);

static symbolKey * symbolKeyCopy(symbolKey *origin);
static symbolVal * symbolValCopy(symbolVal *origin);


/* Definition Section */
int dispatchInit() {
    switchInit();
    return ERROR_NONE;
}

int dispatch(dispatch_type disType, dispatchParam* param) {
    int symCollectType;
    unsigned long key;
    char *val, *moduleName, *sCollection;
    errorType ret = ERROR_NONE;

    if (isNullPtr(param)) {
        return ERROR_NULL_REF;
    }

    switch (dispatchMakeChoice(disType)) {
    case DISPATCH_PARAM_STAGE:
        key = (unsigned long)disParamGet(disParamRetrive(&param));
        val = (char*)disParamGet(disParamRetrive(&param));
        sliceStore(&sliceContainer, sliceConstruct(key, val));
        break;
    case MIBTREE_GENERATION:
        mibObjGen((unsigned long)disParamGet(disParamRetrive(&param)));
        break;
    case SYMBOL_COLLECTING:
        symCollectType = (unsigned long)disParamGet(disParamRetrive(&param));
        symbolCollecting(symCollectType, param);
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

static void debugging(dispatchParam* param) {
    dispatchParam* result;
    unsigned long key;
    char* val;

    result = disParamRetrive(&param);
    if (result == NULL)
        return;
    key = (unsigned long)result->param;
    if (key == 1)
        key = 1;
    result = disParamRetrive(&param);
    if (result == NULL)
        return;

    val = (char*)result->param;

    if (IS_VALID_SLICE_TYPE(key)) {
        printf("DEBUGGING\n");
        printf("Key:value Pair is %lu:%s\n", key, val);
    }
}

static int dispatchMakeChoice(dispatch_type dType) {
    int choice = -1;

    if (SW_STATE(&swState) == DISPATCH_MODE_DOC_GENERATING) {
        choice = dType;
    } else if (SW_STATE(&swState) == DISPATCH_MODE_SYMBOL_COLLECTING) {
        if (dType == DISPATCH_PARAM_STAGE || dType == MIBTREE_GENERATION) {
            choice = SYMBOL_COLLECTING;
        } else {
            /* SYMBOL_COLLECTING, SWITCH_TO_INC_BUFFER no redirect */
            choice = dType;
        }
    }
    if (SW_STATE(&swState) == DISPATCH_MODE_DEBUG)
        choice = DEBUGING;

    return choice;
}

// Following codes is added for import feature.
static int switchInit() {
    int retVal;
    
    if (!isNeedSwitchInit) return OK;
    
    memset(&swState, 0, sizeof(switchingState));
         
    char *srcMibPath = optMngGetOptVal(optionsManager, OPT_KEY_SRC_MIB_FILE);
    SW_CUR_SET_FILE_NAME(&swState, srcMibPath); 

    SW_COUNTER_SET(&swState, 0);
    SW_STATE_SET(&swState, DISPATCH_MODE_DOC_GENERATING);

    SW_CUR_SET_PURPOSE(&swState, SWITCHING_GEN_PURPOSE);

    // Todo: replace the magic number '128' with a macro to get a meaning of it.
    genericStackConstruct(&swState.swStack, 128 * sizeof(switchInfo), sizeof(switchInfo));
    SW_CUR_SET_SYMBOLS(&swState, hashMapConstruct(10, symbolHashing));

    isNeedSwitchInit = FALSE;

    return retVal;
}

int switchToModule(switchingState *swState, char* moduleName) {
    char *path;
    collectInfo* cInfo;

    if (isNullPtr(swState) || isNullPtr(moduleName)) {
        return FALSE;
    }

    // Step 1: Push currentSwitchInfo into stack
    SW_SET_CUR_BUFFER_INFO(swState, getCurrentBufferState());
    push(SW_STACK(swState), SW_CUR_SWITCH_INFO(swState));

    // Step 2: Update currentSwitchInfo
    if ( !(path = lexBufferSwitching(moduleName)) )
        /* Terminate program */
        return ABORT;
    
    SW_CUR_SET_FILE_NAME(swState, path);
    SW_SET_CUR_BUFFER_INFO(swState, getCurrentBufferState());

    return 0;
}

int switchToPrevModule(switchingState *swState) {
    if (isNullPtr(swState))
        return -1;

    // There is no more module to switch to just exit.
    if (SW_STACK(swState)->top == 0) {
        return -1;
    }
    
    // Release resources use by current info structure
    collectInfo_release(SW_CUR_IMPORT(swState));
    yy_delete_buffer(SW_CUR_BUFFER_INFO(swState));
    RELEASE_MEM(SW_CUR_FILE_NAME(swState)); 
    
    // Switch to prev module
    pop(SW_STACK(swState), SW_CUR_SWITCH_INFO(swState));
    yy_switch_to_buffer(SW_CUR_BUFFER_INFO(swState));

    SW_STATE_REFRESH((*swState));

    return 0;
}

static char * lexBufferSwitching(char* newModule) {
    int index = 0, len;
    const char* path;
    char* targetModulePath;
    
    /* Build path similar to ./path/to/module.my */ 
    path = optMngGetOptVal(optionsManager, OPT_KEY_INCLUDE_PATH);
    if (isNullPtr(path)) path = ".";
    
    /* len = strlen(path) + strlen("/") + strlen(newModule) + <len of module extension> + null */
    len = strlen(path) + strlen(newModule) + 1 + 5 + 1;
    targetModulePath = (char *)Malloc(len);
    memset(targetModulePath, 0, len);
    
    strncat(targetModulePath, path, len); 
    strncat(targetModulePath, "/", len);
    strncat(targetModulePath, newModule, len);   
    strncat(targetModulePath, ".my", len);

    yyin = fopen(targetModulePath, "r");
    if (!yyin) 
        abortWithMsg("Can't not open file : %s\n", targetModulePath);

    yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));

    return targetModulePath;;
}

/**************************
 *  collectInfo functions *
 **************************/
#define SYMBOL_SEPERATOR ','

static int symbolHashing(void* key) {
    int hashVal = 0;
    char* keyString;
    symbolKey* sKey = (symbolKey*)key;

    if (isNullPtr(key))
        return ERROR;

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

static symbolKey * symbolKeyCopy(symbolKey *origin) {
    symbolKey *copy;

    if (isNullPtr(origin))
        return NULL;

    copy = (symbolKey *)malloc(sizeof(symbolKey));

    return symbolKeyInit(copy, strdup(origin->symbolIndex));
}

static symbolVal * symbolValCopy(symbolVal *origin) {
    symbolVal *copy;

    if (isNullPtr(origin))
        return NULL;

    copy = (symbolVal *)malloc(sizeof(symbolVal));

    return symbolValInit(copy, strdup(origin->symbol));
}

static int symbolKeyEqual(symbolKey* lSymbol, symbolKey* rSymbol) {
    int lLength, rLength;

    char* lStr = lSymbol->symbolIndex;
    char* rStr = rSymbol->symbolIndex;

    if (isNullPtr(lStr) || isNullPtr(rStr))
        return FALSE;

    lLength = lLength > rLength ? lLength : rLength;
    return !strncmp(lStr, rStr, lLength);
}

static int symbolValEqual(symbolVal* lSymbol, symbolVal* rSymbol) {
    int lLength, rLength;

    if (isNullPtr(lSymbol) || isNullPtr(rSymbol))
        return FALSE;

    char* lStr = lSymbol->symbol;
    char* rStr = rSymbol->symbol;

    if (isNullPtr(lStr) || isNullPtr(rStr))
        return FALSE;

    lLength = lLength > rLength ? lLength : rLength;
    return !strncmp(lStr, rStr, lLength);
}

static char * symbolKeyValue(symbolKey* symbol) {
    if (isNullPtr(symbol))
        return NULL;

    return symbol->symbolIndex;
}

static char * symbolValValue(symbolVal *symbol) {
    if (isNullPtr(symbol))
        return NULL;
    return symbol->symbol;
}

static int symbolKeyRelease(symbolKey *symbol) {
    if (isNullPtr(symbol))
        return FALSE;

    if (!isNullPtr(symbol->symbolIndex))
        RELEASE_MEM(symbol->symbolIndex);

    RELEASE_MEM(symbol);

    return TRUE;
}

static int symbolValRelease(symbolVal *symbol) {
    if (isNullPtr(symbol))
        return FALSE;

    if (!isNullPtr(symbol->symbol))
        RELEASE_MEM(symbol->symbol);

    RELEASE_MEM(symbol);

    return TRUE;
}

static symbolKey * symbolKeyInit(symbolKey *key, char *symbols) {
    pair_key_base *base;

    if (isNullPtr(key) || isNullPtr(symbols))
        return NULL;
    base = &key->base;

    // base init
    base->isEqual = (pairKeyIsEqual)symbolKeyEqual;
    base->release = (pairKeyRelease)symbolKeyRelease;
    base->value = (pairKeyValue)symbolKeyValue;
    base->copy = (pairKeyCopy)symbolKeyCopy;

    key->symbolIndex = symbols;

    return key;
}

static symbolKey * symbolKeyConst(char* symbols) {
    symbolKey* key;

    if (isNullPtr(symbols))
        return NULL;
    key = (symbolKey *)malloc(sizeof(symbolKey));
    return symbolKeyInit(key, symbols);
}

static symbolVal * symbolValInit(symbolVal *val, char *symbol) {
    pair_val_base *base;

    if (isNullPtr(val) || isNullPtr(symbol))
        return NULL;

    base = &val->base;

    // base init
    base->isEqual = (pairValIsEqual)symbolValEqual;
    base->release = (pairValRelease)symbolValRelease;
    base->value = (pairValValue)symbolValValue;
    base->copy = (pairValCopy)symbolValCopy;

    val->symbol = symbol;
    return val;
}

static symbolVal* symbolValConst(char* symbol) {
    symbolVal* val;

    if (isNullPtr(symbol))
        return NULL;

    val = (symbolVal *)malloc(sizeof(symbolVal));
    return symbolValInit(val, symbol);
}

int collectInfoInit(collectInfo* cInfo, char* modName) {
    hashMap* symbolMap;

    if (isNullPtr(cInfo))
        return -1;

    symbolMap = hashMapConstruct(100, symbolHashing);

    cInfo->symbols = symbolMap;
    cInfo->modName = modName;

    return 0;
}

collectInfo * collectInfoConst(char *modName) {
    collectInfo *cInfo;

    if (isNullPtr(modName))
        return FALSE;

    cInfo = (collectInfo *)malloc(sizeof(collectInfo));
    collectInfoInit(cInfo, modName);

    return cInfo;
}

int collectInfo_add(collectInfo* cInfo, char* symbol) {
    if (isNullPtr(cInfo))
        return FALSE;

    return hashMapPut(cInfo->symbols,
                      (pair_key_base *)symbolKeyConst(strdup(symbol)),
                      (pair_val_base *)symbolValConst(symbol));
}

int collectInfo_del(collectInfo* cInfo, char* symbol) {
    symbolKey key;
    if (isNullPtr(cInfo))
        return FALSE;

    symbolKeyInit(&key, symbol);
    return hashMapDelete(cInfo->symbols, (pair_key_base *)&key);
}

char* collectInfo_retrive(collectInfo* cInfo, char* symbol) {
    symbolKey key;
    pair_val_base* val;

    if (isNullPtr(cInfo))
        return FALSE;

    symbolKeyInit(&key, symbol);

    val = (pair_val_base*)hashMapGet(cInfo->symbols, (pair_key_base *)&key);
    return (char *)val->value(val);
}

int collectInfo_exists(collectInfo *cInfo, char *symbol) {
    symbolKey key;

    if (isNullPtr(cInfo) || isNullPtr(symbol))
        return FALSE;

    symbolKeyInit(&key, symbol);

    if (hashMapGet(cInfo->symbols, (pair_key_base *)&key))
        return TRUE;
    else
        return FALSE;
}

int collectInfo_release(collectInfo *cInfo) {
    if (isNullPtr(cInfo))
        return FALSE;

    if (cInfo->modName)
        RELEASE_MEM(cInfo->modName);
    if (cInfo->symbols)
        hashMapRelease(cInfo->symbols);

    return TRUE;
}

char * switch_CurrentMod() {
    return SW_CUR_IMPORT(&swState)->modName;
}

int importWorks(genericStack *importInfoStack) {
    char *modName;
    hashMap *symbolMap;
    collectInfo *infoCollect;

    if (isNullPtr(importInfoStack))
        return FALSE;

    while (pop(importInfoStack, &infoCollect) == 0) {
        modName = infoCollect->modName;
        symbolMap = infoCollect->symbols;

        if (switchToModule(&swState, modName) == ABORT)
            abortWithMsg("%s: No such Mib file.\n", modName);

        SW_STATE_SET(&swState, DISPATCH_MODE_SYMBOL_COLLECTING);
        SW_CUR_SWITCH_INFO(&swState)->purpose = SWITCHING_INC_PURPOSE;

        SW_CUR_IMPORT(&swState)->modName = strdup(modName);
        SW_CUR_IMPORT(&swState)->symbols = hashMapDup(symbolMap);

        // Clean
        collectInfo_release(infoCollect);
        RELEASE_MEM(infoCollect);
    }

    return TRUE;
}

switchingState * getCurSwInfo() {
    return &swState;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void dispatcher__COLLECT_INFO(void** state) {
    char* symbol;
    collectInfo* cInfo = (collectInfo*)malloc(sizeof(collectInfo));

    collectInfoInit(cInfo, "try");

    // CollectInfo add and retrive testing.
    collectInfo_add(cInfo, "try_symbol1");
    symbol = collectInfo_retrive(cInfo, "try_symbol1");
    assert_string_equal(symbol, "try_symbol1");

    return;
}

void dispatcher__SYMBOLTABLE(void **state) {

}

#endif /* MIB2DOC_UNIT_TESTING */

/* End of file */
