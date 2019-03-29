/* optRegister.h */

#ifndef _OPT_REGISTER_H_
#define _OPT_REGISTER_H_

#include "util.h"

/* Callbacks of optCheckCallback type 
 * optCheck(f) ::=
 *   Arity(f) = 4 &&
 *   check(f) &&
 *   optValidity(f) &&
 *   optConflictCheck(f) */
struct optAttr_t;
typedef _Bool (*optCheck_t)(int argc, int *current, char *argv[], struct optAttr_t *attr);

typedef struct optAttr_t {
    int withArgs; 
    char *type; // String or Number
    int index;
    optCheck_t check;
} optAttr_t;

typedef struct optRegister {
    char *optName;
    optAttr_t attr;
} optRegister_t;

typedef enum optionNumber {
    OPT_NONE = -1,
    SourceMibFile = 0,
    TargetPdfFile,
    IncludePath,
    NumOfOptions
} optionNumber;


static optRegister_t registerTable[];

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

/* Enumerate use in relation matrix */
enum {
    REL_INDEPENDENT = 0,
    REL_CONFLICT    = 1,
    REL_CONFLICT_IN = 2
};

static char optionRelation[NumOfOptions][NumOfOptions] = {
    { 0, 0, 0 },
    { 0, 0, 0 },
    { 0, 0, 0 }
};


typedef char         * optHalfPiece;
typedef optHalfPiece * optPiece;
/* optPieces(v) ::=
 *   Array(v) &&
 *   Dimension(v) = 1 &&
 *   optPieces always terminated by null optPiece.
 */
typedef optPiece     * optPieces;

typedef struct optPiecesIter {
    int current_pieces;
    int position_in_piece;
    optPieces oPieces;   
} optPiecesIter;

#define optPiecesConst(NUM_OF_OPTS) ({\
    int i = 0, NUM = (NUM_OF_OPTS) + 1;\
    optPieces pieces = (optPieces)Malloc((NUM) * sizeof(optPieces));\
    while (i < (NUM)) {\
        pieces[i] = (optPiece)Malloc(2 * sizeof(optPiece));\
        memset(pieces[i++], 0, 2 * sizeof(optPiece));\
    }\
    pieces[NUM-1][0] = "T";\
    pieces;\
})
#define optPiecesGetIter(OPieces) ({\
    optPiecesIter *iter = (optPiecesIter *)Malloc(sizeof(optPiecesIter));\
    memset(iter, 0, sizeof(optPiecesIter));\
    iter->oPieces = OPieces;\
})

#define optPiecesNextHelper(oPIter) ({\
    char *optVal;\
    int current_pieces = oPIter->current_pieces,\
        position_in_piece = oPIter->position_in_piece;\
    if (position_in_piece < 2) {\
        optVal = oPIter->oPieces[current_pieces][position_in_piece++];\
    } else {\
        position_in_piece = 0;\
        optVal = oPiter->oPieces[++current_pieces][]\
    }\
    /* Update iterator */ \
    oPIter->current_pieces = current_pieces;\
    oPIter->position_in_piece = position_in_piece;\
    \
    optVal;\
})

#define optPiecesNext(oPIter) ({\
    char *optVal;\
    if (isNullPtr(oPIter)) optVal = NULL;\
    else {\
        while (optVal == NULL)  {\
            optVal = optPiecesNextHelper(oPIter);\
            if (isStringEqual(optVal, "T")) {\
                optVal = NULL;\
                --oPIter->position_in_piece;\
            }\
        }\
    }\
    optVal;\
})

/* Procedures that be registeried
 * in to registerTable */
#include "optRegister_procedures.h"

static optRegister_t registerTable[] = {
    /* Source mib file */
    { "-s", { TRUE, "String", SourceMibFile, pathArgumentCheck } }, 
    /* Target pdf file */
    { "-t", { TRUE, "String", TargetPdfFile, pathArgumentCheck } }, 
    /* Include path */
    { "-I", { TRUE, "String", IncludePath,   pathArgumentCheck } }, 
    /* End */
    { "N/A", { FALSE, "Number", NumOfOptions, NULL } }
};



#endif /* _OPT_REGISTER_H_ */

