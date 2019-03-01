/* yy_syn.def.c */

#include <malloc.h>
#include <string.h>
#include "type.h"
#include "yy_syn.def.h"

/* sequence && sequence_item */

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
    
    RELEASE_MEM(seqItem->ident);
    RELEASE_MEM(seqItem->type);
    RELEASE_MEM(seqItem);

    return OK;
}

// Copy constructor
sequence_item *seqItemCopy(sequence_item *seqItem) {
    sequence_item *seqCopy;

    if (isNullPtr(seqItem))
        return NULL;
    
    seqCopy = seqItemConst();
    seqCopy->ident = strdup(seqItem->ident);
    seqCopy->type = strdup(seqItem->ident);

    return seqCopy;
}

// Assignment
int seqItemAssignment(sequence_item *lSeqItem, sequence_item *rSeqItem) {

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
    strlen(lSeqAttr) == strlen(rSeqAttr) && \
    !strncmp(lSeqAttr, rSeqAttr, strlen(lSeqAttr));

int seqItemIsEqual(sequence_item *lSeqItem, sequence_item *rSeqItem) {
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

sequence_item * seqItemPrev(sequence_item *seqItem) {
    if (isNullPtr(seqItem) && isNullPtr(seqItem->node.prev))
        return NULL;

    return containerOf(listNodePrev(&seqItem->node), sequence_item, node);
}

sequence_item * seqItemNext(sequence_item *seqItem) {
    if (isNullPtr(seqItem) && isNullPtr(seqItem->node.next))
        return NULL;
    return containerOf(listNodeNext(&seqItem->node), sequence_item, node);
}

#ifdef MIB2DOC_UNIT_TESTING

void yy_syn_def__SEQUENCE(void **state) {

}

#endif /* MIB2DOC_UNIT_TESTING */

/* yy_syn.def.c */

