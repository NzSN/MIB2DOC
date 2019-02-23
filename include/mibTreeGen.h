//
// Created by ayden on 2017/4/19.
//

#ifndef GL5610_MIB_DOC_GEN_TOKENOP_H
#define GL5610_MIB_DOC_GEN_TOKENOP_H

#include "type.h"
#include "list.h"
#include "dispatcher.h"
#include "mibTreeObjTree.h"

extern mibTreeHead trees;

#define MIB_TREE_R (&trees)

enum {
    SYMBOL_FOUND,
    SYMBOL_NOT_FOUND
};

typedef struct targetSymbolList {
    char *symbolName;
     // Value : 
     //     1. SYMBOL_FOUND
     //     2. SYMBOL_NOT_FOUND     
    int state;
    listNode node;
} targetSymbolList;

int mibObjGen(int type);
int mibObjGen_Leave();
int mibObjGen_InnerNode();
int mibObjGen_trap();
int symbolCollecting(int type, dispatchParam *param);
int symbolCollectingInit();

int documentGen(mibTreeHead *treeHead, FILE *writeTo);

/* Note: this macro is only use for symbolCollect_XXXX series function */
#define PARAM_STORE_TO_SYM_LIST(type, param) ({\
    char *string; \
    string = (char *)disParamRetrive(&param)->param; \
    sliceStore(&symCollectSlice, sliceConstruct(type, string)); \
})

#ifdef MIB2DOC_UNIT_TESTING

void mibTreeGen__SYMBOL_COLLECT(void **state);

#endif 

#endif //GL5610_MIB_DOC_GEN_TOKENOP_H
