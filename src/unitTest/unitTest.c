//
// Created by ayden on 2017/4/25.
//

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <malloc.h>
#include <string.h>
#include "../../include/type.h"
#include "../../include/list.h"
#include "../../include/mibTreeGen.h"
#include "../../include/queue.h"

elementList elistHead;
char *sectionRecord[SIZE_OF_SECTION_RECORD];

int tableRecognize(char *buffer, int size);

/* A test case that does nothing and succeeds. */
static void list_test(void **state) {
    elementList *new1;
    elementList *new2;
    elementList *got;

    memset(&elistHead, 0, sizeof(elementList));

    new1 = buildElement(IDENTIFIER_EL, "LLLL");
    appendElement_el(&elistHead, new1);

    new2 = buildElement(TYPE_EL, "lllll");
    appendElement_el(&elistHead, new2);

    got = getElement_el(&elistHead, IDENTIFIER_EL);
    if ( (got->content != new1->content) || (got->key != new1->key))
        fail();

    got = getElement_el(&elistHead, TYPE_EL);
    if ( (got->content != new2->content) || (got->key != new2->key))
        fail();

    flushAll_el(&elistHead);
    got = getElement_el(&elistHead, IDENTIFIER_EL);
    if (got != NULL)
        fail();

}


static void mibTree_test(void **state) {
    mibObjectTreeNode *pNode;
    mibNodeInfo *info;
    char *ident;
    char *suffix;
    char *parent;

    mibObjectTreeInit(&mibObjectTreeRoot);
    ident = (char *)malloc(strlen("gogo"));
    suffix = (char *)malloc(strlen("1"));
    parent = (char *)malloc(strlen("enterprises"));

    strncpy(ident, "gogo", strlen("gogo"));
    strncpy(suffix, "1", strlen("1"));
    strncpy(parent, "enterprises", strlen("enterpises"));

    appendElement_el(&elistHead, buildElement(IDENTIFIER_EL, ident));
    appendElement_el(&elistHead, buildElement(SUFFIX_EL, suffix));
    appendElement_el(&elistHead, buildElement(PARENT_EL, parent));

    deal_with_objIdent();
    pNode = search_mot(&mibObjectTreeRoot, "gogo");

}

static void tableInfoQueue_test(void **state) {
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

static void desc_test(void ** state) {
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

static void fa_test(void **state) {
    int ret;
    char *buffer = "inactiveVlanTable";

    ret = tableRecognize(buffer, strlen(buffer));

    if (ret != 1)
        fail();
}

int main(void) {
    const struct CMUnitTest tests[] = {
            //cmocka_unit_test(list_test),
            //cmocka_unit_test(mibTree_test),
            //cmocka_unit_test(fa_test),
            //cmocka_unit_test(tableInfoQueue_test),
            cmocka_unit_test(desc_test)
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}