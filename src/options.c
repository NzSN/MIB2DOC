/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "options.h"
#include "list.h"
#include "string.h"
#include "util.h"

/* Register implementation */
#include "optRegister.h"

/* Declaration */
static int paramMapping(char *param);

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

// Options preprocessing procedures
static _Bool optionValidity(int argc, char *argv[]);
static optPieces optionArrayGen(int argc, char *argv[]);
static int optionProcessing(char ** *optArray);


/* Global Var */
#define OPT_HASH_MAP_SIZE 20
optionMng *optionsManager;

/* Local Var */

/* Steps to deal with options:
 * 1.Check option's validity
 * 2.Transfer options into [][2] array type.
 * 3.Deal with every [2] array.
 */
int optionsInit(int argc, char *argv[]) {
    int i=0, NumOfOptions;
    optPieces optArray;   // optArray[][]

    if (isNullPtr(argv)) {
        return ERROR_NULL_REF;
    }
       
    optionsManager = optMngConst(); 

    if (optionValidity(argc, argv) == FALSE) {
        helpInfoPrint();
        exit(1);
    }
    
    optArray = optionArrayGen(argc, argv);
    if (isNullPtr(optArray))
        abortWithMsg("Option array generation error...\n");
    
    if (optionProcessing(optArray) == ERROR)
        abortWithMsg("Option processing error...\n");

    return OK;
}

static _Bool optionValidity(int argc, char *argv[]) {
    return argumentChecking(argc, argv); 
}

static optPieces optionArrayGen(int argc, char *argv[]) {
    if (isNullPtr(argv))
        return NULL;
    
    return argumentSplit(argc, argv);
}

static int optionProcessing(optPieces optArray) {
    if (isNullPtr(optArray)) return ERROR;
    
    int index = 0;
    optHalfPiece optVal;
    optPiecesIter *iter_opt = optPiecesGetIter(optArray);

    while (optVal = optPiecesNext(iter_opt)) {
        switch(paramMapping(optVal)) {
            case SourceMibFile:
                optMngAddOpt(optionsManager, OPT_KEY_SRC_MIB_FILE);
                optMngAppendOptVal(optionsManager, OPT_KEY_SRC_MIB_FILE, optPiecesNext(iter_opt));
                break;
            case TargetPdfFile:
                optMngAddOpt(optionsManager, OPT_KEY_TARGET_PDF);
                optMngAppendOptVal(optionsManager, OPT_KEY_TARGET_PDF, optPiecesNext(iter_opt)); 
                break;
            case IncludePath:
                optMngAddOpt(optionsManager, OPT_KEY_INCLUDE_PATH);
                optMngAppendOptVal(optionsManager, OPT_KEY_INCLUDE_PATH, optPiecesNext(iter_opt));
                break;
        }        
    }

}  

static void helpInfoPrint(void) {
    printf("options is incorrectly and help info is still pending...\n");
}

static int paramMapping(char *param) {
    int index = 0;
    
    if (isNullPtr(param)) return ERROR;
    
    optAttr_t *attr = optAttr(param);
    return optAttr_idx(attr);
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
      
    option_hash_val *val = hashMapGet(mng->options, (pair_key_base *)&key);
    if (isNullPtr(val)) return NULL;
    return val->option;
}

char * optMngGetOptVal(optionMng *mng, char *optName) {
    options_t *opt = optMngGetOpt(mng, optName);
    if (opt && opt->vals)
        return opt->vals->val;
    return NULL;
}

