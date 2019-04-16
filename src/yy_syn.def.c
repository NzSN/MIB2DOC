/* yy_syn.def.c */

#include <malloc.h>
#include <string.h>
#include "type.h"
#include "yy_syn.def.h"
#include "util.h"

/* sequence && sequence_item */
#define SEQUENCE_ITEM_NODE_R(SI) (&SI->node)

/* For the purposes of regular the following procedure will be defined:
 * . Default Constructor
 * . Destructor
 * . copy constructor
 * . Assignment
 * . equality
 */

// Default Constructor
sequence_item * seqItemConst() {
    sequence_item *seqItem;

    seqItem = (sequence_item *)malloc(sizeof(sequence_item));
    memset(seqItem, 0, sizeof(sequence_item));

    return seqItem;
}

// Destructor
int seqItemDestruct(sequence_item *seqItem) {

    if (isNullPtr(seqItem))
        return ERROR;

    if (seqItem->ident) RELEASE_MEM(seqItem->ident);
    if (seqItem->type) RELEASE_MEM(seqItem->type);

    RELEASE_MEM(seqItem);

    return OK;
}

int seqItemsDestruct(sequence_item *seqItem) {
    if (isNullPtr(seqItem))
        return ERROR;

    sequence_item *seqNext = seqItemNext(seqItem);
    for (; seqItem != NULL; seqItem = seqNext) {
        seqItemDestruct(seqItem);

        seqItem = seqNext;
        seqNext = seqItemNext(seqNext);
    }
    return OK;
}

// Copy constructor
sequence_item *seqItemCopy(const sequence_item *seqItem) {
    sequence_item *seqCopy;

    if (isNullPtr(seqItem))
        return NULL;

    seqCopy = seqItemConst();
    seqCopy->ident = strdup(seqItem->ident);
    seqCopy->type = strdup(seqItem->type);

    return seqCopy;
}

sequence_item * seqItemsCopy(const sequence_item *seqItem) {
    if (isNullPtr(seqItem))
        return NULL;

    sequence_item head, *copy;

    memset(&head, 0, sizeof(sequence_item));

    for (; seqItem != NULL; seqItem = seqItemNext(seqItem)) {
        copy = seqItemCopy(seqItem);
        seqItemAppend(&head, copy);
    }

    copy = seqItemNext(&head);
    copy->node.prev = NULL;

    return copy;
}

// Assignment
int seqItemAssignment(sequence_item *lSeqItem, const sequence_item *rSeqItem) {

    if (isNullPtr(lSeqItem) || isNullPtr(rSeqItem))
        return ERROR;

    if (lSeqItem->ident) RELEASE_MEM(lSeqItem->ident);
    if (lSeqItem->type)  RELEASE_MEM(lSeqItem->type);

    lSeqItem->ident = strdup(rSeqItem->ident);
    lSeqItem->type = strdup(rSeqItem->type);

    return OK;
}

// Equality
#define isAttrEqual(lAttr, rAttr)\
    strlen(lAttr) == strlen(rAttr) && \
    !strncmp(lAttr, rAttr, strlen(lAttr))

int seqItemIsEqual(const sequence_item *lSeqItem, const sequence_item *rSeqItem) {
    int isIdentEqual, isTypeEqual;
    char *lSeqAttr, *rSeqAttr;

    if (isNullPtr(lSeqItem) || isNullPtr(rSeqItem))
        return FALSE;

    lSeqAttr = lSeqItem->ident;
    rSeqAttr = rSeqItem->ident;
    if (isNonNullPtr(lSeqAttr) && isNonNullPtr(rSeqAttr))
        isIdentEqual = isAttrEqual(lSeqAttr, rSeqAttr);

    lSeqAttr = lSeqItem->type;
    rSeqAttr = rSeqItem->type;
    if (isNonNullPtr(lSeqAttr) && isNonNullPtr(rSeqAttr))
        isTypeEqual = isAttrEqual(lSeqAttr, rSeqAttr);

    return isIdentEqual && isTypeEqual;
}

sequence_item * seqItemPrev(const sequence_item *seqItem) {
    if (isNullPtr(seqItem) || isNullPtr(seqItem->node.prev))
        return NULL;

    return containerOf(listNodePrev(&seqItem->node), sequence_item, node);
}

