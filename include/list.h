//
// Created by ayden on 17-5-1.
//

#ifndef _MIB2DOC_LIST_H_
#define _MIB2DOC_LIST_H_

#include <stdbool.h>
#include "type.h"

enum elementType {
    SLICE_TYPE_MINIMUM = 5,
    SLICE_IDENTIFIER,
    SLICE_TYPE,
    SLICE_PERMISSION,
    SLICE_DESCRIPTION,
    SLICE_PARENT,
    SLICE_OID_SUFFIX,
    SLICE_TYPE_MAXIMUM,
    SLICE_BIT_NAME
} sliceType;

typedef struct listNode {
    struct listNode *next;
    struct listNode *prev;
} listNode;

typedef struct listEntry {
    void *element;
    listNode node;
} listEntry;

typedef struct list {
    listEntry *entry;
    void   (*release)(void *);
    void * (*copy)(void *);
    int    (*isEqual)(void *, void *);
    int size;
} list;

typedef struct listIter {
    list *l;
    listEntry *entry;
} listIter;

// Todo: slice should be replaced by list
typedef struct slice {
    int sliKey;
    union {
        char *sliVal;
        void *sliVal_;
        double number;
        int num_h;
        int num_l;
    };
    listNode sliNode;
} slice;

typedef struct sliceIter {
    slice *sl;
    listNode *node;
} sliceIter;

PreCond((Slice), slice(Slice))
#define IS_VALID_SLICE_TYPE(Slice) (Slice > SLICE_TYPE_MINIMUM && Slice < SLICE_TYPE_MAXIMUM)
PreCond((slice), slice(slice))
#define IS_LAST_SLICE_NODE(slice) (slice->sliNode.next == NULL)

/* list */
#define listGetEntry(l) ((l)->entry)
#define listSetEntry(l, e) ((l)->entry = e)
#define listSetReleaseMethod(l, m) ((l)->release = m)
#define listSetCopyMethod(l, m) ((l)->copy = m)
#define listSetEqualMethod(l, m) ((l)->isEqual = m)
#define listGetSize(l) ((l)->size)
#define listSetSize(l, s) ((l)->size = s)

list * listConst(void);
void   listRelease(list *l);
list * listCopy(list *l);
int    listAssignment(list *l, list *r);
int    listIsEqual(list *l, list *r);
int    listConcate(list *l, list *r);
int    listAppend(list *l, void *);
void * listRetriveTail(list *l);
int    listPush(list *l, void *);
void * listPop(list *l);

listIter listGetIter(list *l);
listIter    listPredecessor(listIter li);
listIter    listSuccessor(listIter li);
listEntry * listPrev(listIter *li);
listEntry * listNext(listIter *li);
listEntry * listSource(listIter li);
int         listSink(listIter li_l, listIter li_r);

/* ListNode */
typedef _Bool (*listNodeCmp)(const listNode *node, const void *arg);
typedef _Status (*listNodeTask)(listNode *head, void *arg);
typedef _Bool (*listNodeEqualCheck)(const listNode *node, const listNode *node_);
typedef _Bool (*listNodeEqualCheck_extra)(const listNode *node, const listNode *node_, void *arg);
listNode *listNodePrev(const listNode *node);
listNode *listNodeNext(const listNode *node);
listNode *listNodeInsert(listNode *head, listNode *node);
listNode *listNodeDelete(listNode *node);
listNode *listNodeAppend(listNode *listH, listNode *listN);
listNode * listNodeTail(const listNode *head);
listNode *listNodeInsert(listNode *head, listNode *node);
listNode *listNodeDelete(listNode *node);
listNode *listNodeAppend(listNode *listH, listNode *listN);
listNode * listNodeTail(const listNode *head);
bool listNodeIsEmpty(listNode *node);
_Bool listNodeIsEqual(const listNode *first, const listNode *second,
                      listNodeEqualCheck equalCheck);
_Bool listNodeIsEqual_extra(const listNode *first, const listNode *second,
                            void *arg, listNodeEqualCheck_extra equalCheck);
_Status listNodeTravel(listNode *head, listNodeTask func, void *arg);
listNode * listNodeSearch(listNode *head, listNodeCmp cmpOp, void *arg);

/* Slice Prototype */
slice * sliceConstruct(int sliKey, char *sliVal);
slice * sliceGet(slice *sli, int sliKey);
int sliceAssignment(slice *sli_l, slice *sli_r);
int sliceStore(slice *sli, slice *next);
int sliceStore_without_key(slice *sliHead, slice *newSli);
bool sliceRelease(slice *sli);
bool sliceRelease_STATIC(slice *sli);
bool sliceReset(slice *sli);
bool sliceReset_STATIC(slice *sli);
char * sliceGetVal(slice *sliceHead, int sliKey);
char * sliceRetriVal(slice *sliHead, int sliKey);
sliceIter sliceGetIter(slice *sl);
sliceIter sliceSuccessor(sliceIter si);
sliceIter slicePredecessor(sliceIter si);
slice   * slicePrev(sliceIter *si);
slice   * sliceNext(sliceIter *si);
slice   * sliceSource(sliceIter si);
int       sliceSink(sliceIter si_l, sliceIter si_r);

#ifdef MIB2DOC_UNIT_TESTING

void list__Generic_List(void **state);

#endif /* MIB2DOC_UNIT_TESTING */

#endif /* _MIB2DOC_LIST_H_ */

