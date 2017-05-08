//
// Created by ayden on 2017/4/24.
//
#include "../include/queue.h"

#define Qnext(idx) ({ \
    int ret = idx; \
    if (idx == SIZE_OF_QUEUE - 1) { \
        ret = 0; \
    }      \
    else { \
        ret++; \
    } \
    idx = ret; \
})

static int isQueueFull(mibNodeInfoQueue *q);
static int isQueueEmpty(mibNodeInfoQueue *q);

int appendQueue(mibNodeInfoQueue *q, tableInfo *pData) {
    if (isQueueFull(q))
        return -1;
    q->tableBuffer[q->tail] = pData;
    Qnext(q->tail);
    q->count++;

    return 0;
}

tableInfo *getQueue(mibNodeInfoQueue *q) {
    tableInfo *ret;
    if (isQueueEmpty(q))
        return 0;
    ret = q->tableBuffer[q->head];
    Qnext(q->head);
    q->count--;
    return ret;
}

static int isQueueFull(mibNodeInfoQueue *q) {
    int head = q->head;
    int tail = q->tail;

    if (Qnext(tail) == head)
        return 1;
    else
        return 0;
}

static int isQueueEmpty(mibNodeInfoQueue *q) {
    if (q->head == q->tail)
        return 1;
    else
        return 0;
}