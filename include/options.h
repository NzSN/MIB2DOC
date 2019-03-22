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

typedef struct optionMng {
    hashMap *options;  
} optionMng;

int optionsInit(int argc, char *argv[]);
const optionVal *  getOption(char *);

#endif /* _OPTIONS_H_ */
