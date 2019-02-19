//
// Created by ayden on 2017/4/25.
//

#include "test.h"
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "list.h"
#include "mibTreeGen.h"
#include "mibTreeObjTree.h"
#include "queue.h"
#include "stack.h"
#include "dispatcher.h"
#include "hash.h"
#include <stdio.h>
#include "test.h"

mibObjectTreeNode mibObjectTreeRoot;
slice sliceContainer;
char *sectionRecord[SIZE_OF_SECTION_RECORD];

int tableRecognize(char *buffer, int size);

static void list__LISTNODE_SLICE(void **state) {
    slice *new1;
    slice *new2;
    slice *got;

    memset(&sliceContainer, 0, sizeof(slice));

    new1 = sliceConstruct(SLICE_IDENTIFIER, strdup("LLLL"));
    sliceStore(&sliceContainer, new1);

    new2 = sliceConstruct(SLICE_TYPE, strdup("lllll"));
    sliceStore(&sliceContainer, new2);

    got = sliceGet(&sliceContainer, SLICE_IDENTIFIER);
    if ( (got->sliVal != new1->sliVal) || (got->sliKey != new1->sliKey))
        fail();

    got = sliceGet(&sliceContainer, SLICE_TYPE);
    if ( (got->sliVal != new2->sliVal) || (got->sliKey != new2->sliKey))
        fail();

    sliceRelease_STATIC(&sliceContainer);
    memset(&sliceContainer, 0, sizeof(slice));

    typedef struct {
        int idx;
        listNode node;
    } test_list;
    
    int i = 1;
    test_list head, *current;
    memset(&head, 0, sizeof(test_list));

    while (i < 100) {
        current = (test_list *)malloc(sizeof(test_list)); 
        memset(current, 0, sizeof(test_list));
        current->idx = i;
        listNodeInsert(listNodeTail(&head.node), &current->node);
        ++i;
    }
    
    current = &head;

    do {
        if (current->node.next == NULL)
            return;
        current = containerOf(listNodeNext(&current->node), test_list, node);     
    } while (current != NULL); 

    // list delete testing 
     
}


static void mibTree_test(void **state) {
    mibObjectTreeNode *pNode;
    mibNodeInfo *info;
    char *ident;
    char *suffix;
    char *parent;

    mibObjectTreeInit(&mibObjectTreeRoot);
    ident = (char *)calloc(1, strlen("gogo")+1);
    suffix = (char *)calloc(1, strlen("1")+1);
    parent = (char *)calloc(1, strlen("enterprises")+1);
    
    strncpy(ident, "gogo", strlen("gogo"));
    strncpy(suffix, "1", strlen("1"));
    strncpy(parent, "enterprises", strlen("enterpises"));

    sliceStore(&sliceContainer, sliceConstruct(SLICE_IDENTIFIER, ident));
    sliceStore(&sliceContainer, sliceConstruct(SLICE_OID_SUFFIX, suffix));
    sliceStore(&sliceContainer, sliceConstruct(SLICE_PARENT, parent));

    mibObjectTreeInit(&mibObjectTreeRoot);
    mibObjGen_InnerNode();
    pNode = search_MibTree(&mibObjectTreeRoot, "gogo");
    if (strncmp(pNode->identifier, "gogo", strlen("gogo")) != 0)
        fail();
}

