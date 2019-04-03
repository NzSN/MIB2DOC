//
// Created by ayden on 2017/4/19.
//

#ifndef _MIB2DOC_TYPE_H_
#define _MIB2DOC_TYPE_H_

#include <stddef.h>
#include <stdbool.h>

#include <assert.h>

/* define */
#ifndef FALSE
#define FALSE false 
#endif 

#ifndef TRUE
#define TRUE true
#endif

#ifndef NOT
#define NOT !
#endif 

#ifndef OK
#define OK 0
#endif

#ifndef ERROR
#define ERROR (-1)
#endif

#define cSpace ' '
#define null (0)

#define SIZE_OF_CURRENT_TABLE 64
#define SIZE_OF_SECTION_RECORD 1024
#define SIZE_OF_OID_STRING 256
#define EXTRA_OF_OID_LEN 10

#define isNullPtr(PTR) (PTR == NULL)
#define isNonNullPtr(PTR) (PTR != NULL)

#define RELEASE_MEM(PTR) ({ free(PTR); PTR=NULL; })
#define RELEASE_IF_NON_NULL(PTR) ({ if (PTR) RELEASE_MEM(PTR); })

#define errorMsg(msg, args...) fprintf(stderr, msg, ##args)
#define abortWithMsg(msg, args...) \
do {\
    errorMsg(msg, ##args);\
    exit(1);\
} while (0)

#define containerOf(ptr, ConType, member) ({\
        const typeof( ((ConType *)(0))->member) *__mptr = ptr;\
        (ConType *)((char *)__mptr - offsetof(ConType, member));\
})

typedef int _Status;
typedef void * anything;

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

