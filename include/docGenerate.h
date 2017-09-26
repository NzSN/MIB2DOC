//
// Created by ayden on 2017/5/3.
//

#include "type.h"

#ifndef UNIT_TEST_DOCGENERATE_H
#define UNIT_TEST_DOCGENERATE_H

typedef struct tableInfo {
    char *identifier;
    char *oid;
    char *rw;
    char *type;
    char *desc;
} tableInfo;

int documentGen(mibObjectTreeNode *root, FILE *writeTo);
int tableRecognize(char *buffer, int size);

#endif /* UNIT_TEST_DOCGENERATE_H */