int optMngAddOpt(optionMng *mng, char *optName) {
    if (isNullPtr(mng) || isNullPtr(optName))
        return ERROR;
    
    option_hash_key *key = keyConst();
    option_hash_val *val = valConst();
     
    key->key = strdup(optName);
    val->option = optionConst();
    val->option->optionName = optName;

    return hashMapPut(mng->options, (pair_key_base *)key, (pair_val_base *)val);
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
    
    if (opt->vals)    
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
    
    if (opt->vals)    
        listNodeAppend(&opt->vals->node, &newVal->node);
    else 
        opt->vals = newVal;  


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

optValIter * optionIter(options_t *opt) {
    if (isNullPtr(opt)) return NULL;

    optValIter *iter = (optValIter *)Malloc(sizeof(optValIter));
    
    listNode *node;  
    if (opt->vals == NULL) 
        node = NULL;
    else
        node = &opt->vals->node;
    
    iter->opt = opt;  
    iter->node = node;

    return iter;
}

const char * optionNext(optValIter *iter) {
    if (isNullPtr(iter)) return NULL;
       
    listNode *aNode = iter->node;
    if (isNullPtr(aNode)) return NULL;

    optionVal *optVal = containerOf(aNode, optionVal, node);  
    
    aNode = listNodeNext(aNode);
    iter->node = aNode;

    return optVal->val;
}

int optionRewind(optValIter *iter) {
    if (isNullPtr(iter)) return ERROR;
    
    listNode *aNode; 
    if (iter->opt->vals) 
        aNode = &iter->opt->vals->node;
    else
        aNode = NULL;
    iter->node = aNode;

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
    
    int index = 0, size, value, hashValue = 0;
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
    
    // Add, Get testing 
    assert_int_equal(optMngAddOpt(mng, strdup("INCLUDE")), TRUE);

    options_t *opt = optMngGetOpt(mng, "INCLUDE");
    assert_non_null(opt);
    assert_string_equal(opt->optionName, "INCLUDE");      

    optMngAppendOptVal(mng, "INCLUDE", strdup("."));
    optMngAppendOptVal(mng, "INCLUDE", strdup(".."));
    
    // Iterator testing
    optValIter *iter = optionIter(opt);
    const char *val = optionNext(iter);
    assert_non_null(val);
    assert_string_equal(val, ".");
    
    val = optionNext(iter);
    assert_non_null(val);
    assert_string_equal(val, "..");  

    val = optionNext(iter);
    assert_non_null(!val); 

    if (optionRewind(iter) == ERROR)
        fail();
    val = optionNext(iter);
    assert_non_null(val);
    assert_string_equal(val, ".");

    // Release tseting
    optMngRelease(mng); 
    
    char *key; 
    int status = 0; 
    mng = optMngConst();

    // Try to add manay option
    unsigned int capacity = 20;
    unsigned int idx = 0, idx_inner = 0;

    while (idx < capacity) {
        key = numberToStr(idx);

        status = optMngAddOpt(mng, key); 
        assert_int_equal(status, TRUE);
        
        while (idx_inner < capacity) {
            optMngAppendOptVal(mng, key, numberToStr(idx_inner));                     
            ++idx_inner;
        }
        idx_inner = 0;
        ++idx;
    }

    // Now try to retrive options and check is it correctly. 
    idx = idx_inner = 0;
    char *val_cmp;
    while (idx < capacity) {
        key = numberToStr(idx);   
        
        opt = optMngGetOpt(mng, key);
        assert_non_null(opt);
        assert_string_equal(opt->optionName, key);
        
        iter = optionIter(opt);
        while (idx_inner < capacity) {
            val = optionNext(iter);         
            val_cmp = numberToStr(idx_inner);
            assert_string_equal(val, val_cmp);

            RELEASE_MEM(val_cmp);
            ++idx_inner; 
        } 
        idx_inner = 0;
        ++idx;
    } 
    
    optMngRelease(mng);

    /* option register testing */  
    optRegister_t *optReg = optMatch("-I"); 
    assert_string_equal(optReg->optName, "-I");  

    optAttr_t *optAttr = optAttr("-I");
    assert_int_equal(optAttr_withArgs(optAttr), TRUE);
    assert_string_equal(optAttr_type(optAttr), "String");
    assert_int_equal(optAttr_idx(optAttr), IncludePath); 
    
    int argc = 5;
    char *argv[] = { "./test", "-s", "/usr/src", "-I", "/usr/include" };

    assert_int_equal(argumentChecking(argc, argv), TRUE); 

    /* option splite testing */ 
    optPieces pieces = argumentSplit(argc, argv);
    assert_string_equal(pieces[0][0], "-s");
    assert_string_equal(pieces[0][1], "/usr/src");
    assert_string_equal(pieces[1][0], "-I");
    assert_string_equal(pieces[1][1], "/usr/include");
    assert_string_equal(pieces[2][0], "T");
    
    /* optPieces iterator testing */
    char *optVal;
    optPiecesIter *iter__ = optPiecesGetIter(pieces);   
    assert_string_equal(optPiecesNext(iter__), "-s");
    assert_string_equal(optPiecesNext(iter__), "/usr/src");
    assert_string_equal(optPiecesNext(iter__), "-I");
    assert_string_equal(optPiecesNext(iter__), "/usr/include");    

    /* option module initializations */
    assert_int_equal(optionsInit(argc, argv), OK);
    optVal = optMngGetOptVal(optionsManager, OPT_KEY_SRC_MIB_FILE);
    assert_string_equal(optVal, "/usr/src");
    optVal = optMngGetOptVal(optionsManager, OPT_KEY_INCLUDE_PATH);
    assert_string_equal(optVal, "/usr/include");
}

#endif /* MIB2DOC_UNIT_TESTING */

/* options.c */