sequence_item * seqItemNext(const sequence_item *seqItem) {
    if (isNullPtr(seqItem) || isNullPtr(seqItem->node.next))
        return NULL;
    return containerOf(listNodeNext(&seqItem->node), sequence_item, node);
}

sequence_item * seqItemTail(const sequence_item *seqItem) {
    if (isNullPtr(seqItem))
        return NULL;

    listNode *lastNode = listNodeTail(SEQUENCE_ITEM_NODE_R(seqItem));
    return containerOf(lastNode, sequence_item, node);
}

int seqItemDelete(sequence_item *item) {
    if (isNullPtr(item))
        return ERROR;

    if (listNodeDelete(SEQUENCE_ITEM_NODE_R(item)) == NULL)
        return ERROR;

    return OK;
}

int seqItemInsert(sequence_item *leftItem, sequence_item *rightItem) {
    if (isNullPtr(leftItem) || isNullPtr(rightItem))
        return ERROR;

    listNodeInsert(SEQUENCE_ITEM_NODE_R(leftItem), SEQUENCE_ITEM_NODE_R(rightItem));
    return OK;
}

int seqItemAppend(sequence_item *seqHead, sequence_item *newItem) {
    if (isNullPtr(seqHead) || isNullPtr(newItem))
        return ERROR;

    sequence_item *seqTail = seqItemTail(seqHead);
    return seqItemInsert(seqTail, newItem);
}

sequence_item * seqItemSearch(const sequence_item *seqHead, const char *ident) {
    if (isNullPtr(seqHead) || isNullPtr(ident))
        return NULL;

    const sequence_item *currentItem = seqHead;
    for (; currentItem != NULL; currentItem = seqItemNext(currentItem)) {
        if (isNullPtr(currentItem->ident)) continue;
        if (isAttrEqual(currentItem->ident, ident)) break;
    }
    return (sequence_item *)currentItem;
}

// Sequence

// Default constructor
sequence * seqConst() {
    sequence *newSeq = (sequence *)malloc(sizeof(sequence));
    memset(newSeq, 0, sizeof(sequence));

    return newSeq;
}

// Destructor
int  seqDestructor(sequence *seq) {
    if (isNullPtr(seq))
        return ERROR;

    sequence_item *firstItem = seqItemNext(&seq->items);
    if (isNonNullPtr(firstItem)) seqItemDestruct(firstItem);

    RELEASE_IF_NON_NULL(seq->identifier);

    RELEASE_MEM(seq);

    return OK;
}

// Copy constructor
sequence * seqCopy(const sequence *seq) {
    if (isNullPtr(seq))
        return NULL;

    sequence *seq_copy = seqConst();

    seq_copy->identifier = strdup(seq->identifier);

    sequence_item *firstItem = seqItemNext(&seq->items);
    sequence_item *seqItem_copy = seqItemsCopy(firstItem);

    if (seqItemAppend(&seq_copy->items, seqItem_copy) == ERROR)
        return NULL;

    seq_copy->length = seq->length;

    return seq_copy;
}

// Assignment
int seqAssignment(sequence *seq_lval, const sequence *seq_rval) {
    if (isNullPtr(seq_lval) || isNullPtr(seq_lval))
        return ERROR;

    RELEASE_IF_NON_NULL(seq_lval->identifier);
    seqItemDestruct(seqItemNext(&seq_lval->items));

    seq_lval->identifier = strdup(seq_rval->identifier);

    sequence_item *firstItem = seqItemNext(&seq_rval->items);
    sequence_item *seqItem_copy = seqItemsCopy(firstItem);

    if (seqItemAppend(&seq_lval->items, seqItem_copy) == ERROR)
        return ERROR;

    seq_lval->length = seq_rval->length;

    return OK;
}

