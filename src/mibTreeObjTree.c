//
// Created by ayden on 2017/4/24.
//

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "mibTreeObjTree.h"
#include "type.h"
#include "docGenerate.h"
#include "util.h"

/* Global */
mibObjectTreeNode mibObjectTreeRoot;

/* Local */
static int nodeCmp(void *arg, mibObjectTreeNode *node);
static int Treeprint(void *arg, mibObjectTreeNode *node);
static int descriptionDeal(mibObjectTreeNode *node);

/* Define */
#define OID_LENGTH 256
#define IS_NODE_HAS_CHILD_MT(node) (node->child ? 1 : 0)
#define IS_NODE_HAS_SIBLING_MT(node) (node->sibling ? 1 : 0)
#define LOWER_CASE(C) ({ \
    int ret; \
    if (C < 97) \
        ret = C+32; \
    else \
        ret = C; \
    ret; \
})

/* Declaration */
mibObjectTreeNode * travel_MibTree(mibObjectTreeNode *obj, 
    int (*func)(void *argu, mibObjectTreeNode *node), void *arg);

void mibObjectTreeInit(mibObjectTreeNode *root) {
    mibObjectTreeNode *obj;

    memset(root, 0, sizeof(mibObjectTreeNode));
}

mibObjectTreeNode * mibNodeBuild(char *ident, char *oid, char *parent) {
    mibObjectTreeNode *obj;
    mibNodeInfo *info;

    if (isNullPtr(ident) || isNullPtr(oid))
        return NULL;

    info = (mibNodeInfo *)malloc(sizeof(mibNodeInfo));
    memset(info, 0, sizeof(mibNodeInfo));

    info->ident = ident;
    info->oid = oid;

    obj = (mibObjectTreeNode *)malloc(sizeof(mibObjectTreeNode));
    memset(obj, 0, sizeof(mibObjectTreeNode));

    obj->identifier = ident;
    obj->isNode = 1;
    obj->info = (void *)info;
    obj->mergeInfo.parent = parent;

    return obj;
}

mibObjectTreeNode *mibLeaveBuild(char *ident, char *type, char *rw, char *desc, char *oid, char *parent) {
    mibObjectTreeNode *obj;
    mibLeaveInfo *info;

    if (isNullPtr(ident) || isNullPtr(type) ||  isNullPtr(oid))
        return NULL;

    info = (mibLeaveInfo *)malloc(sizeof(mibLeaveInfo));
    memset(info, 0, sizeof(mibLeaveInfo));
    info->nodeInfo = (mibNodeInfo *)malloc(sizeof(mibNodeInfo));
    memset(info->nodeInfo, 0, sizeof(mibNodeInfo));

    info->nodeInfo->ident = ident;
    info->nodeInfo->oid = oid;
    info->type = type;
    info->rw = rw;
    info->detail = desc;

    obj = (mibObjectTreeNode *)malloc(sizeof(mibObjectTreeNode));
    memset(obj, 0, sizeof(mibObjectTreeNode));
    obj->identifier = ident;
    obj->info = (void *)info;
    obj->isNode = 0;
    obj->mergeInfo.parent = parent;

    return obj;
}

/* If parent is root node ,then <parent_ident> can be NULL */
int insert_MibTree(mibObjectTreeNode *root, mibObjectTreeNode *obj, char *parent_ident) {
    mibObjectTreeNode *parentNode, *child, *current;

    if (isNullPtr(root) || isNullPtr(obj) || isNullPtr(parent_ident))
        return -1;

    parentNode = search_MibTree(root, parent_ident);

    if (parentNode == NULL)
        return -1;

    child = parentNode->child;

    if (IS_NODE_HAS_CHILD_MT(parentNode)) {
        for (current = child; current != NULL; current = current->sibling ) {
            if (IS_NODE_HAS_SIBLING_MT(current)) {
                continue;
            }
            current->sibling = obj;
            obj->parent = parentNode;
            obj->head = child;
            goto MISC;
        }
    } else {
        parentNode->child = obj;
        obj->head = obj;
        obj->parent = parentNode;
    }
MISC:
    descriptionDeal(obj);
    return 0;
}

int setAsChild_MibTree(mibObjectTreeNode *parent, mibObjectTreeNode *child) {
    if (isNullPtr(parent) || isNullPtr(child)) {
        return FALSE; 
    }  
    
    if (IS_NODE_HAS_CHILD_MT(parent)) {
        return TRUE; 
    }

    parent->child = child;
    
    return TRUE;
}

