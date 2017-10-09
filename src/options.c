/* options.c */

#include <stdio.h>
#include "options.h"

/* Declaration */
static int paramMapping(char *param);
static void mappingTableInit();
/* Local Var */
typedef enum optionNumber {
    SourceMibFile = 0,
    TargetPdfFile,
    NumOfOptions
} optionNumber;

static char * mappingTable[NumOfOptions];

int optionsInit(char *argv[], int argc) {
    char *param;
    int i=0;

    if (IS_PTR_NULL(argv)) {
        return -1;
    }

    if (argc == 0) {
        printf("usage : mib2doc [options] <mibfilePath>");
        return -1;
    }

    mappingTableInit();

    while (param = argv[i++]) {
        switch(paramMapping(param)) {
            case SourceMibFile:
                /* Deal with param */
                break;
            case TargetPdfFile:
                /* Deal with param */
                break;
        }

        if (i > argc-1)
            break;
    }
}

static void mappingTableInit() {
    mappingTable[SourceMibFile] = "-s";
    mappingTable[TargetPdfFile] = "-t";
    return;
}

static int paramMapping(char *param) {

}

/* options.c */
