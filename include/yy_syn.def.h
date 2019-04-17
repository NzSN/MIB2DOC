/* yy_syn.def.h */

#ifndef __YY_SYN_DEF_H__
#define __YY_SYN_DEF_H__

#include "type.h"
#include "list.h"
#include "iterators.h"

/* SEQUENCE */
typedef struct sequence_item {
    char *ident;
    char *type;    
    listNode node;
} sequence_item;

typedef struct sequence {
    int length;
    char *identifier;
    sequence_item items; 
} sequence;

// sequence_item's procedures
extern sequence_item * seqItemConst();
extern int seqItemDestruct(sequence_item *seqItem);
extern int seqItemsDestruct(sequence_item *seqItem);
extern sequence_item * seqItemCopy(const sequence_item *seqItem);
extern sequence_item * seqItemsCopy(const sequence_item *seqitem);
extern int seqItemAssignment(sequence_item *lSeqItem, const sequence_item *rSeqItem);
extern int seqItemIsEqual(const sequence_item *lSeqItem, const sequence_item *rSeqItem);
extern sequence_item * seqItemPrev(const sequence_item *seqItem);
extern sequence_item * seqItemNext(const sequence_item *seqItem);
extern sequence_item * seqItemTail(const sequence_item *seqitem);
extern int seqItemDelete(sequence_item *item);
extern int seqItemInsert(sequence_item *leftItem, sequence_item *rightItem);
extern int seqItemAppend(sequence_item *seqHead, sequence_item *newItem);
extern sequence_item * seqItemSearch(const sequence_item *seqHead, const char *ident);

// sequence's procedures
extern sequence * seqConst();
extern int seqDestructor(sequence *seq);
extern sequence *seqCopy(const sequence *seq);
extern int seqAssignment(sequence *seq_lval, const sequence *seq_rval);
extern _Bool seqisEqual(sequence *leftSeq, sequence *rightSeq);
extern int seqAddItem(sequence *seq, sequence_item *item);
extern int seqDelItem(sequence *seq, const char *ident);
extern sequence_item * seqSearch(sequence *seq, const char *ident);

#ifdef MIB2DOC_UNIT_TESTING

void yy_syn_def__SEQUENCE(void **state);

#endif

/* OBJECT-TYPE */
typedef enum {
    SYNTAX_TYPE_BIT_NAME,
    // Type except BITNAME
    SYNTAX_TYPE_STRING_VAL
} SYNTAX_TYPE;

typedef enum {
    ACCESS_TYPE_RO,
    ACCESS_TYPE_RW,
    ACCESS_TYPE_RC,
    ACCESS_TYPE_NOT_ACCESS,
    ACCESS_TYPE_NOTIFY
} ACCESS_TYPE;

typedef enum {
    STATUS_TYPE_CURRENT,
    STATUS_TYPE_DEPRECATED,
    STATUS_TYPE_OBSOLETE
} STATUS_TYPE;

typedef enum {
    DEFVAL_TYPE_IDENT,
    DEFVAL_TYPE_NUM,
    DEFVAL_TYPE_HEX_STR,
    DEFVAL_TYPE_STR,
    DEFVAL_TYPE_BITS
} DEFVAL_TYPE;

typedef struct SYNTAX_t {
    SYNTAX_TYPE type;
    char *val;
    slice *vals;
} SYNTAX_t;
typedef sliceIter SYNTAX_iter;

typedef struct UNITS_t {
    char *unitInfo;
} UNITS_t;

typedef struct ACCESS_t {
    ACCESS_TYPE type;
    char *access;
} ACCESS_t;

typedef struct STATUS_t {
    STATUS_TYPE type;
    char *status;
} STATUS_t;

typedef struct DESC_t {
    char *description;
} DESC_t;

typedef struct REF_t {
    char *refInfo;
} REF_t;

typedef struct INDEX_t {
    unsigned int numOfIdx;
    char *idx;
    slice *idxs;
} INDEX_t;
typedef sliceIter INDEX_iter;

typedef struct DEFVAL_t {
    DEFVAL_TYPE type;
    char *val;
    slice *vals;
} DEFVAL_t;
typedef sliceIter DEFVAL_iter;

typedef struct MOUNT_t {
    char *parent;
    char *suffix;
} MOUNT_t;

// Temporary ignore message from this field.
typedef struct OBJECT_t {} OBJECT_t;

typedef struct OBJECT_type_t {
    SYNTAX_t *syntax;
    ACCESS_t *access;
    STATUS_t *status;
    DESC_t   *desc;
    REF_t    *ref;
    INDEX_t  *index;
    MOUNT_t  *mount;
} OBJECT_TYPE_t;

/* Procedure implement as macro */
#define syntaxType(S) ((S)->type)
#define syntaxSetType(S, TYPE) ((S)->type = TYPE)
#define syntaxVal(S, I) ({\
    char *theVal = NULL;\
    if ((S)->type == SYNTAX_TYPE_BIT_NAME) {\
        theVal = source(I)->sliVal;\
        I = successor(I);\
    } else {\
        theVal = (S)->val;\
    }\
    theVal;\
})
#define syntaxSetVal(S, V) ((S)->val = V)
#define syntaxAddVal(S, V) ({\
    int ret = OK;\
    slice *sl = sliceConstruct(0, (void *)V);\
    if (isNullPtr(sl)) ret = ERROR;\
    else {                       \
        if (S->vals)\
            ret = sliceStore_without_key(S->vals, sl);\
        else\
            S->vals = sl;\
    }\
    ret;\
})

