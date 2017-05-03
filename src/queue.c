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
    ret; \
})

static int isQueueFull(tableInfoQueue *q);
static int isQueueEmpty(tableInfoQueue *q);

int appendQueue(tableInfoQueue *q, tableInfo *pData) {
    if (isQueueFull(q))
        return -1;
    q->tableBuffer[q->tail] = pData;
    q->tail++;
}

void *getQueue(tableInfoQueue *q) {
    void *ret;
    if (isQueueEmpty(q))
        return 0;
    ret = q->tableBuffer[q->head];
    q->head++;
    return ret;
}

static int isQueueFull(tableInfoQueue *q) {
    int head = q->head;
    int tail = q->tail;

    if (Qnext(tail) == head)
        return 1;
    else
        return 0;
}

static int isQueueEmpty(tableInfoQueue *q) {
    if (q->head == q->tail)
        return 1;
    else
        return 0;
}