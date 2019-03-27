/* optRegister.h */

#ifndef _OPT_REGISTER_H_
#define _OPT_REGISTER_H_

#include "util.h"

typedef struct optAttr_t {
    int withArgs; 
    char *type; // String or Number
    int index;
    /* Callbacks of optCheckCallback type 
     * optCheck(f) ::=
     *   Arity(f) = 4 &&
     *   check(f) &&
     *   argValidity(f) &&
     *   argConflictCheck(f) &&
     *   optCheckCond(f)
     *
     * Property(argc: int, current: char*, argv: char **) optCheckCond ::=
     *     weak_validity: argc > = 0 && current != NULL && argv != NULL 
     *     bound        : argc > 1 && argc > *current > 0
     */
    _Bool (*optCheck)(int argc, int *current, char *argv[], struct optAttr_t *attr);
} optAttr_t;

typedef struct optRegister {
    char *optName;
    optAttr_t attr;
    _Bool inUsed;
} optRegister_t;

typedef enum optionNumber {
    SourceMibFile = 0,
    TargetPdfFile,
    IncludePath,
    NumOfOptions
} optionNumber;

/* Procedures for registerTable implement as macros */
#define regTblNext(iter) ({\
    optRegister_t *__optReg;\
    if (isStringEqual((++iter)->optName, "N/A")) {\
        __optReg = NULL;\
    } else {\
        __optReg = iter;\
    }\
    __optReg;\
})

#define optMatch(O) ({\
    optRegister_t *optReg = registerTable;\
    while (optReg) {\
        if (isStringEqual(optReg->optName, O)) break;\
        optReg = regTblNext(optReg);\
    }\
    optReg;\
})

#define optAttr(O) ({\
    optRegister_t *optReg = optMatch(O);\
    optAttr_t *attr = &optReg->attr;\
    attr;\
})

#define optAttr_withArgs(attr) (attr->withArgs == TRUE)
#define optAttr_type(attr)     (attr->type)
#define optAttr_idx(attr)      (attr->index)

/* Procedures that be registeried
 * in to registerTable */
#include "optRegister_procedures.h"

static optRegister_t registerTable[] = {
    /* Source mib file */
    { "-s", { TRUE, "String", SourceMibFile, sourceMibFile_argCheck }, false }, 
    /* Target pdf file */
    { "-t", { TRUE, "String", TargetPdfFile, targetPdfFile_argCheck }, false }, 
    /* Include path */
    { "-I", { TRUE, "String", IncludePath,   includePath_argCheck   }, false }, 
    /* End */
    { "N/A", { FALSE, "Number", NumOfOptions, NULL }, false}
};

static int optionRelation[];


#endif /* _OPT_REGISTER_H_ */

