/* options.c */

#include <stdio.h>
#include <stdlib.h>
#include "type.h"
#include "options.h"
#include "list.h"
#include "string.h"

/* Declaration */
static int paramMapping(char *param);
static void mappingTableInit();

/* Global Var */
options_t optionsManager;

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
static void helpInfoPrint();

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


/* options hash key, value implementations */
typedef struct option_hash_key {
    pair_key_base base;
    char *key;
} option_hash_key;

typedef struct option_hash_val {
    pair_val_base base;
    options_t  *option;
} option_hash_val;

/* Hash function */


/* options.c */
