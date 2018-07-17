//
// Created by ayden on 17-5-1.
//

#ifndef _MIB2DOC_LIST_H_
#define _MIB2DOC_LIST_H_

#include <stdbool.h>

enum elementType {
    SLICE_IDENTIFIER = 6,
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

/* ListNode */
typedef void (*listNodeTask)(listNode *head, void *arg);
listNode *listNodePrev(listNode *node);
listNode *listNodeNext(listNode *node);
listNode *listNodeInsert(listNode *head, listNode *node);
listNode *listNodeDelete(listNode *node);
bool listNodeIsEmpty(listNode *node);
int listNodeTravel(listNode *head, listNodeTask func, void *arg);

/* Slice */
slice * sliceConstruct(int sliKey, char *sliVal);
slice * sliceGet(slice *sli, int sliKey);
int sliceStore(slice *sli, slice *next);
bool sliceRelease(slice *sli);
bool sliceReset(slice *sli);

#endif /* _MIB2DOC_LIST_H_ */
