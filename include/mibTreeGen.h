//
// Created by ayden on 2017/4/19.
//

#ifndef GL5610_MIB_DOC_GEN_TOKENOP_H
#define GL5610_MIB_DOC_GEN_TOKENOP_H

#include "type.h"

typedef enum {
    SYMBOL_FOUND,
    SYMBOL_NOT_FOUND
};

typedef struct targetSymbolList {
    char *symbolName;
    /* 
     * VALUE : 
     *     1. SYMBOL_FOUND
     *     2. SYMBOL_NOT_FOUND
     */
    int state;
    listNode node;
} targetSymbolList;


int deal_with(int type);
int deal_with_object();
int deal_with_objIdent();
int deal_with_trap();
#endif //GL5610_MIB_DOC_GEN_TOKENOP_H
