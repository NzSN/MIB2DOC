//
// Created by ayden on 2017/4/24.
//

#include "../include/docGenerate.h"

#ifndef GL5610_MIB_DOC_GEN_QUEUE_H
#define GL5610_MIB_DOC_GEN_QUEUE_H

#define SIZE_OF_QUEUE 512

typedef struct mibNodeInfoQueue {
    int head;
    int tail;
    int count;
    tableInfo *tableBuffer[SIZE_OF_QUEUE];
} mibNodeInfoQueue;

int appendQueue(mibNodeInfoQueue *q, tableInfo *f);
tableInfo *getQueue(mibNodeInfoQueue *q);

#endif //GL5610_MIB_DOC_GEN_QUEUE_H