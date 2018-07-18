//
// Created by ayden on 2017/4/19.
//

#ifndef _MIB2DOC_TYPE_H_
#define _MIB2DOC_TYPE_H_

#include <stddef.h>
#include <stdbool.h>
#include "mibTreeObjTree.h"
#include "lexer.h"
#include "yy_syn.tab.h"

/* define */
#ifndef FALSE
#define FALSE false 
#endif /* FALSE */
#ifndef TRUE
#define TRUE true
#endif /* TRUE */

#define cSpace ' '
#define null (0)
#define SIZE_OF_CURRENT_TABLE 64
#define SIZE_OF_SECTION_RECORD 1024
#define SIZE_OF_OID_STRING 256
#define isNullPtr(PTR) (PTR ? false : true)
#define RELEASE_MEM(PTR) ({free(PTR); PTR=NULL;})
#define containerOf(ptr, ConType, member) ({\
        const typeof( ((ConType *)(0))->member) *__mptr = ptr;\
        (ConType *)((char *)__mptr - offsetof(ConType, member));\
})

typedef enum unitType {
    OBJECT = 1,
    TRAP,
    OBJECT_IDENTIFIER,
    SEQUENCE,
    SMI_DEF
} unitType;

typedef enum errorType {
    ERROR_NONE,
    ERROR_GENERIC,
    ERROR_NULL_REF,
    ERROR_WRONG_IDX,
    ERROR_OPEN_FAILED,
    ABORT
} errorType;

extern int mib2docError;

#endif /* _MIB2DOC_TYPE_H_ */
