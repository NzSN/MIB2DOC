/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "options.h"
#include "list.h"
#include "string.h"
#include "util.h"

/* Declaration */
static int paramMapping(char *param);
static void mappingTableInit();

options_t * optionConst(void);
options_t * optionInit(options_t *opt);
int optionIsEqual(options_t *opt_l, options_t *opt_r);
int optionRelease(options_t *opt);
options_t * optionDup(options_t *opt);
int optionAddValue(options_t *opt, char *value);
int optionDelValue(options_t *opt, char *value);

typedef struct option_hash_key {
    pair_key_base base;
    char *key;
} option_hash_key;

typedef struct option_hash_val {
    pair_val_base base;
    options_t  *option;
} option_hash_val;

static option_hash_key * keyConst();
static option_hash_key * keyInit(option_hash_key *key);
static int keyRelease(option_hash_key *key);
static void * keyValue(option_hash_key *key);
static int keyMatch(option_hash_key *key_l, option_hash_key *key_r);
struct option_hash_key * keyCopy(option_hash_key *key);

static option_hash_val * valConst();
static option_hash_val * valInit(option_hash_val *val);
static int valRelease(option_hash_val *val);
static void * valValue(option_hash_val *val);
static int valIsEqual(option_hash_val *val_l, option_hash_val *val_r);
static option_hash_val * valCopy(option_hash_val *val);

static void helpInfoPrint();
static int optionHashing(option_hash_key *key);


/* Global Var */
#define OPT_HASH_MAP_SIZE 20
options_t * optionsManager;

/* Local Var */
typedef enum optionNumber {
    SourceMibFile = 0,
    TargetPdfFile,
    UNIQUE_PARAM, /* params above this line is unique another is not */
    IncludePath,
    NumOfOptions
} optionNumber;

static char * mappingTable[NumOfOptions];

/* Local Functions */

int optionsInit(int argc, char *argv[]) {
    char *param;
    char *paramVal;
    int i=0, paramIndex = 0;

    if (isNullPtr(argv)) {
        return ERROR_NULL_REF;
    }
    
    if (argc == 1) {
        helpInfoPrint();
        return ERROR_GENERIC;
    }
    

    mappingTableInit();

    /*
     * Note : should deal with error that no paramVal for
     *        an option, such as only give a "-s" but not
     *        with the value should after the "-s".
     */
    while (param = argv[i++]) {
        if ((paramIndex = paramMapping(param)) < UNIQUE_PARAM) {
            if (mappingTable[paramIndex] == null) {
                return ERROR_GENERIC;
            }
        }
        switch(paramMapping(param)) {
            case SourceMibFile:
                break;
            case TargetPdfFile:
                break;
            case IncludePath:
                break;
            default:
                helpInfoPrint();
                return ERROR_WRONG_IDX;
        }

        if (i > argc-1)
            break;
    }
}

const optionVal * getOption(char *opName) {
    if (isNullPtr(opName)) return NULL;

}

static void helpInfoPrint() {}

static void mappingTableInit() {
    mappingTable[SourceMibFile] = "-s";
    mappingTable[TargetPdfFile] = "-t";
    mappingTable[IncludePath] = "-I";

    return;
}

static int paramMapping(char *param) {
    int index = 0;

    if (isNullPtr(param)) {
        return ERROR_NULL_REF;
    }

    while (index < NumOfOptions) {
        if (strncmp(param, mappingTable[index], strlen(param)) == 0) {
            break;
        }
        index++;
    }
    return index;
}

/* optionMng 
 * . Construction
 * . Destruction
 * . CopyConstruction
 * . Selectors
 * */
optionMng * optMngConst(void) {
    optionMng *mng = (optionMng *)Malloc(sizeof(optionMng));

    mng->options = hashMapConstruct(OPT_HASH_MAP_SIZE, (hashFunction)optionHashing);
    return mng;
}

int optMngRelease(optionMng *mng) {
    if (isNullPtr(mng)) return ERROR;
    
    return hashMapRelease(mng->options);    
}

optionMng * optMngDup(optionMng *mng) {
    if (isNullPtr(mng)) return NULL;

    optionMng *mng_copy = (optionMng *)Malloc(sizeof(optionMng));

    hashMap *map = hashMapDup(mng->options);
    if (isNullPtr(map)) {
        RELEASE_MEM(mng_copy);
        return NULL;
    }
    mng_copy->options = map;

    return mng_copy;
}