#define unitInfo(U) ((U)->unitInfo)
#define unitSetInfo(U, I) ((U)->unitInfo = I)

#define accessType(A) ((A)->type)
#define accessSetType(A, T) ((A)->type = T)
#define accessVal(A) ((A)->access)
#define accessSetVal(A, V) ((A)->access = V)

#define statusType(S) ((S)->type)
#define statusSetType(S, T) ((S)->type = T)
#define statusVal(S) ((S)->status)
#define statusSetVal(S, T) ((S)->status = T)

#define refGetInfo(R) ((R)->refInfo)
#define refSetInfo(R, I) ((R)->refInfo = I)

#define defvalType(D) ((D)->type)
#define defvalSetType(D, T) ((D)->type = T)
#define defvalGetVal(D, I) ({\
    char *theVal = NULL;\
    if ((D)->type = DEFVAL_TYPE_BITS) {\
        theVal = sucessor(I);\
    } else {\
        theVal = (D)->val;\
    }\
    theVal;\
})
#define defValSetVal(D, V) ((D)->val = V)
#define defValAddVal(D, V) ({\
    int ret = OK;\
    slice *sl = sliceConstruct(0, (void *)V);\
    if (isNullPtr(sl)) ret = ERROR;\
    else {                                    \
        if (D->vals)\
            ret = sliceStore_without_key(D, sl);    \
        else\
            D->valse = sl;\
    }\
    ret;\
})

#define indexNumOf(I) ((I)->numOfIdx)
#define indexSetNumOf(I, N) ((I)->numOfIdx = N)
#define indexGetVal(I, ITER) ({\
    char *theVal = NULL;\
    if (indexNumOf(I) > 1) {\
        theVal = successor(ITER);\
    } else {\
        theVal = (I)->idx;\
    }\
    theVal;\
})
#define indexSetVal(I, V) ((I)->idx = V)
#define indexAddVal(I, V) ({\
    int ret = OK;\
    slice *sl = sliceConstruct(0, (void *)V);\
    if (isNullPtr(sl)) ret = ERROR;\
    else {                       \
        if (I->idxs)\
            ret = sliceStore_without_key(I->idxs, sl);    \
        else                                  \
            I->idxs = sl; \
    }\ 
    ret;\
})

#define mountParent(M) ((M)->parent)
#define mountSetParent(M, P) ((M)->parent = P)
#define mountSuffix(M) ((M)->suffix)
#define mountSetSuffix(M, S) ((M)->suffix = S)

/* Prototype */
OBJECT_TYPE_t * objTypeConst(void);
SYNTAX_t *  objTypeSyntax(OBJECT_TYPE_t *obj);
int         objTypeSetSyntax(OBJECT_TYPE_t *obj, SYNTAX_t *syn);
ACCESS_t *  objTypeAccess(OBJECT_TYPE_t *obj);
int         objTypeSetAccess(OBJECT_TYPE_t *obj, ACCESS_t *access);
STATUS_t *  objTypeStatus(OBJECT_TYPE_t *obj);
int         objTypeSetStatus(OBJECT_TYPE_t *obj, STATUS_t *status);
DESC_t   *  objTypeDesc(OBJECT_TYPE_t *obj);
int         objTypeSetDesc(OBJECT_TYPE_t *obj, DESC_t *desc);
REF_t    *  objTypeRef(OBJECT_TYPE_t *obj);
int         objTypeSetRef(OBJECT_TYPE_t *obj, REF_t *ref);
INDEX_t  *  objTypeIndex(OBJECT_TYPE_t *obj);
int         objTypeSetIndex(OBJECT_TYPE_t *obj, INDEX_t *index);
MOUNT_t  *  objTypeMount(OBJECT_TYPE_t *obj);
int         objTypeSetMount(OBJECT_TYPE_t *obj, MOUNT_t *mount);

SYNTAX_t * syntaxConst(void);
SYNTAX_t * syntaxInit(SYNTAX_t *syn);
SYNTAX_iter syntaxGetIter(SYNTAX_t *syn);
SYNTAX_iter syntaxSuccessor(SYNTAX_iter i);
SYNTAX_iter syntaxPredecessor(SYNTAX_iter i);
slice * syntaxNext(SYNTAX_iter *i);
slice * syntaxPrev(SYNTAX_iter *i);

INDEX_t * indexConst(void);
INDEX_t * indexInit(INDEX_t *idx);
INDEX_iter indexGetIter(INDEX_t *idx);
INDEX_iter indexSuccessor(INDEX_iter i);
INDEX_iter indexPredecessor(INDEX_iter i);
slice      * indexNext(INDEX_iter *i);
slice      * indexPrev(INDEX_iter *i);

#ifdef MIB2DOC_UNIT_TESTING

void yy_syn_def_Basic_Field(void **state);

#endif 

/* IMPORT */
typedef struct moduleItem {
    slice items;
} moduleItem;

#endif /* YY_syn.def.h */

