/* yy_syn.def.c */

#include <malloc.h>
#include <string.h>
#include "type.h"
#include "yy_syn.def.h"

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
    seqCopy->type = strdup(seqItem->ident);

    return seqCopy;
}

sequence_item * seqItemsCopy(const sequence_item *seqItem) {
    if (isNullPtr(seqItem)) 
        return NULL;
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
    isIdentEqual = isAttrEqual(lSeqAttr, rSeqAttr); 

    lSeqAttr = lSeqItem->type;
    rSeqAttr = rSeqItem->type;
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
    seqItemDestruct(firstItem);  

    RELEASE_MEM(seq);

    return OK;
}

// Copy constructor
sequence * seqCopy(const sequence *seq) {
    if (isNullPtr(seq))
        return NULL;

    sequence *seq_copy = seqConst();
    
    sequence_item *firstItem = seqItemNext(&seq->items);
    sequence_item *seqItem_copy = seqItemsCopy(firstItem);

    if (seqItemAppend(&seq_copy->items, seqItem_copy) == ERROR)
        return NULL;
    
    seq_copy->length = seq->length;  
    
    return NULL;
}

// Assignment
int seqAssignment(sequence *seq_lval, const sequence *seq_rval) {
    if (isNullPtr(seq_lval) || isNullPtr(seq_lval))
        return ERROR;
    
    if (seqItemDestruct(seqItemNext(&seq_lval->items)) == ERROR)
        return ERROR;
    
    sequence_item *firstItem = seqItemNext(&seq_rval->items);
    sequence_item *seqItem_copy = seqItemsCopy(firstItem);    

    seqItemAppend(&seq_lval->items, seqItem_copy);

    return OK;
}

// Equality
_Bool seqIsEqual(sequence *leftSeq, sequence *rightSeq) {
    if (isNullPtr(leftSeq) || isNullPtr(rightSeq))
        return FALSE;
    
    if (leftSeq->length != rightSeq->length) 
        return FALSE;
    
    int idx = 0, length = leftSeq->length;
    sequence_item *current_left = &leftSeq->items;
    sequence_item *current_right = &rightSeq->items;

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

void yy_syn_def__SEQUENCE(void **state) {
    /* Sequence item test */
    sequence_item *new, *item = seqItemConst();  
    
    // Append
    int i = 0;
    while (i++ < 100) {
        new = seqItemConst();
        new->ident = randStr(10);
        new->type = randStr(10);

        assert_int_equal(seqItemAppend(item, new), OK);
    } 
    
    seqItemsDestruct(item);     

    /* Sequence test */
    
}

#endif /* MIB2DOC_UNIT_TESTING */

/* yy_syn.def.c */

