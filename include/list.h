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
    SLICE_TYPE_MAXIMUM
} sliceType;

typedef struct listNode {
    struct listNode *next;
    struct listNode *prev;
} listNode;

typedef struct slice {
    int sliKey;
    char *sliVal;
    listNode sliNode;
} slice;


typedef slice sliceIter;

PreCond((Slice), slice(Slice))
#define IS_VALID_SLICE_TYPE(Slice) (Slice > SLICE_TYPE_MINIMUM && Slice < SLICE_TYPE_MAXIMUM)

/* ListNode */
typedef _Bool (*listNodeCmp)(const listNode *node, const void *arg);
typedef _Status (*listNodeTask)(listNode *head, void *arg);
typedef _Bool (*listNodeEqualCheck)(const listNode *node, const listNode *node_);
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
_Status listNodeTravel(listNode *head, listNodeTask func, void *arg);
listNode * listNodeSearch(listNode *head, listNodeCmp cmpOp, void *arg);

/* Slice Prototype */
slice * sliceGetPrev(sliceIter *sli);
slice * sliceGetNext(sliceIter *sli);
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
sliceIter * sliceGetIter(slice *sl, sliceIter *si);
sliceIter * sliceSuccessor(sliceIter *si);
sliceIter * slicePredecessor(sliceIter *si);
slice     * slicePrev(sliceIter *si);
slice     * sliceNext(sliceIter *si);

/* Procedures implement as macro */
/* Provide readability to slice iterators */
PreCond((si), slice(si))
#define sliceSource(si) ((slice *)si)

/* Provide writability to slice iterators */
PreCond((si), slice(si))
#define sliceSink(si_l, si_r) ({\
    slice *sl_l = sliceSource(si_l);\
    slice *sl_r = sliceSource(si_r);\
    sliceAssignment(sl_l, sl_r);    \
    OK;\
})


#endif /* _MIB2DOC_LIST_H_ */

