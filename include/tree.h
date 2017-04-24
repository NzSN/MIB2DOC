//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_TREE_H
#define GL5610_MIB_DOC_GEN_TREE_H

typedef struct mibObjectTreeNode {
    char *ident;
    char *oid;
    struct mibObjectTreeNode *parent;
    /* chaild point to first child of children */
    struct mibObjectTreeNode *child;
    struct mibObjectTreeNode *sibling;
    struct mibObjectTreeNode *head;
} mibObjectTreeNode;

void insert_mt(mibObjectTreeNode* root,mibObjectTreeNode *obj, char *parent);
mibObjectTreeNode * getParent_mt(mibObjectTreeNode *root, char *ident);

#endif //GL5610_MIB_DOC_GEN_TREE_H
