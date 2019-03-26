/* optRegister.h */

#ifndef _OPT_REGISTER_H_
#define _OPT_REGISTER_H_

#include "util.h"

typedef struct optAttr {
    int withArgs; 
    int index;
    // String
    // Number 
    char *type;
} optAttr_t;

typedef struct optRegister {
    char *optName;
    optAttr_t attr;
} optRegister_t;

typedef enum optionNumber {
    SourceMibFile = 0,
    TargetPdfFile,
    IncludePath,
    NumOfOptions
} optionNumber;


static optRegister_t registerTable[] = {
    { "-s", { TRUE, "String", SourceMibFile } }, /* Source mib file */
    { "-t", { TRUE, "String", TargetPdfFile } }, /* Target pdf file */
    { "-I", { TRUE, "String", IncludePath   } }, /* Include path    */
    { "N/A", { FALSE, "Number", NumOfOptions } } /* End */
};

#define optMatch(O) ({\
    int i = 0;\
    char *optName;\
    optRegister_t *optReg = registerTable[0];\
    optName = optReg->optName;\
    \
    while (!isStringEqual(optReg->optName, "N/A")) {\
        if (isStringEqual(optReg->optName, O))\
            break;\
        optReg = registerTable[++i];\
        optName = optReg->optName;\
    }\
    optReg;\
})

#define optAttr(O) ({\
    optRegister_t *optReg = optMatch(O);\
})

#endif /* _OPT_REGISTER_H_ */

