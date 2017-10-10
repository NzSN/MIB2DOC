#ifndef _OPTIONS_H_
#define _OPTIONS_H

#include "list.h"

typedef struct incPathList {
    char *path;
    listNode node;
} incPathList;

typedef struct options {
    incPathList includePath;
    char *sourceMibFilePath;
    char *targetPdfPATH;
    int fontSize;
} options_t;

int optionsInit(int argc, char *argv[]);
const char * getOption_SourceMibFilePath();
const char * getOption_targetPdfPath();
const char * getOption_includePath(int *index);

#endif /* _OPTIONS_H_ */