// Equality
_Bool seqIsEqual(sequence *leftSeq, sequence *rightSeq) {
    if (isNullPtr(leftSeq) || isNullPtr(rightSeq))
        return FALSE;

    if (!isStringEqual(leftSeq->identifier, rightSeq->identifier))
        return FALSE;

    if (leftSeq->length != rightSeq->length)
        return FALSE;

    int idx = 0, length = leftSeq->length;
    sequence_item *current_left = seqItemNext(&leftSeq->items);
    sequence_item *current_right = seqItemNext(&rightSeq->items);

    while (idx++ < length) {
        if (seqItemIsEqual(current_left, current_right) == FALSE)
            return FALSE;
        current_left = seqItemNext(current_left);
        current_right = seqItemNext(current_right);
    }

    return TRUE;
}

// Add
int seqAddItem(sequence *seq, sequence_item *item) {
    if (isNullPtr(seq) || isNullPtr(item))
        return ERROR;

    seqItemAppend(&seq->items, item);
    seq->length++;

    return OK;
}

// Delete
int seqDelItem(sequence *seq, const char *ident) {
    if (isNullPtr(seq) || isNullPtr(ident))
        return ERROR;

    sequence_item *selected = seqItemSearch(&seq->items, ident);
    if (seqItemDelete(selected) == ERROR)
        return ERROR;
    if (seqItemDestruct(selected) == ERROR)
        return ERROR;

    return OK;
}