static int descriptionDeal(mibObjectTreeNode *node) {

    int i, pos, size, descSize, sumChild, sumParent;
    char *ident = getIdentFromInfo(node);
    char *parentIdent = getIdentFromInfo(node->parent);
    mibLeaveInfo *info;
    
    if (strlen(ident) > strlen(parentIdent))
        size = strlen(parentIdent);
    else
        size = strlen(ident);

    if (!node->isNode & !tableRecognize(ident, strlen(ident))) {
        info = node->info;

        pos = -1;
        sumChild = 0;
        sumParent = 0;
        for (i=0; i<size; i++) {
            sumChild += LOWER_CASE(ident[i]);
            sumParent += LOWER_CASE(parentIdent[i]);

            if (sumChild == sumParent) {
                pos = i;
                continue;
            }
            break;
        }
        if (pos == -1)
            return 0;

        size = strlen(ident);
        descSize = size-(pos+1);

        info->desc = (char *)malloc(descSize+1);
        memset(info->desc, 0, descSize+1);
        strncpy(info->desc, ident+pos+1, descSize);
    }
    return 0;
}

mibObjectTreeNode *parent_MibTree(mibObjectTreeNode *root, char *ident) {
    mibObjectTreeNode *node;

    node = search_MibTree(root, ident);
    if (node != NULL)
        return node->parent;
    else
        return NULL;
}

mibObjectTreeNode * search_MibTree(mibObjectTreeNode *root, char *const ident) {
    mibObjectTreeNode *target;

    target = travel_MibTree(root, nodeCmp, ident);

    return target;
}

void showTree(mibObjectTreeNode *root) {
    travel_MibTree(root, Treeprint, NULL);
}

static int Treeprint(void *arg, mibObjectTreeNode *node) {
    printf("%s : %s", getIdentFromInfo(node), getOidFromInfo(node));
    if (!node->isNode)
        printf(" -- %s -- %s\n", ((mibLeaveInfo *)node->info)->type,
               ((mibLeaveInfo *)node->info)->detail);
    else
        printf("\n");
    return 0;
}

int nodeCmp(void *arg, mibObjectTreeNode *node) {
    char *ident;
    char *targetIdent;
    int size, size1, size2;
    ident = arg;

    targetIdent = getIdentFromInfo(node);
    size1 = strlen(ident);
    size2 = strlen(targetIdent);

    if (size1 < size2)
        size = size1;
    else
        size = size2;

    if (strncmp(ident, getIdentFromInfo(node), size) == 0)
        return 1;
    else
        return 0;
}

char * getIdentFromInfo(mibObjectTreeNode *node) {
    if (node->isNode)
        return ((mibNodeInfo *)node->info)->ident;
    else
        return ((mibLeaveInfo *)node->info)->nodeInfo->ident;

}

char *getOidFromInfo(mibObjectTreeNode *node) {
    if (node->isNode)
        return ((mibNodeInfo *)node->info)->oid;
    else
        return ((mibLeaveInfo *)node->info)->nodeInfo->oid;

}


mibObjectTreeNode * travel_MibTree(mibObjectTreeNode *obj, 
    int (*func)(void *argu, mibObjectTreeNode *node), void *arg) {
    int ret;
    mibObjectTreeNode *targetC, *targetS;

    if (isNullPtr(obj) || isNullPtr(func))
        return NULL;


    ret = func(arg, obj);
    if (ret == 1) {
        return obj;
    }

    targetC = travel_MibTree(obj->child, func, arg);
    targetS = travel_MibTree(obj->sibling, func, arg);

    if (targetC != NULL)
        return targetC;
    else
        return targetS;
}

// mibTrees functions
mibTree * mibTreeConstruction(mibTree *tree) {
    if (isNullPtr(tree))
        return NULL;
    
    memset(tree, 0, sizeof(mibTree));
    tree->root = (mibObjectTreeNode *)malloc(sizeof(mibObjectTreeNode));
    mibObjectTreeInit(tree->root);

    return tree;
}

int mibTreeSetRoot(mibTree *tree, mibObjectTreeNode *rootNode) {
    if (isNullPtr(tree) || isNullPtr(rootNode))
        return FALSE;
    tree->root = rootNode;

    return TRUE;
}

mibTree * mibTreeNext(mibTree *tree) {
    if (isNullPtr(tree) || MIBTREE_IS_LAST_TREE(tree))
        return NULL;

    return containerOf(listNodeNext(&tree->node), mibTree, node);
}

