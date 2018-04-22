//
// Created by ayden on 2017/4/19.
//

#ifndef _MIB2DOC_TYPE_H_
#define _MIB2DOC_TYPE_H_

#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include "mibTreeObjTree.h"

/* define */
#define cSpace ' '
#define null (0)
#define SIZE_OF_CURRENT_TABLE 64
#define SIZE_OF_SECTION_RECORD 1024
#define SIZE_OF_OID_STRING 256
#define isNullPtr(PTR) (PTR ? 0:1)
#define RELEASE_MEM(PTR) ({free(PTR); PTR=NULL;})
#define containerOf(ptr, ConType, member) ({\
    if (ptr != NULL) {\
        const typeof( ((ConType *)(0))->member) *__mptr = ptr;\
        __RET_PTR__ = (ConType *)((char *)__mptr - offsetof(ConType, member));\
    }\
})


/* Token Id Definition */
#ifndef YYTOKENTYPE
#define YYTOKENTYPE
typedef enum yytokentype {
    IDENTIFIER = 258,
    OBJ_SPECIFIER = 259,
    SYNTAX_SPECIFIER = 260,
    ACCESS_SPECIFIER = 261,
    ACCESS_VALUE = 262,
    STATUS_SPECIFIER = 263,
    STATUS_VALUE = 264,
    DESC_SPECIFIER = 265,
    DESC_VALUE = 266,
    MOUNT_POINT = 267,
    ASSIGNED = 268,
    BEGIN_ = 269,
    END_ = 270,
    DEF = 271,
    SEQ = 272,
    COMMA = 273,
    SEMICOLON = 274,
    INDEX_ = 275,
    TRAP_SPECIFIER = 276,
    OBJ_IDEN_ = 277,
    L_BRACE = 278,
    R_BRACE = 279,
    OBJECTS_ = 280,
    TYPE = 281,
    NUM = 282,
    FROM_ = 283,
    IMPORTS_ = 284
} tokenType;
#endif /* YYTOKENTYPE */

typedef enum unitType {
    OBJECT = 1,
    TRAP,
    OBJECT_IDENTIFIER,
    SEQUENCE,
    SMI_DEF
} unitType;

enum elementType {
    SLICE_IDENTIFIER = 6,
    SLICE_TYPE,
    SLICE_PERMISSION,
    SLICE_DESCRIPTION,
    SLICE_PARENT,
    SLICE_OID_SUFFIX,
    SLICE_TYPE_MAXIMUM
} sliceType;

typedef enum errorType {
    ERROR_NONE,
    ERROR_GENERIC,
    ERROR_NULL_REF,
    ERROR_WRONG_IDX,
    ERROR_OPEN_FAILED,
    ABORT
} errorType;

extern errorType mib2docError;

typedef struct dispatchParam {
    void *param;
    listNode node;
} dispatchParam;

typedef struct nodeMeta_t {
    char *nodeIdent;
    char *parentIdent;
    char *suffix;
} nodeMeta_t;

typedef struct leaveMeta_t {
    char *ident;
    char *parent;
    char *suffix;
    char *type;
    char *permission;
} leaveMeta_t;

#define MAX_CHAR_OF_MOD_IDENT 64

typedef enum symErrorCode {
    SYM_TABLE_EXISTS,
    SYM_TABLE_NOT_FOUND,
    SYM_EXISTS,
    SYM_NOT_FOUND
} symErrorCode;

extern symErrorCode symTblError;

typedef enum symbolType {
    SYMBOL_TYPE_NODE,
    SYMBOL_TYPE_LEAVE
} symbolType;

typedef struct symbol_t {
    symbolType symType;
    char *symIdent;
    union {
       leaveMeta_t leaveMeta;
       nodeMeta_t nodeMeta;
   } symInfo;
    listNode symNode;
} symbol_t;

typedef struct symbolTable {
    char *modName;
    symbol_t *symbol;
    listNode symTblNode;
} symbolTable;

#endif /* _MIB2DOC_TYPE_H_ */
