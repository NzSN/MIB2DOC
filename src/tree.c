//
// Created by ayden on 2017/4/24.
//

#include <mem.h>
#include "../include/tree.h"
#include "../include/type.h"

/* Define */
#define IS_NODE_HAS_CHILD_MT(node) (node->child ? 1 : 0)
#define IS_NODE_HAS_SIBLING_MT(node) (node->sibling ? 1 : 0)

/* Declaration */
static mibObjectTreeNode * next_mt(mibObjectTreeNode *obj);
static mibObjectTreeNode * search_mt(mibObjectTreeNode *root, char * const ident);

void insert_mt(mibObjectTreeNode *root, mibObjectTreeNode *obj, char *parent_ident) {
    mibObjectTreeNode *parentNode, *child, *current;

    parentNode = search_mt(root, parent_ident);
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
            return;
        }

        child->sibling = obj;
        obj->parent = parentNode;
        obj->head = child;
    } else {
        parentNode->child = obj;
        obj->head = obj;
    }
}

mibObjectTreeNode *getParent_mt(mibObjectTreeNode *root, char *ident) {
    mibObjectTreeNode *node;

    node = search_mt(root, ident);
    if (node != NULL)
        return node->parent;
    else
        return NULL;
}

static mibObjectTreeNode * search_mt(mibObjectTreeNode *root, char * const ident) {
    mibObjectTreeNode *current;

    if (IS_PTR_NULL(root) || IS_PTR_NULL(ident)) {
        return NULL;
    }

    if (strncmp(root->ident, ident, strlen(ident)) == 0) {
        return root;
    }

    for (current = next_mt(root); current != NULL; current = next_mt(current)) {
        if (strncmp(current->ident, ident, strlen(ident)) == 0) {
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