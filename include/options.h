#ifndef _OPTIONS_H_
#define _OPTIONS_H

#include "list.h"
#include "hash.h"

/* Type define */
#define OPT_KEY_SRC_MIB_FILE "SOURCE_MIB"
#define OPT_KEY_TARGET_PDF   "TARGET_PDF"
#define OPT_KEY_INCLUDE_PATH "INCLUDE_PATH"
#define OPT_KEY_TREE_PRINT   "TREE_PRINT"

typedef struct optionVal {
    char *val;
    listNode node;
} optionVal;

/* incPathList includePath;
 * char *sourceMibFilePath;
 * char *targetPdfPATH;
 * int fontSize; */
typedef struct option_t {
    char *optionName; 
    optionVal *vals; 
} options_t;

typedef struct optionMng {
    hashMap *options;  
} optionMng;

// fixme: first to implement hashMap iterator.
typedef struct optIter {} optIter;

typedef struct optValIter {
    options_t *opt;
    listNode *node; 
} optValIter;

int optionsInit(int argc, char *argv[]);

/* Prototypes */
optionMng * optMngConst(void);
int optMngRelease(optionMng *mng);
optionMng * optMngDup(optionMng *mng);
options_t * optMngGetOpt(optionMng *mng, char *optName);
char * optMngGetOptVal(optionMng *mng, char *optName);
int optMngIsOptSet(optionMng *mng, char *optName);
int optMngAddOpt(optionMng *mng, char *optName);
int optMngDelOpt(optionMng *mng, char *optName);
int optMngAppendOptVal(optionMng *mng, char *optName, char *value);
int optMngDelOptVal(optionMng *mng, char *optName, char *value);
int optionMngNumOfOpt(optionMng *mng);

options_t * optionConst(void);
options_t * optionInit(options_t *opt);
int optionIsEqual(options_t *opt_l, options_t *opt_r);
int optionRelease(options_t *opt);
options_t *optionDup(options_t *opt);
int optionAddValue(options_t *opt, char *value);
int optionDelValue(options_t *opt, char *value);

optValIter * optionIter(options_t *opt);
const char * optionNext(optValIter *iter);
int optionRewind(optValIter *iter);

#ifdef MIB2DOC_UNIT_TESTING

void * option_Basic(void **state);

#endif

extern optionMng *optionsManager;

#endif /* _OPTIONS_H_ */