mibTree * mibTreePrev(mibTree *tree) {
    if (isNullPtr(tree) || MIBTREE_IS_FIRST_TREE(tree))
        return NULL;

    return containerOf(listNodePrev(&tree->node), mibTree, node);
}

mibTree * mibTreeSearch(mibTree *tree, char *name) {
    mibTree *currentTree;

    if (isNullPtr(tree) || MIBTREE_IS_LAST_TREE(tree)) 
        return NULL;
    
    int match = FALSE;
    currentTree = tree; 

    do {
        match = strncmp(currentTree->rootName, name, strlen(name));
        if (match) {
            break;
        }
    } while (currentTree = mibTreeNext(currentTree));

    return currentTree;
}

int mibTreeAppend(mibTree *head, mibTree *new) {
    if (isNullPtr(head) || isNullPtr(new)) {
        return FALSE; 
    }

    if (listNodeAppend(&head->node, &new->node) == NULL) {
        return FALSE; 
    }
    
    return TRUE;
}

mibTree * mibTreeDelete(mibTree *node) {
    if (isNullPtr(node)) {
        return NULL; 
    }
    if (listNodeDelete(&node->node) == NULL) {
        return NULL; 
    }
    return node;
}

mibTree * mibTreeDeleteByName(mibTree *treeListHead, char *name) {
    mibTree *beDeleted;    

    if (isNullPtr(treeListHead) || isNullPtr(name)) {
        return NULL;
    }
    
    beDeleted = mibTreeSearch(treeListHead, name);
    mibTreeDelete(beDeleted);

    return beDeleted;
}

// Success: return the root of the new tree.
// Failed : return NULL.
mibTree * mibTreeMerge(mibTree *lTree, mibTree *rTree) {
    int ret;
    mibTree *merged;
    
    // Try to merge left tree into right tree.
    ret = insert_MibTree(lTree->root, rTree->root, MIB_OBJ_TREE_NODE_PARENT_NAME(rTree->root));
    if (ret == 0) {
        return lTree;
    } 
    // Try to merge right tree into left tree.
    ret = insert_MibTree(rTree->root, lTree->root, MIB_OBJ_TREE_NODE_PARENT_NAME(lTree->root));
    if (ret == 0) {
        return rTree;
    }
    return NULL;
}

/* mibTreeHead functions */
int mibTreeHeadInit(mibTreeHead *treeHead) {
    if (isNullPtr(treeHead)) 
        return FALSE;

    treeHead->last = NULL;
    treeHead->numOfTree = 0; 
    
    if (mibTreeConstruction(&treeHead->trees) == NULL) {
        printf("mibTreeHeadInit failed\n");  
        exit(1);
    }

    return TRUE;
}

/* Desc: Try to merge last tree into another tree
 * or merge another tree into last tree.
 *
 * Note: if merge failed you should terminate whole
 * program cause the mibTree list is broken.
 */
int mibTreeHeadMerge_LAST(mibTreeHead *treeHead) {
    mibTree *last, *current, *newTree, *current_tmp;

    if (isNullPtr(treeHead)) {
        return FALSE;
    }
    
    last = treeHead->last;
    current = &treeHead->trees;

    do {
        if (current == last) {
            continue;
        }
        
        newTree = mibTreeMerge(current, last);
        // Merge success
        // After that we should remove the two tree
        // and place the new one into the mibTree list.
        if (!isNullPtr(newTree)) {
            current_tmp = mibTreeNext(current);

            current = mibTreeDelete(current);
            last = mibTreeDelete(last);

            if (isNullPtr(current) || isNullPtr(last)) {
                return FALSE; 
            }
            
            mibTreeAppend(&treeHead->trees, newTree);            

            last = newTree;
            current = current_tmp;
        }
    } while (current = mibTreeNext(current));
    
    return TRUE;
}

/* Desc: Try to merge each tree of the list 
 * to let the number of tree in the tree list 
 * as small as possible.
 */
inline static mibTree * currentUpdate(mibTree *current, mibTree *currentMerge, mibTree *iterMerge) {
    while (current = mibTreeNext(current)) {
        if (current != currentMerge && current != iterMerge) {
            break; 
        }
    }
    return current;
}

