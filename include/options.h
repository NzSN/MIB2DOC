#ifndef _OPTIONS_H_
#define _OPTIONS_H

#include "list.h"

typedef struct options {
    listNode *includePath;
    char *sourceMibFilePath;
    cahr *targetPdfPATH;
    int fontSize;
} options;

int optionsInit(char *argv[], int argc);

#endif /* _OPTIONS_H_ */
