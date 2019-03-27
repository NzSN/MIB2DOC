/* optRegister_procedures.h */

#include "type.h"
#include "re.h"

/* optBufferIter(obj) ::= 
 *   Iterator(obi) &&
 *   range_check(obi): obi -> _Bool */
typedef struct optBufferIter {
    int argc;
    int current;
    char **argv;
} optBufferIter;

/* Property(oBi: optBufferIter) */
#define opt_range_check(obi) (obi->current > 0 && obi->current < obi->argc)

/* Property(argc: int, argv: char **) ::=
 *   arg_nums: argc > 1
 *   non_null: argv != NULL */
#define optBufferIterConst(argc, argv) ({\
    optBufferIter *__iter;\
    if (argc < 2 || argv == NULL) __iter = null;\
    else {\
        iter = (optBufferIter *)Malloc(sizeof(optBufferIter));\
        iter->argc = argc;\
        iter->argv = argv;\
        iter->current = 1;\
    }\
    iter;\
})

/* Property(oBi: optBufferIter) */
#define optBufferNext(obi) ({\
    char *__opt;\
    if (!opt_range_check(obi))\
        __opt == null;\
    __opt = obi->argv[obi->current++];\
    __opt;\
})

#define VALID_PATH_PATTERN ""

/* sourceMibFile_argCheck(f) ::= optCheck(f) */
static _Bool sourceMibFile_argCheck(int argc, int *current, char *argv[], optAttr_t *attr) { 
    // Precondition: weak_validity && lowerbound
    if (isNullPtr(current) || isNullPtr(argv))
        return FALSE;
    
    int current_int = *current;
    if (argc < 2 || current_int <= 0 || current_int >= argc)
        return FALSE; 
    
    char *optVal;  
    optBufferIter *iter = optBufferIterConst(argc, argv);
    re_t pattern = re_compile(VALID_PATH_PATTERN); 

    /* Argument checking via regex */ 
    if (optAttr_withArgs(attr)) {
        optVal = optBufferNext(iter); 
        
        if (re_matchp(pattern, optVal) == ERROR)
            return FALSE;
    } 

    /* Conflict check */
     
}

/* targetPdfFile_argCheck(f) ::= optCheck(f) */
static _Bool targetPdfFile_argCheck(int argc, int *current, char *argv[], optAttr_t *attr) {
         
}

/* includePath_argCheck(f) ::= optCheck(f) */
static _Bool includePath_argCheck(int argc, int *current, char *argv[], optAttr_t *attr) {

}