int mibTreeHeadMerge(mibTreeHead *treeHead) {
    int skip, numOfTree;
    mibTree *current, *current_merge, 
            *current_merge_tmp, *newTree,
            *iterMerge, *iterMerge_tmp;

    if (isNullPtr(treeHead)) 
        return FALSE;
   
    numOfTree = treeHead->numOfTree;

    for (current = &treeHead->trees; ! isNullPtr(current); current = mibTreeNext(current)) {
        current_merge = current;
        iterMerge = mibTreeNext(current);
        
        skip = isNullPtr(current_merge->root) || MIBTREE_IS_LAST_TREE(current_merge);
        if (skip) continue;

        do { 
            newTree = mibTreeMerge(current_merge, iterMerge);   
            if (!isNullPtr(newTree)) {
                // current update 
                current = currentUpdate(current, current_merge, iterMerge);  
                iterMerge_tmp = mibTreeNext(iterMerge);
                
                if (current_merge->node.prev != NULL || current_merge->node.next != NULL)
                    numOfTree -= 1; 

                mibTreeDelete(current_merge);
                mibTreeDelete(iterMerge);
                numOfTree -= 1; 

                current_merge = newTree;   
            }
        } while (iterMerge = iterMerge_tmp); 
        
        mibTreeAppend(&treeHead->trees, newTree);
        numOfTree += 1;
    } 
    
    treeHead->numOfTree = numOfTree;
    return TRUE;
}

int mibTreeHeadAppend(mibTreeHead *treeHead, mibObjectTreeNode *newNode) {
    char *parent;
    mibTree *treeIter, *last;
    mibObjectTreeNode *treeRoot; 

    if (isNullPtr(treeHead) || isNullPtr(newNode)) {
        return FALSE; 
    }
    
    parent = newNode->mergeInfo.parent;

    // A new root.
    if (isNullPtr(parent)) {
        goto NEW_TREE; 
    }

    // First check last tree. 
    treeIter = treeHead->last; 
    if (isNullPtr(treeIter))
        goto ITERATE_OVER_ALL;

LAST_TREE:
    treeRoot = treeIter->root;
    
    if (insert_MibTree(treeRoot, newNode, parent) != -1) {
        return TRUE; 
    }

ITERATE_OVER_ALL: 
    // Iterate over all another trees.
    last = treeHead->last;    
    treeIter = &treeHead->trees;
    
    do {
        if (insert_MibTree(treeIter->root, newNode, parent) != -1) {
            treeHead->last = treeIter;
            return TRUE;
        }
    } while (treeIter = mibTreeNext(treeIter));

NEW_TREE:
    // Build a new tree. 
    treeIter = (mibTree *)malloc(sizeof(mibTree));
    mibTreeConstruction(treeIter); 
    mibTreeSetRoot(treeIter, newNode);
    
    mibTreeAppend(&treeHead->trees, treeIter);
    treeHead->numOfTree++;

    return TRUE;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

typedef struct {
    int idx;
    char *order;
} order;

static int mibTreeMergeAssert(void *arg, mibObjectTreeNode *node) {
    int idx;
    order *pOrder = arg;
    
    idx = pOrder->idx; 
    if (pOrder->order[idx] != node->identifier[0])
        fail();

    ++idx;
    pOrder->idx = idx;
}

void mibTreeObjTree__MIBTREE_MERGE(void **state) {
    // Merge testing
    mibObjectTreeNode *node;
    mibTreeHead treeHead;
    mibTree *currentTree; 

    memset(&treeHead, 0, sizeof(mibTreeHead));
    
    node = mibNodeBuild("A", "1", NULL); 
    mibTreeHeadAppend(&treeHead, node);
    
    node = mibNodeBuild("B", "2", "A");
    mibTreeHeadAppend(&treeHead, node); 
    
    node = mibNodeBuild("F", "1", "C");
    mibTreeHeadAppend(&treeHead, node);
    
    node = mibNodeBuild("E", "2", "C"); 
    mibTreeHeadAppend(&treeHead, node);

    node = mibNodeBuild("C", "3", "A");
    mibTreeHeadAppend(&treeHead, node);
     
    node = mibNodeBuild("G", "2", "B");
    mibTreeHeadAppend(&treeHead, node);
    
    node = mibNodeBuild("D", "1", "B");
    mibTreeHeadAppend(&treeHead, node);

    assert_int_equal(treeHead.numOfTree, 3);

    mibTreeHeadMerge(&treeHead);

    assert_int_equal(treeHead.numOfTree, 1);
    
    char nodeOrder[7] = {'A', 'B', 'G', 'D', 'C', 'F', 'E'};
    order orderDeck;

    orderDeck.idx = 0;
    orderDeck.order = nodeOrder;
    
    currentTree = mibTreeNext(&treeHead.trees);
    travel_MibTree(currentTree->root, mibTreeMergeAssert, &orderDeck);
}

#endif /* MIB2DOC_UNIT_TESTING */

/* mibTreeObjTree.c */

