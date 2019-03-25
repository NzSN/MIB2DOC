#ifndef _OPTIONS_H_
#define _OPTIONS_H

#include "list.h"
#include "hash.h"

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

typedef struct optIter {
    listNode *node;
} optIter;

typedef struct optionMng {
    hashMap *options;  
} optionMng;

int optionsInit(int argc, char *argv[]);

/* Prototypes */
optionMng * optMngConst(void);
int optMngRelease(optionMng *mng);
optionMng * optMngDup(optionMng *mng);
options_t * optMngGetOpt(optionMng *mng, char *optName);
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

#ifdef MIB2DOC_UNIT_TESTING

void * option_Basic(void **state);

#endif

#endif /* _OPTIONS_H_ */
