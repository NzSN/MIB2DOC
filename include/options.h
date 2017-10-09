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

int optionsInit(char *argv[], int argc);

#endif /* _OPTIONS_H_ */
