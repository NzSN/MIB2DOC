/* optRegister_procedures.h */

#include "type.h"
#include "re.h"

/* optBufferIter(obj) ::= 
 *   Iterator(obi) &&
 *   range_check: obi -> _Bool */
typedef struct optBufferIter {
    int argc;
    int current;
    char **argv;
} optBufferIter;


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
        __opt = null;\
    else\
        __opt = obi->argv[obi->current++];\
    __opt;\
})

static _Bool argConflictCheck(char martix[][NumOfOptions], int optIdx) {
    int i = 0, accumulate = 0;
    if (optIdx < 0) return FALSE; 

    while (i < NumOfOptions) {
        accumulate += martix[optIdx][i++] == REL_CONFLICT_IN;
    }

    return accumulate == 0;
}

static int argInUsedStatusSet(char martix[][NumOfOptions], int optIdx) {
    int i = 0, info, offset = 10;
    if (optIdx < 0) return ERROR; 
    
    while (i < NumOfOptions) {
        info = martix[i][optIdx];
        martix[i++][optIdx] = info * REL_CONFLICT_IN;
    }

    return OK;
}

#define VALID_PATH_PATTERN "^.?/?[a-zA-Z0-9/_\\.]+"

/* Property(argc: int, argv: char *) ::=
 *   argumentSplit: argc * argv
 *     (argc, argv) -- withArgs(argc, argv) */
static optPieces argumentSplit(int argc, char *argv[]) {
    if (argc < 2 || isNullPtr(argv)) 
        return FALSE;
    
    optPieces pieces = optPiecesConst(argc- 1);
    
    char *opt, *optVal; 
    optAttr_t *attr;
    int numOfOptions = 0; 
    
    optBufferIter *iter = optBufferIterConst(argc, argv); 
    
    while (opt = optBufferNext(iter)) {
        attr = optAttr(opt); 
        pieces[numOfOptions][0] = opt;
        
        if (optAttr_withArgs(attr)) {
            optVal = optBufferNext(iter); 
            pieces[numOfOptions][1] = optVal;
        } 

        ++numOfOptions;
    }

    return pieces;
}

/* Property(argc: int, argv: char **) ::=
 *     argumentChecking: argc * current * argv * attr
 *       (argc, argv) -- withArgs(argc, argv) */ 
static _Bool argumentChecking(int argc, char *argv[]) {
    if (argc < 2 || isNullPtr(argv)) 
        return FALSE;
    
    int current = 1; 
    optAttr_t *attr;

    while (current < argc) {
        attr = optAttr(argv[current]);
        if (isNullPtr(attr))
            return FALSE;
        if (attr->check(argc, &current, argv, attr) == FALSE)
            return FALSE;
    }

    return TRUE;
}

/* Property(path: const char *, pattern: re_t) ::=
 *   isPathString: path * pattern 
 *       (path, pattern) -- (pattern == NULL || pattern != NULL) &&
 *                          (pattern != NULL && pattern(str) == TRUE) 
 *                          => (str is belong to subset of pathStr);
 */
static _Bool isPathString(const char *path, re_t pattern) {
    if (isNullPtr(path)) return FALSE; 
    
    re_t pattern_ = pattern; 
    if (isNullPtr(pattern_)) 
        pattern_ = re_compile(VALID_PATH_PATTERN); 

    if (re_matchp(pattern_, path) == ERROR) {
        return FALSE;
    }

    return TRUE;
}

/* Concept(optCheck) */
static _Bool pathArgumentCheck(int argc, int *current, char *argv[], optAttr_t *attr) {
    int ret = TRUE;

    // Precondition: weak_validity
    if (isNullPtr(current) || isNullPtr(argv))
        return FALSE;
    
    char *optVal;  
    optBufferIter *iter = optBufferIterConst(argc, argv);
    iter->current = *current + 1;

    /* Argument checking via regex */ 
    if (optAttr_withArgs(attr)) {
        optVal = optBufferNext(iter); 
        if (isNullPtr(optVal)) {
            ret = FALSE;
            goto EXIT;
        } 
        
        if (!isPathString(optVal, NULL)) {
            ret = FALSE; 
            goto EXIT;
        }
    }
     

    /* Conflict check */
    int index = attr->index;
    argInUsedStatusSet(optionRelation, index);
    if (argConflictCheck(optionRelation, index) == FALSE) {
        ret = FALSE;
    }
    
EXIT:

    *current = iter->current;

    RELEASE_MEM(iter);
    return ret;

}

