//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_TREE_H
#define GL5610_MIB_DOC_GEN_TREE_H

#include "list.h"

typedef struct mibNodeInfo {
    char *ident;
    char *oid;
} mibNodeInfo;

typedef struct mibInfo {
    mibNodeInfo *nodeInfo;
    char *rw;
    char *type;
    char *desc;
    char *detail;
} mibLeaveInfo;

typedef struct mibObjectTreeNode {
    int isNode;
    void *info;
    char *identifier;
    struct mibObjectTreeNode *parent;
    /* chaild point to first child of children */
    struct mibObjectTreeNode *child;
    struct mibObjectTreeNode *sibling;
    struct mibObjectTreeNode *head;
} mibObjectTreeNode;

typedef struct {
    mibObjectTreeNode *lRef;
    listNode node;
} leaveNodeRef;

typedef struct mibTree {
    char *rootName;
    mibObjectTreeNode root;
    // References to leave node of the tree
    // for the purpose of merge.
    leaveNodeRef lRef;
    listNode node;
} mibTree;

typedef struct mibTreeHead {
    int numOfTree;
    mibTree *trees;
} mibTreeHead;

void mibObjectTreeInit(mibObjectTreeNode *root);
int insert_MibTree(mibObjectTreeNode *root, mibObjectTreeNode *obj, char *parent);
mibObjectTreeNode * parent_MibTree(mibObjectTreeNode *root, char *ident);
mibObjectTreeNode * mibLeaveBuild(char *ident, char *type, char *rw, char *desc, char *oid);
mibObjectTreeNode * mibNodeBuild(char *ident, char *oid);
mibObjectTreeNode * search_MibTree(mibObjectTreeNode *root, char *const ident);
char * getIdentFromInfo(mibObjectTreeNode *node);
char * getOidFromInfo(mibObjectTreeNode *node);
void showTree(mibObjectTreeNode *root);
mibObjectTreeNode * travel_MibTree(mibObjectTreeNode *obj, int (*func)(void *argu, mibObjectTreeNode *node), void *arg);

// mibTree operations
#define MIBTREE_IS_FIRST_TREE(MIBTREE) (MIBTREE->node.prev == NULL)
#define MIBTREE_IS_LAST_TREE(MIBTREE) (MIBTREE->node.next == NULL)
mibTree *mibTreeConstruction(mibTree *tree);
mibTree * mibTreeNext(mibTree *tree);
mibTree *mibTreePrev(mibTree *tree);


#endif //GL5610_MIB_DOC_GEN_TREE_H
