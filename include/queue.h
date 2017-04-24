//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_QUEUE_H
#define GL5610_MIB_DOC_GEN_QUEUE_H

#define SIZE_OF_QUEUE 512

typedef struct queue {
    int head;
    int tail;
    void *buffer[SIZE_OF_QUEUE];
} Queue;

int appendQueue(Queue *q, void *f);
void *getQueue(Queue *q);

#endif //GL5610_MIB_DOC_GEN_QUEUE_H
