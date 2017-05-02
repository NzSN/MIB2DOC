//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_TREE_H
#define GL5610_MIB_DOC_GEN_TREE_H

typedef struct mibNodeInfo {
    char *ident;
    char *oid;
} mibNodeInfo;

typedef union mibInfo {
    mibNodeInfo *nodeInfo;
    char *rw;
    char *type;
    char *desc;
    char *detail;
} mibLeaveInfo;

typedef struct mibObjectTreeNode {
    int isNode;
    void *info;
    struct mibObjectTreeNode *parent;
    /* chaild point to first child of children */
    struct mibObjectTreeNode *child;
    struct mibObjectTreeNode *sibling;
    struct mibObjectTreeNode *head;
} mibObjectTreeNode;


void mibObjectTreeInit(mibObjectTreeNode *root);
int insert_mot(mibObjectTreeNode *root, mibObjectTreeNode *obj, char *parent);
mibObjectTreeNode * parent_mot(mibObjectTreeNode *root, char *ident);
mibObjectTreeNode *mibLeaveBuild(char *ident, char *type, char *rw, char *desc, char *oid);
mibObjectTreeNode * mibNodeBuild(char *ident, char *oid);
mibObjectTreeNode * search_mot(mibObjectTreeNode *root, char *const ident);
char * getIdentFromInfo(mibObjectTreeNode *node);
char * getOidFromInfo(mibObjectTreeNode *node);
#endif //GL5610_MIB_DOC_GEN_TREE_H
