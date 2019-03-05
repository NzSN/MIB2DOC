/* yy_syn.def.h */

#ifndef __YY_SYN_DEF_H__
#define __YY_SYN_DEF_H__

#include "list.h"

typedef struct {
    char *ident;
    char *type;    
    listNode node;
} sequence_item;

typedef struct {
    int length;
    sequence_item items; 
} sequence;

// sequence_item's procedures
sequence_item * seqItemNext(const sequence_item *item);

#ifdef MIB2DOC_UNIT_TESTING

void yy_syn_def__SEQUENCE(void **state);

#endif

#endif /* __YY_SYN_DEF_H__ */

/* yy_syn.def.h */