options_t * optMngGetOpt(optionMng *mng, char *optName) {
    if (isNullPtr(mng) || isNullPtr(optName))
        return NULL;
    
    option_hash_key key;
    keyInit(&key);
    key.key = optName;
      
    return hashMapGet(mng->options, (pair_key_base *)&key);
}

int optMngAddOpt(optionMng *mng, char *optName) {
    if (isNullPtr(mng) || isNullPtr(optName))
        return ERROR;
    
    option_hash_key key;
    option_hash_val *val;

    keyInit(&key);
    key.key = optName;

    val->option = optionConst();
    val->option->optionName = optName;

    return hashMapPut(mng->options, (pair_key_base *)&key, (pair_val_base *)val);
}

int optMngDelOpt(optionMng *mng, char *optName) {
    if (isNullPtr(mng) || isNullPtr(optName))
        return ERROR;
    
    option_hash_key key;
    option_hash_val *val;
    
    keyInit(&key);
    key.key = optName; 

    if (hashMapDelete(mng->options, (pair_key_base *)&key) == ERROR)
        return ERROR;

    return OK;
}

int optMngAppendOptVal(optionMng *mng, char *optName, char *value) {
    if (isNullPtr(mng) || isNullPtr(optName) || isNullPtr(value))
        return ERROR;

    option_hash_key key;
    keyInit(&key);
    key.key = optName;

    option_hash_val *val;
    val = hashMapGet(mng->options, (pair_key_base *)&key);

    if (optionAddValue(val->option, value) == ERROR)
        return ERROR;

    return OK;
}

int optMngDelOptVal(optionMng *mng, char *optName, char *value) {
    if (isNullPtr(mng) || isNullPtr(optName) || isNullPtr(value))
        return ERROR;

    option_hash_key key;
    keyInit(&key);
    key.key = optName;

    option_hash_val *val;
    val = hashMapGet(mng->options, (pair_key_base *)&key);

    if (optionDelValue(val->option, value) == ERROR)
        return ERROR;

    return OK;
}

int optionMngNumOfOpt(optionMng *mng) {
    if (isNullPtr(mng))
        return ERROR;

    return mng->options->size;
}

/* options_t */
options_t * optionConst(void) {
    options_t *opt = (options_t *)Malloc(sizeof(options_t));
    return optionInit(opt);
}

options_t * optionInit(options_t *opt) {
    if (isNullPtr(opt)) return NULL;

    opt->optionName = NULL;
    opt->vals = NULL; 
    
    return opt;
}

static _Bool isOptValEqual(const listNode *node_l, const listNode *node_r) {
    optionVal *val_l, *val_r;
    
    if (isNullPtr(node_l) || isNullPtr(node_r))
        return ERROR;

    val_l = containerOf(node_l, optionVal, node);
    val_r = containerOf(node_r, optionVal, node);

    _Bool isEqual = isStringEqual(val_l->val, val_r->val); 

    return isEqual;
}
int optionIsEqual(options_t *opt_l, options_t *opt_r) {
    if (isNullPtr(opt_l) || isNullPtr(opt_r))
      return ERROR;  

    _Bool isNameEqual = isStringEqual(opt_l->optionName, opt_r->optionName); 
    _Bool isValsEqual = listNodeIsEqual(&opt_l->vals->node, &opt_r->vals->node, isOptValEqual); 

    return isNameEqual && isValsEqual;
}

static int optValRelease(const listNode *node, void *args) {
    if (isNullPtr(node))
        return ERROR;

    optionVal *val = containerOf(node, optionVal, node);
    RELEASE_MEM(val->val);
    RELEASE_MEM(val);

    return OK;
}
int optionRelease(options_t *opt) {
    if (isNullPtr(opt))
        return ERROR;

    RELEASE_MEM(opt->optionName);

    listNodeTravel(&opt->vals->node, (listNodeTask)optValRelease, NULL);
    RELEASE_MEM(opt);

    return OK;
}

static int optValDup(const listNode *node, void *args) {
    if (isNullPtr(node) || isNullPtr(args)) 
        return ERROR;
    
    listNode *head = args; 
    optionVal *val = containerOf(node, optionVal, node);
    
    optionVal *val_copy = (optionVal *)Malloc(sizeof(optionVal));
    memset(val_copy, 0, sizeof(optionVal));
    val_copy->val = strdup(val->val);
     
    listNodeAppend(head, &val_copy->node);

    return OK;
}
options_t * optionDup(options_t *opt) {
    if (isNullPtr(opt))
     return NULL;
    
    listNode head;
    memset(&head, 0, sizeof(listNode)); 

    options_t *opt_copy = optionConst();
    opt_copy->optionName = strdup(opt->optionName);
    listNodeTravel(&opt->vals->node, (listNodeTask)optValDup, (void *)&head);
    opt_copy->vals = containerOf(listNodeNext(&head), optionVal, node);
    
    return opt_copy;
}

