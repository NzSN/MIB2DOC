//
// Created by ayden on 2017/4/24.
//

#ifndef GL5610_MIB_DOC_GEN_TREE_H
#define GL5610_MIB_DOC_GEN_TREE_H

#include "list.h"
#include "symbolTbl.h"

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
    struct {
        char *parent; 
    } mergeInfo;
} mibObjectTreeNode;

typedef struct mibTree {
    char *rootName;
    mibObjectTreeNode *root;
    listNode node;
} mibTree;

typedef struct mibTreeHead {
    int numOfTree;
    mibTree *last;
    mibTree trees;
} mibTreeHead;

enum {
    VISIT_PRE,
    VISIT_IN,
    VISIT_POST
};

typedef int (*travelFunc)(void *argu, mibObjectTreeNode *node);

/* mibObjectTreeNode functions */
#define MIB_OBJ_TREE_NODE_PARENT_NAME(node) ((node)->mergeInfo.parent)
#define MIB_OBJ_TREE_NODE_PARENT(node) ((node)->parent)
#define MIB_OBJ_TREE_NODE_CHILD(node) ((node)->child)
#define MIB_OBJ_TREE_NODE_SIBLING(node) ((node)->sibling)

int mibObjectTreeInit();
int insert_MibTree(mibObjectTreeNode *root, mibObjectTreeNode *obj, char *parent);
mibObjectTreeNode * parent_MibTree(mibObjectTreeNode *root, char *ident);
mibObjectTreeNode * mibLeaveBuild(char *ident, char *type, char *rw, 
    char *desc, char *oid, char *parent);
mibObjectTreeNode * mibNodeBuild(char *ident, char *oid, char *parent);
mibObjectTreeNode * search_MibTree(mibObjectTreeNode *root, char *const ident);
char * getIdentFromInfo(mibObjectTreeNode *node);
char * getOidFromInfo(mibObjectTreeNode *node);
int showTree(mibTreeHead *head);
mibObjectTreeNode * travel_MibTree(mibObjectTreeNode *obj, travelFunc func, void *arg);

/* mibTree operations */
#define MIBTREE_IS_FIRST_TREE(MIBTREE) (MIBTREE->node.prev == NULL)
#define MIBTREE_IS_LAST_TREE(MIBTREE) (MIBTREE->node.next == NULL)

mibTree * mibTreeConstruction();
int mibTreeSetRoot(mibTree *tree, mibObjectTreeNode *rootNode);
mibTree * mibTreeNext(mibTree *tree);
mibTree * mibTreePrev(mibTree *tree);
mibTree * mibTreeSearch(mibTree *tree, char *name);
int mibTreeAppend(mibTree *head, mibTree *now);
mibTree * mibTreeDelete(mibTree *node);
mibTree * mibTreeDeleteByName(mibTree *treeListHead, char *name);

// Merge lTree and rTree and return the root of the tree.
mibTree * mibTreeMerge(mibTree *lTree, mibTree *rTree);

/* mibTreeHead functions */
int mibTreeHeadInit(mibTreeHead *treeHead);
int mibTreeHeadMerge_LAST(mibTreeHead *treeHead);
int mibTreeHeadMerge(mibTreeHead *treeHead);
int mibTreeHeadComplete(mibTreeHead *treeHead, symbolTable *symTbl);
int mibTreeHeadAppend(mibTreeHead *treeHead, mibObjectTreeNode *newNode);
mibTree * mibTreeHeadFirstTree(mibTreeHead *treeHead);

/* mibTree type functions */
_Bool isMibNodeType_TABLE(mibObjectTreeNode *node);
_Bool isMibNodeType_ENTRY(mibObjectTreeNode *node);

#ifdef MIB2DOC_UNIT_TESTING
#include "test.h"

void mibTreeObjTree__MIBTREE_MERGE(void **state);

#endif /* MIB2DOC_UNIT_TESTING */

#endif //GL5610_MIB_DOC_GEN_TREE_H

