//
// Created by ayden on 2017/4/24.
//

#include "../include/docGenerate.h"

#ifndef GL5610_MIB_DOC_GEN_QUEUE_H
#define GL5610_MIB_DOC_GEN_QUEUE_H

#define SIZE_OF_QUEUE 512

typedef struct tableInfoQueue {
    int head;
    int tail;
    tableInfo *tableBuffer[SIZE_OF_QUEUE];
} tableInfoQueue;

int appendQueue(tableInfoQueue *q, tableInfo *f);
void *getQueue(tableInfoQueue *q);

#endif //GL5610_MIB_DOC_GEN_QUEUE_H
