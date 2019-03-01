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
    sequence_item items; 
} sequence;



#endif /* __YY_SYN_DEF_H__ */

/* yy_syn.def.h */

