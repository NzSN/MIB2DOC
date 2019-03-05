/* yy_syn.def.h */

#ifndef __YY_SYN_DEF_H__
#define __YY_SYN_DEF_H__

#include "list.h"

typedef struct sequence_item {
    char *ident;
    char *type;    
    listNode node;
} sequence_item;

typedef struct sequence {
    int length;
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

#endif /* __YY_SYN_DEF_H__ */

/* yy_syn.def.h */

