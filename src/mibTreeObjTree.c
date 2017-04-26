//
// Created by ayden on 2017/4/24.
//

#include <mem.h>
#include <malloc.h>
#include "../include/mibTreeObjTree.h"
#include "../include/type.h"

/* Global */
mibObjectTreeNode *mibObjectTreeRoot;

/* Define */
#define OID_LENGTH 256

#define IS_NODE_HAS_CHILD_MT(node) (node->child ? 1 : 0)
#define IS_NODE_HAS_SIBLING_MT(node) (node->sibling ? 1 : 0)

/* Declaration */
static mibObjectTreeNode * next_mt(mibObjectTreeNode *obj);
static mibObjectTreeNode * mibNodeBuild(char *ident, char *oid);

void mibObjectTreeInit(mibObjectTreeNode *root) {
    mibObjectTreeNode *obj;

    memset(root, 0, sizeof(mibObjectTreeNode));

    insert_mot(root, mibNodeBuild("iso", "1"), NULL);
    insert_mot(root, mibNodeBuild("org", "1.3"), "iso");
    insert_mot(root, mibNodeBuild("dod", "1.3.6"), "org");
    insert_mot(root, mibNodeBuild("internet", "1.3.6.1"), "dod");
    insert_mot(root, mibNodeBuild("private", "1.3.6.1.4"), "internet");
    insert_mot(root, mibNodeBuild("enterprises", "1.3.6.1.4.1"), "private");
}

mibObjectTreeNode * mibNodeBuild(char *ident, char *oid) {
    mibObjectTreeNode *obj;
    mibNodeInfo *info;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(oid))
        return NULL;

    info = (mibNodeInfo *)malloc(sizeof(mibNodeInfo));
    memset(info, 0, sizeof(mibNodeInfo));

    info->oid = (char *)malloc(OID_LENGTH);
    info->ident = ident;
    info->oid = oid;

    obj = (mibObjectTreeNode *)malloc(sizeof(mibObjectTreeNode));
    memset(obj, 0, sizeof(mibObjectTreeNode));

    obj->isNode = 1;
    obj->info = (void *)info;
    return obj;
}

mibObjectTreeNode *mibLeaveBuild(char *ident, char *type, char *rw, char *desc, char *oid) {
    mibObjectTreeNode *obj;
    mibLeaveInfo *info;

    if (IS_PTR_NULL(ident) || IS_PTR_NULL(type) || IS_PTR_NULL(rw) ||
            IS_PTR_NULL(desc) || IS_PTR_NULL(oid))
        return NULL;

    info = (mibLeaveInfo *)malloc(sizeof(mibLeaveInfo));
    memset(info, 0, sizeof(mibLeaveInfo));

    info->nodeInfo->ident = ident;
    info->nodeInfo->oid = oid;
    info->type = type;
    info->rw = rw;
    info->desc = desc;


    obj = (mibObjectTreeNode *)malloc(sizeof(mibObjectTreeNode));
    memset(obj, 0, sizeof(mibObjectTreeNode));
    obj->info = info;
    obj->isNode = 0;

    return obj;
}

/* If parent is root node ,then <parent_ident> can be NULL */
int insert_mot(mibObjectTreeNode *root, mibObjectTreeNode *obj, char *parent_ident) {
    mibObjectTreeNode *parentNode, *child, *current;

    if (IS_PTR_NULL(root) || IS_PTR_NULL(obj))
        return -1;

    parentNode = search_mot(root, parent_ident);
    child = parentNode->child;
    current = child;

    if (IS_NODE_HAS_CHILD_MT(parentNode)) {
        if (IS_NODE_HAS_SIBLING_MT(child)) {
            while (current = next_mt(current)) {
                if (IS_NODE_HAS_SIBLING_MT(current))
                    continue;
                current->sibling = obj;
                obj->parent = parentNode;
                obj->head = child;
            }
            return -1;
        }

        child->sibling = obj;
        obj->parent = parentNode;
        obj->head = child;
    } else {
        parentNode->child = obj;
        obj->head = obj;
    }
}

mibObjectTreeNode *getParent_mot(mibObjectTreeNode *root, char *ident) {
    mibObjectTreeNode *node;

    node = search_mot(root, ident);
    if (node != NULL)
        return node->parent;
    else
        return NULL;
}

mibObjectTreeNode * search_mot(mibObjectTreeNode *root, char *const ident) {
    mibObjectTreeNode *current;

    if (IS_PTR_NULL(root)) {
        return NULL;
    }

    if (ident == NULL) {
        return root;
    }

    for (current = next_mt(root); current != NULL; current = next_mt(current)) {
        if (strncmp(current->info, ident, strlen(ident)) == 0) {
            return current;
        }
    }
}

static mibObjectTreeNode * next_mt(mibObjectTreeNode *obj) {
    if (IS_PTR_NULL(obj))
        return NULL;
    if (obj->sibling != NULL) {
        return obj->sibling;
    } else if (obj->child != NULL) {
        return obj->head->child;
    }
}