static void queue__QUEUE_APPEND(void **state) {
    mibNodeInfoQueue queue;
    tableInfo *pInfo;

    memset(&queue, 0, sizeof(mibNodeInfoQueue));

    tableInfo i1, i2, i3;

    i1.identifier = "1";
    i1.desc = "desc1";
    i1.oid = "1";
    i1.rw = "rw1";
    i1.type = "type1";

    i2.identifier = "2";
    i2.desc = "desc2";
    i2.oid = "2";
    i2.rw = "rw2";
    i2.type = "type2";

    i3.identifier = "3";
    i3.desc = "desc3";
    i3.oid = "3";
    i3.rw = "rw3";
    i3.type = "type3";

    appendQueue(&queue, &i1);
    appendQueue(&queue, &i2);
    appendQueue(&queue, &i3);

    pInfo = getQueue(&queue);
    if (strncmp(pInfo->identifier, i1.identifier, strlen(i1.identifier)) != 0 ||
            strncmp(pInfo->desc, i1.desc, strlen(i1.desc)) != 0 ||
            strncmp(pInfo->oid, i1.oid, strlen(i1.oid)) != 0 ||
            strncmp(pInfo->rw, i1.rw, strlen(i1.rw)) != 0 ||
            strncmp(pInfo->type, i1.type, strlen(i1.type)) != 0)
        fail();

    pInfo = getQueue(&queue);
    if (strncmp(pInfo->identifier, i2.identifier, strlen(i2.identifier)) != 0 ||
        strncmp(pInfo->desc, i2.desc, strlen(i2.desc)) != 0 ||
        strncmp(pInfo->oid, i2.oid, strlen(i2.oid)) != 0 ||
        strncmp(pInfo->rw, i2.rw, strlen(i2.rw)) != 0 ||
        strncmp(pInfo->type, i2.type, strlen(i2.type)) != 0)
        fail();

    pInfo = getQueue(&queue);
    if (strncmp(pInfo->identifier, i3.identifier, strlen(i3.identifier)) != 0 ||
        strncmp(pInfo->desc, i3.desc, strlen(i3.desc)) != 0 ||
        strncmp(pInfo->oid, i3.oid, strlen(i3.oid)) != 0 ||
        strncmp(pInfo->rw, i3.rw, strlen(i3.rw)) != 0 ||
        strncmp(pInfo->type, i3.type, strlen(i3.type)) != 0)
        fail();
}

static void UNIT_TEST__DESC_CHECK(void ** state) {
    char *ident = "inactiveLineDeviceType";
    char *parentIdent = "inactiveLinePrimaryEntry";
    char *result;
    int sumChild, sumParent, pos, i, size;

    if (strlen(ident) > strlen(parentIdent))
        size = strlen(parentIdent);
    else
        size = strlen(ident);

    pos = -1;
    sumChild = 0;
    sumParent = 0;
    for (i=0; i<size; i++) {
        sumChild += ident[i];
        sumParent += parentIdent[i];

        if (sumChild == sumParent) {
            pos = i;
            continue;
        }
        break;
    }
    result = (char *)malloc(pos+2);
    memset(result, 0, pos+2);
    strncpy(result, ident+pos+1, size-(pos+1));
}

static void util__tableRecognize(void **state) {
    int ret;
    char *buffer = "inactiveVlanTable";

    ret = tableRecognize(buffer, strlen(buffer));

    if (ret != 1)
        fail();
}

static void list__DISPATCH_PARAM_STORE(void **state) { 
    char *IDENTIFIER_S = "GOGO";
    dispatchParam *param = disParamConstruct((void *)SLICE_PARENT);
    disParamStore(param, disParamConstruct((void *)IDENTIFIER_S));

    if ((unsigned long)disParamRetrive(&param)->param != SLICE_PARENT)
        fail();
    if (strncmp(disParamRetrive(&param)->param, IDENTIFIER_S, strlen(IDENTIFIER_S)) != 0)
        fail();
    
    int i = 0;
    dispatchParam *massive = disParamConstruct((void *)100);
    while (i < 100) {
        disParamStore(massive, disParamConstruct((void *)i+1)); 
        ++i;
    }
    
    i = 0;
    dispatchParam *current = massive;
    while (current = dispatchParamNext(current)) {
        if ((unsigned long)current->param != i+1)
            fail();
        ++i;
    }
    
    disParamRelease(massive, NULL);
}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(list__LISTNODE_SLICE),
            cmocka_unit_test(util__tableRecognize),
            cmocka_unit_test(queue__QUEUE_APPEND),
            cmocka_unit_test(UNIT_TEST__DESC_CHECK),
            cmocka_unit_test(list__DISPATCH_PARAM_STORE),
            
            // Module unit testing
            #ifdef MIB2DOC_UNIT_TESTING
            cmocka_unit_test(stack__STACK_PUSH_POP),
            cmocka_unit_test(dispatcher__COLLECT_INFO),
            cmocka_unit_test(hash__HASH_BASIC),
            cmocka_unit_test(mibTreeObjTree__MIBTREE_MERGE),
            cmocka_unit_test(list_symbolTable)
            #endif
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