// Search
sequence_item * seqSearch(sequence *seq, const char *ident) {
    if (isNullPtr(seq) || isNullPtr(ident))
        return NULL;

    sequence_item *selected = seqItemSearch(&seq->items, ident);
    if (isNullPtr(selected)) return NULL;

    return selected;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"
#include "util.h"

void yy_syn_def__SEQUENCE_ITEM(void) {
    /* Sequence item test */
    sequence_item *new, *item = seqItemConst();

    // Constructor
    // Destructor
    // Append
    int i = 0;
    while (i++ < 100) {
        new = seqItemConst();
        new->ident = randStr(10);
        new->type = randStr(10);

        assert_int_equal(seqItemAppend(item, new), OK);
    }

    seqItemsDestruct(item);

    // Copy constructor
    item = seqItemConst();
    item->ident = strdup("Try");
    item->type = strdup("Type");

    sequence_item *copy = seqItemCopy(item);
    assert_string_equal(item->ident, copy->ident);
    assert_string_equal(item->type, copy->type);

    seqItemDestruct(copy);

    // Search
    i = 1;
    sequence_item *head = seqItemConst();
    head->ident = strdup("0");
    head->type = strdup("0");
    while (i < 100) {
        new = seqItemConst();
        new->ident = numberToStr(i);
        new->type = numberToStr(i);

        seqItemAppend(head, new);
        ++i;
    }

    i = 0;
    char *ident;

    while (i < 100) {
        ident = numberToStr(i);
        new = seqItemSearch(head, ident);
        assert_non_null(new);
        assert_string_equal(new->ident, ident);

        RELEASE_MEM(ident);
        ++i;
    }

    // seqItemsCopy
    copy = seqItemsCopy(head);
    i = 0;
    while (i < 100) {
        ident = numberToStr(i);
        new = seqItemSearch(copy, ident);
        assert_non_null(new);
        assert_string_equal(new->ident, ident);

        RELEASE_MEM(ident);
        ++i;
    }

    seqItemsDestruct(copy);
    seqItemsDestruct(head);

    // Assignment
    new = seqItemConst();
    seqItemAssignment(new, item);

    assert_string_equal(new->ident, item->ident);
    assert_string_equal(new->type, item->type);

    // Equlity
    assert_int_equal(seqItemIsEqual(item, new), TRUE);

    RELEASE_MEM(new->ident);
    new->ident = strdup("TryTry");
    assert_int_equal(seqItemIsEqual(item, new), FALSE);
}

void yy_syn_def__SEQUENCE_(void) {
    /* Sequence test */

    // Default constructor
    sequence *seq = seqConst();
    sequence_item *item;

    seq->identifier = strdup("Seq");

    // Copy constructor
    item = seqItemConst();
    item->ident = strdup("Try");
    item->type = strdup("Try");
    seqAddItem(seq, item);

    sequence_item *found;
    found = seqSearch(seq, "Try");
    assert_non_null(found);
    assert_string_equal(found->ident, "Try");

    sequence *copy = seqCopy(seq);
    found = seqSearch(copy, "Try");
    assert_non_null(found);
    assert_string_equal(found->ident, "Try");
    assert_string_equal(found->type, "Try");

    // Assignment
    sequence *seqAssign = seqConst();
    seqAssignment(seqAssign, seq);

    found = seqSearch(seqAssign, "Try");
    assert_non_null(found);
    assert_string_equal(found->ident, "Try");

    // Equality
    int isEqual = seqIsEqual(copy, seqAssign);
    assert_int_equal(isEqual, TRUE);

    // Destructor
    seqDestructor(seq);
}

void yy_syn_def__SEQUENCE(void **state) {
    yy_syn_def__SEQUENCE_ITEM();
    yy_syn_def__SEQUENCE_();
}

#endif /* MIB2DOC_UNIT_TESTING */


/* OBJECT-TYPE */
OBJECT_TYPE_t * objTypeConst(void) {
    OBJECT_TYPE_t *objType = (OBJECT_TYPE_t *)Malloc(sizeof(OBJECT_TYPE_t));
    return objType;
}

SYNTAX_t * objTypeSyntax(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->syntax;
}

int objTypeSetSyntax(OBJECT_TYPE_t *obj, SYNTAX_t *syn) {
    if (isNullPtr(obj)) return ERROR;
    obj->syntax = syn;
    return OK;
}

ACCESS_t * objTypeAccess(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->access;
}

int objTypeSetAccess(OBJECT_TYPE_t *obj, ACCESS_t *access) {
    if (isNullPtr(obj)) return ERROR;
    obj->access = access;
    return OK;
}

STATUS_t * objTypeStatus(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->status;
}

int objTypeSetStatus(OBJECT_TYPE_t *obj, STATUS_t *status) {
    if (isNullPtr(obj)) return ERROR;
    obj->status = status;
    return OK;
}

DESC_t * objTypeDesc(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->desc;
}

int objTypeSetDesc(OBJECT_TYPE_t *obj, DESC_t *DESC) {
    if (isNullPtr(obj)) return ERROR;
    obj->desc = DESC;
    return OK;
}

REF_t * objTypeRef(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->ref;
}

int objTypeSetRef(OBJECT_TYPE_t *obj, REF_t *ref) {
    if (isNullPtr(obj)) return ERROR;
    obj->ref = ref;
    return OK;
}

INDEX_t * objTypeIndex(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->index;
}

int objTypeSetIndex(OBJECT_TYPE_t *obj, INDEX_t *index) {
    if (isNullPtr(obj)) return NULL;
    obj->index = index;
    return OK;
}

MOUNT_t * objTypeMount(OBJECT_TYPE_t *obj) {
    if (isNullPtr(obj)) return NULL;
    return obj->mount;
}

int objTypeSetMount(OBJECT_TYPE_t *obj, MOUNT_t *mount) {
    if (isNullPtr(obj)) return ERROR;
    obj->mount = mount;
    return OK;
}

/* Syntax */
SYNTAX_iter * syntaxGetIter(SYNTAX_t *syn, SYNTAX_iter *iter) {
    return sliceGetIter(syn->vals, iter);
}

SYNTAX_iter * syntaxSuccessor(SYNTAX_iter *i) {
    return sliceSuccessor(i);
}

SYNTAX_iter *syntaxPredecessor(SYNTAX_iter *i) {
    return slicePredecessor(i);
}

slice * syntaxNext(SYNTAX_iter *i) {
    return sliceNext(i);
}

slice * syntaxPrev(SYNTAX_iter *i) {
    return slicePrev(i);
}

/* Index */
INDEX_iter * indexGetIter(INDEX_t *idx, INDEX_iter *iter) {
    return sliceGetIter(idx->idxs, iter);
}

INDEX_iter * indexSuccessor(INDEX_iter *i) {
    return sliceSuccessor(i);
}

INDEX_iter * indexPredecessor(INDEX_iter *i) {
    return slicePredecessor(i);
}

slice * indexNext(INDEX_iter *i) {
    return sliceNext(i);
}

slice * indexPrev(INDEX_iter *i) {
    return slicePrev(i);
}

/* yy_syn.def.c */