int optionAddValue(options_t *opt, char *value) {
    if (isNullPtr(opt) || isNullPtr(value))
        return ERROR;
        
    optionVal *newVal = (optionVal *)Malloc(sizeof(optionVal));
    memset(newVal, 0, sizeof(optionVal));
    newVal->val = value;
    
    listNodeAppend(&opt->vals->node, &newVal->node);

    return OK;
}

static _Bool optValSearchHelper(const listNode *aNode, void *args) {
    if (isNullPtr(aNode) || isNullPtr(args))
        return ERROR;
    
    optionVal *val = containerOf(aNode, optionVal, node); 
    return isStringEqual(val->val, (char *)args); 
}
int optionDelValue(options_t *opt, char *value) {
    if (isNullPtr(opt) || isNullPtr(value)) 
        return ERROR;
    
    listNode *foundNode = listNodeSearch(&opt->vals->node, (listNodeCmp)optValSearchHelper, (void *)value);
    optionVal *val = containerOf(foundNode, optionVal, node); 

    listNodeDelete(foundNode); 
    RELEASE_MEM(val->val);
    RELEASE_MEM(val);

    return OK;
}

/* options hash key, value implementations */

/* option_hash_key members */
static option_hash_key * keyConst() {
    option_hash_key *key = (option_hash_key *)Malloc(sizeof(option_hash_key));
 
    return keyInit(key);
}

static option_hash_key * keyInit(option_hash_key *key) {
    if (isNullPtr(key)) return NULL; 

    key->base.release = (pairKeyRelease)keyRelease;
    key->base.value = (pairKeyValue)keyValue;
    key->base.isEqual = (pairKeyIsEqual)keyMatch;
    key->base.copy = (pairKeyCopy)keyCopy;

    return key;
}

static int keyRelease(option_hash_key *key) {
    if (isNullPtr(key)) return ERROR;

    RELEASE_MEM(key->key);

    return OK;
}

static void * keyValue(option_hash_key *key) {
    if (isNullPtr(key)) return NULL;

    return (void *)key->key;
}

static int keyMatch(option_hash_key *key_l, option_hash_key *key_r) {
    if (isNullPtr(key_l) || isNullPtr(key_r))
        return ERROR;

    int match = isStringEqual(key_l->key, key_r->key);

    return match;
}

struct option_hash_key * keyCopy(option_hash_key *key) {
    if (isNullPtr(key)) return NULL;

    option_hash_key *key_copy = (option_hash_key *)Malloc(sizeof(option_hash_key));
    key_copy->key = strdup(key->key); 

    return key_copy;
}

/* option_hash_val members */
static option_hash_val * valConst() {
    option_hash_val *val = (option_hash_val *)Malloc(sizeof(option_hash_val));

    return valInit(val);
}

static option_hash_val * valInit(option_hash_val *val) {
    if (isNullPtr(val)) return NULL;
    
    val->base.release = (pairValRelease)valRelease;
    val->base.isEqual = (pairValIsEqual)valIsEqual;
    val->base.copy = (pairValCopy)valCopy;
    val->base.value = (pairValValue)valValue;
    
    return val;
}

static int valRelease(option_hash_val *val) {
    if (isNullPtr(val)) return ERROR;
     
    if (val->option) 
        return optionRelease(val->option);

    return OK;
}

static void * valValue(option_hash_val *val) {
    if (isNullPtr(val)) return NULL;
    return val->option;
}

static int valIsEqual(option_hash_val *val_l, option_hash_val *val_r) {
    if (isNullPtr(val_l) || isNullPtr(val_r)) 
        return ERROR;

    return optionIsEqual(val_l->option, val_r->option);
}

static option_hash_val * valCopy(option_hash_val *val) {
    if (isNullPtr(val)) return NULL;
    
    option_hash_val *copy = valConst();
    copy->option = optionDup(val->option); 

    return copy;
}

/* Hash function */
static int optionHashing(option_hash_key *key) {
    if (isNullPtr(key)) return ERROR;
    
    char *str = key->key;
    
    int index = 0, size, value, hashValue;
    size = strlen(str);

    while (index < size) {
        value = str[index];
        hashValue += (value << 5) + value; 
        ++index;
    }
    
    return hashValue;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void * option_Basic(void **state) {
    optionMng *mng = optMngConst();

}

#endif /* MIB2DOC_UNIT_TESTING */

/* options.c */
