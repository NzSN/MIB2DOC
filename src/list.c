/* list.c 
 * 
 * Todo: (1) refactoring list module to provide a generic linear structure
 *       (2) Type name of some type define in this module is so specific such as
 *           the type dispatchParam the name of type hurt generic of it, cause
 *           a it contain a <void *> pointer inside its structure it can be more
 *           generic than present.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "list.h"
#include "type.h"
#include "dispatcher.h"
#include "symbolTbl.h"
#include "typeTable.h"
#include "options.h"

enum {
    SYM_TRAVEL_CONTINUE = 10,
    SYM_TRAVEL_STOP = 11
};

/***********
 * List    *
 ***********/
list * listConst(void) {
    list *l = (list *)zMalloc(sizeof(list));
    return l;
}

static _Status listReleaseHelper(listNode *head, void *arg) {
    list *l = arg;
    l->release(containerOf(head, listEntry, node));
    return OK;
}
void listRelease(list *l) {
    listIter iter;
    listEntry *current, *next;

    if (isNullPtr(l))
        return /* void */;

    if (l->entry)
        listNodeTravel(&l->entry->node, listReleaseHelper, (void *)l);

    return /* void */;
}

static _Status listCopyHelper(listNode *head, void *arg) {
    list *l_copy = arg;
    listEntry *current = containerOf(head, listEntry, node);
    listAppend(l_copy, l_copy->copy(current->element));
    return OK;
}
list * listCopy(list *l) {
    if (isNullPtr(l))
        return NULL;

    list *l_copy = (list *)zMalloc(sizeof(list));

    listSetSize(l_copy, 0);

    listSetCopyMethod(l_copy, l->copy);
    listSetReleaseMethod(l_copy, l->release);
    listSetEqualMethod(l_copy, l->isEqual);

    if (l->entry)
        listNodeTravel(&l->entry->node, listCopyHelper, (void *)l_copy);

    return l_copy;
}

int listAssignment(list *l, list *r) {
    listEntry entry = { 0 };

    if (isNullPtr(l) || isNullPtr(r))
        return ERROR;

    listSetSize(l, 0);

    listSetCopyMethod(l, r->copy);
    listSetReleaseMethod(l, r->release);
    listSetEqualMethod(l, r->isEqual);

    // Release entries of list l
    if (l->entry) listNodeTravel(&l->entry->node, listReleaseHelper, (void *)l);
    // Copy entries into list l
    listNodeTravel(&r->entry->node, listCopyHelper, (void *)l);

    return OK;
}

_Bool listEqualHelper(const listNode *node_l, const listNode *node_r, void *arg) {
    list *l = arg;
    listEntry *entry_l = containerOf(node_l, listEntry, node);
    listEntry *entry_r = containerOf(node_r, listEntry, node);

    return l->isEqual(entry_l->element, entry_r->element);
}
int listIsEqual(list *l, list *r) {
    if (isNullPtr(l) || isNullPtr(r))
        return false;

    if (l->size != r->size)
        return false;

    return listNodeIsEqual_extra(&l->entry->node, &r->entry->node, (void *)l, listEqualHelper);
}

int listConcate(list *l, list *r) {
    if (isNullPtr(l) || isNullPtr(r))
        return ERROR;

    // Append first entry of r onto last entry of l
    listNodeAppend(&l->entry->node, &r->entry->node);
    l->size += r->size;

    return OK;
}

int listAppend(list *l, void *elem) {
    if (isNullPtr(l) || isNullPtr(elem))
        return ERROR;

    listEntry *newEntry = (listEntry *)zMalloc(sizeof(listEntry));
    newEntry->element = elem;

    if (l->entry == null) {
        l->entry = newEntry;
        ++l->size;
        return OK;
    }

    listNodeAppend(&l->entry->node, &newEntry->node);
    ++l->size;

    return OK;
}

void * listRetriveTail(list *l) {
    if (isNullPtr(l) || isNullPtr(l->entry))
        return NULL;

    listNode *tail = listNodeTail(&l->entry->node);
    listEntry *lastEntry = containerOf(tail, listEntry, node);
    void *elem = lastEntry->element;

    if (--l->size == 1)
        l->entry = NULL;

    listNodeDelete(tail);
    RELEASE_MEM(lastEntry);

    return elem;
}

int listPush(list *l, void *elem) {
    if (isNullPtr(l) || isNullPtr(elem))
        return ERROR;

    listEntry *newEntry = (listEntry *)zMalloc(sizeof(listEntry));
    newEntry->element = elem;

    if (l->entry == NULL) {
        ++l->size;
        l->entry = newEntry;
        return OK;
    }

    listNodeAppend(&newEntry->node, &l->entry->node);
    l->entry = newEntry;

    ++l->size;

    return OK;
}

void * listPop(list *l) {
    if (isNullPtr(l) || l->size == 0)
        return NULL;

    listEntry *firstEntry = l->entry;
    void *elem = firstEntry->element;

    l->entry = containerOf(firstEntry->node.next, listEntry, node);

    if (--l->size == 0)
        l->entry = NULL;

    listNodeDelete(firstEntry);
    RELEASE_MEM(firstEntry);

    return elem;
}

listIter listGetIter(list *l) {
    listIter iter = { 0 };

    if (isNullPtr(l))
        return iter;

    iter.entry = l->entry;
    iter.l = l;

    return iter;
}

listIter listPredecessor(listIter li) {
    listIter iter;

    iter.l = li.l;
    if (li.entry && li.entry->node.prev)
        iter.entry = containerOf(li.entry->node.next, listEntry, node);

    return iter;
}

listIter listSuccessor(listIter li) {
    listIter iter;

    iter.l = li.l;
    if (li.entry && li.entry->node.prev)
        iter.entry = containerOf(li.entry->node.prev, listEntry, node);

    return iter;
}

listEntry * listPrev(listIter *li) {
    if (isNullPtr(li)) return NULL;

    listEntry *current = li->entry;
    if (current)
        if (current->node.prev)
            li->entry = containerOf(current->node.prev, listEntry, node);
        else
            li->entry = NULL;

    return current;
}

listEntry * listNext(listIter *li) {
    if (isNullPtr(li)) return NULL;

    listEntry *current = li->entry;
    if (current)
        if (current->node.next)
            li->entry = containerOf(current->node.next, listEntry, node);
        else
            li->entry = NULL;

    return current;
}

listEntry * listSource(listIter li) {
    return li.entry;
}

int listSink(listIter li_l, listIter li_r) {
    if (li_l.entry && li_r.entry) {
        li_l.l->release(li_l.entry->element);
        li_l.entry->element = li_l.l->copy(li_r.entry->element);
    }
    return OK;
}

/************
 * ListNode *
 ************/
listNode *listNodeNext(const listNode *sliNode) {
    if (isNullPtr(sliNode) || isNullPtr(sliNode->next))
        return NULL;
    return sliNode->next;
}

listNode * listNodePrev(const listNode *sliNode) {
    if (isNullPtr(sliNode) || isNullPtr(sliNode->prev))
        return NULL;
    return sliNode->prev;
}

listNode * listNodeInsert(listNode *head, listNode *sliNode) {
    listNode *middle;

    if (isNullPtr(head) || isNullPtr(sliNode)) {
        return NULL;
    }
    if (head->next == NULL) {
        head->next = sliNode;
        sliNode->prev = head;
    } else {
        middle = head->next;
        head->next = sliNode;
        sliNode->next = middle;
    }
    return sliNode;
}

listNode * listNodeDelete(listNode *sliNode) {
    if (isNullPtr(sliNode)) {
        return NULL;
    }

    if (sliNode->prev != NULL && sliNode->next != NULL) {
        /* Middle */
        sliNode->prev->next = sliNode->next;
        sliNode->next->prev = sliNode->prev;
    } else if(sliNode->prev != NULL) {
        /* Tail */
        sliNode->prev->next = NULL;
    } else if (sliNode->next != NULL) {
        /* Head */
        sliNode->next->prev = NULL;
    } else {
        /* Single sliNode */
    }

    sliNode->next = NULL;
    sliNode->prev = NULL;

    return sliNode;
}

listNode * listNodeTail(const listNode *head) {
    if (isNullPtr(head)) {
        return NULL;
    }
    while (head->next) {
        head = head->next;
    }
    return (listNode *)head;
}

listNode * listNodeAppend(listNode *listH, listNode *listN) {
    if (isNullPtr(listH) || isNullPtr(listN)) {
        return NULL;
    }
    listH = listNodeTail(listH);
    listNodeInsert(listH, listN);
    return listN;
}

bool listNodeIsEmpty(listNode *head) {
    int i = 0;

    if (isNullPtr(head)) {
        return FALSE;
    }
    if (head->next != NULL) {
        return TRUE;
    } else {
        return FALSE;
    }
}

listNode * listNodeSearch(listNode *head, listNodeCmp cmpOp, void *arg) {
    if (isNullPtr(head) || isNullPtr(cmpOp) || isNullPtr(arg))
        return NULL;

    listNode *currentNode = head;

    while (isNonNullPtr(currentNode)) {
        if (cmpOp(currentNode, arg) == TRUE)
            break;
        currentNode = listNodeNext(currentNode);
    }

    return currentNode;
}

_Bool listNodeIsEqual(const listNode *first, const listNode *second, listNodeEqualCheck equalCheck) {
    if (isNullPtr(first) || isNullPtr(second) || isNullPtr(equalCheck))
        return ERROR;

    _Bool isEqual;
    const listNode *current_first = first;
    const listNode *current_second = second;

    while (isNonNullPtr(current_first)) {
        isEqual = equalCheck(current_first, current_second);
        if (!isEqual) return FALSE;

        current_first = listNodeNext(current_first);
        current_second = listNodeNext(current_second);
    }

    return TRUE;
}

_Bool listNodeIsEqual_extra(const listNode *first, const listNode *second,
                            void *arg, listNodeEqualCheck_extra equalCheck) {

    if (isNullPtr(first) || isNullPtr(second) || isNullPtr(equalCheck))
        return ERROR;

    _Bool isEqual;
    const listNode *current_first = first;
    const listNode *current_second = second;

    while (isNonNullPtr(current_first) && isNonNullPtr(current_second)) {
        isEqual = equalCheck(current_first, current_second, arg);
        if (!isEqual) return FALSE;

        current_first = listNodeNext(current_first);
        current_second = listNodeNext(current_second);
    }
    return TRUE;
}

_Status listNodeTravel(listNode *head, listNodeTask func, void *arg) {
    listNode *next;

    if (isNullPtr(head) || isNullPtr(func)) {
        return ERROR_NULL_REF;
    }

    next = listNodeNext(head);

    while (head != NULL) {
        func(head, arg);

        head = next;
        next = listNodeNext(next);
    }
    return ERROR_NONE;
}

/*******************************************
 *  Element list Operation function define *
 *******************************************/
slice * sliceConstruct(int sliKey, char *sliVal) {
    slice *sli = (slice *)Malloc(sizeof(slice));

    sli->sliKey = sliKey;
    sli->sliVal = sliVal;
    sli->sliNode.next = NULL;
    sli->sliNode.prev = NULL;
    return sli;
}

bool sliceDestruct(slice *sli) {
    if (isNullPtr(sli)) {
        return FALSE;
    }
    if (!isNullPtr(sli->sliVal)) {
        RELEASE_MEM(sli->sliVal);
    }
    RELEASE_MEM(sli);
    return TRUE;
}

slice * sliceGet(slice *sliHead, int sliKey) {
    if (isNullPtr(sliHead)) {
        mib2docError = ERROR_NULL_REF;
        return NULL;
    }

    sliceIter iter = sliceGetIter(sliHead);
    slice *current = sliceNext(&iter);

    while (current != NULL) {
        if (current->sliKey == sliKey)
            break;
        current = sliceNext(&iter);
    }
    return current;
}

char * sliceGetVal(slice *sliHead, int sliKey) {
    slice *sli;

    if (sli = sliceGet(sliHead, sliKey))
        return sli->sliVal;
    return NULL;
}

char *sliceRetriVal(slice *sliHead, int sliKey) {
    slice *sli;
    char *val;

    if (sli = sliceGet(sliHead, sliKey)) {
        val = sli->sliVal;
        sli->sliVal = NULL;
    }
    return val;
}

int sliceStore(slice *sliHead, slice *newSli) {
    if (isNullPtr(sliHead) || isNullPtr(newSli)) {
        return ERROR_NULL_REF;
    }

    if (sliceGet(sliHead, newSli->sliKey)) {
        return ERROR_NONE;
    }

    return sliceStore_without_key(sliHead, newSli);
}

int sliceStore_without_key(slice *sliHead, slice *newSli) {
    if (isNullPtr(sliHead) || isNullPtr(newSli))
        return ERROR;

    sliceIter iter = sliceGetIter(sliHead);
    while (sliHead != NULL) {
        if (IS_LAST_SLICE_NODE(sliHead)) {
            sliHead->sliNode.next = &newSli->sliNode;
            newSli->sliNode.prev = &sliHead->sliNode;
            return OK;
        }
        sliHead = sliceNext(&iter);
    }
    return OK;
}

int sliceAssignment(slice *sl_l, slice *sl_r) {
    if (isNullPtr(sl_l) || isNullPtr(sl_r))
        return ERROR;

    // Todo: Shallow copy is dangerous fix it.
    sl_l->sliKey = sl_r->sliKey;
    sl_l->sliVal = sl_r->sliVal;

    return OK;
}

bool sliceRelease(slice *sli) {
    slice *pSli;
    slice *pSli_next;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    for (pSli = sliceNext(&iter); pSli != NULL; pSli = pSli_next) {
        pSli_next = sliceNext(&iter);
        sliceDestruct(pSli);
    }
    return TRUE;
}

bool sliceRelease_STATIC(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    iter = sliceSuccessor(iter);

    sliceRelease(sliceSource(iter));

    sli->sliNode.next = NULL;
    sli->sliNode.prev = NULL;

    return TRUE;
}

bool sliceReset(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    while (sli != NULL) {
        pSli = sli;
        sli = sliceNext(&iter);
        RELEASE_MEM(pSli);
    }
    return TRUE;
}

bool sliceReset_STATIC(slice *sli) {
    slice *pSli;

    if (isNullPtr(sli))
        return FALSE;

    sliceIter iter = sliceGetIter(sli);
    if (sliceReset(sliceNext(&iter))) {
        memset(&sli->sliNode, 0, sizeof(listNode));
    }
    return TRUE;
}

sliceIter sliceGetIter(slice *sl) {
    sliceIter si = {
        .sl = sl,
        .node = &sl->sliNode
    };
    return si;
}

sliceIter sliceSuccessor(sliceIter si) {
    si.node = listNodeNext(si.node);
    return si;
}

sliceIter slicePredecessor(sliceIter si) {
    si.node = listNodePrev(si.node);
    return si;
}

slice * sliceNext(sliceIter *si) {
    if (isNullPtr(si) || isNullPtr(si->node))
        return null;

    listNode *current = si->node;
    si->node = listNodeNext(current);
    return containerOf(current, slice, sliNode);
}

slice * slicePrev(sliceIter *si) {
    if (isNullPtr(si) || isNullPtr(si->node->prev))
        return null;
    si->node = listNodePrev(si->node);
    return containerOf(si->node, slice, sliNode);
}

slice * sliceSource(sliceIter si) {
    if (isNullPtr(si.node)) return NULL;
    return containerOf(si.node, slice, sliNode);
}

int sliceSink(sliceIter si_l, sliceIter si_r) {
    if (isNullPtr(si_l.node) || isNullPtr(si_r.node))
        return ERROR;

    slice *sl_l = sliceSource(si_l);
    slice *sl_r = sliceSource(si_r);
    sliceAssignment(sl_l, sl_r);

    return OK;
}


#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void insert_range_to_list(list *l, int from, int to) {
    while (from <= to) {
        listAppend(l, numberToStr(from++));
    }
}

listEntry * peek_next_list(listIter *iter) {
    return listNext(iter);
}

void listRelease_Test(void *elem) {
    free(elem);
}

void * listCopy_Test(void *elem) {
    char *orig = (char *)elem;
    char *copy;

    copy = (char *)zMalloc(strlen(orig));
    strncpy(copy, orig, strlen(orig));

    return copy;
}

int listIsEqual_Test(void *l, void *r) {
    char *l_c = (char *)l;
    char *r_c = (char *)r;
    return TRUE;
}

void list__Generic_List(void **state) {
    int num = 0;
    listEntry *entry;
    list *l = listConst();

    listSetReleaseMethod(l, listRelease_Test);
    listSetCopyMethod(l, listCopy_Test);
    listSetEqualMethod(l, listIsEqual_Test);

    // Insert 1000 element into list.
    insert_range_to_list(l, 0, 1000);
    // Peek into entries and check is entries valid
    listIter iter = listGetIter(l);
    while (entry = peek_next_list(&iter)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // Copy validity
    num = 0;
    list *l_copy = listCopy(l);
    listIter iter_copy = listGetIter(l_copy);

    entry = peek_next_list(&iter_copy);
    assert_non_null(entry);
    assert_string_equal(entry->element, numberToStr(num++));

    while (entry = peek_next_list(&iter_copy)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // Assignment checking
    num = 0;
    list *l_assign = listConst();

    listAssignment(l_assign, l);

    listIter iter_assign = listGetIter(l_assign);

    while (entry = peek_next_list(&iter_assign)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // Equal checking
    assert_int_equal(listIsEqual(l_assign, l_copy), TRUE);
    assert_int_equal(listIsEqual(l_assign, l), TRUE);
    assert_int_equal(listIsEqual(l, l_copy), TRUE);
    assert_int_equal(listIsEqual(l, l), TRUE);

    // Should be equal after insert equal value into both list
    listAppend(l, "YY");
    listAppend(l_copy, "YY");
    assert_int_equal(listIsEqual(l, l_copy), TRUE);

    // Should be false in this case
    listAppend(l, "TT");
    assert_int_equal(listIsEqual(l, l_copy), FALSE);

    // List concate operation
    list *l_concate = listConst();
    list *r_concate = listConst();

    insert_range_to_list(l_concate, 0, 4);
    insert_range_to_list(r_concate, 5, 9);

    listConcate(l_concate, r_concate);
    iter = listGetIter(l_concate);

    assert_int_equal(l_concate->size, 10);

    num = 0;
    while (entry = peek_next_list(&iter)) {
        assert_string_equal(entry->element, numberToStr(num++));
    }

    // listRetriveTail testing
    num = 9;
    char *elem;
    while (elem = listRetriveTail(l_concate)) {
        assert_string_equal(elem, numberToStr(num));
        assert_int_equal(l_concate->size, num--);
    }

    // listPush and listPop testing
    list *l_push = listConst();
    listPush(l_push, numberToStr(1));
    listPush(l_push, numberToStr(2));
    assert_string_equal(listPop(l_push), numberToStr(2));
    assert_int_equal(l_push->size, 1);
    assert_string_equal(listPop(l_push), numberToStr(1));
    assert_int_equal(l_push->size, 0);
}

void list_symbolTable(void **state) {
    symbol_t *symbol, *found;
    symbolTable symTbl;

    symbolTableInit(&symTbl);

    symbol = symbolNodeConst("B", "A", "1");
    symbolTableAdd(&symTbl, symbol);

    found = symbolTableSearch(&symTbl, "B");
    assert_non_null(found);

    assert_string_equal(found->symIdent, "B");
}

void list__TYPE_TABLE(void **state) {
    _Bool isExists;
    typeTable *tbl = typeTableConst();

    typeTableAdd(tbl, strdup("INTEGER"), CATE_BUILD_IN, NULL);
    isExists = typeTableIsTypeExists(tbl, "INTEGER");

    assert_int_equal(isExists, TRUE);

    typeTableDel(tbl, "INTEGER");
    isExists = typeTableIsTypeExists(tbl, "INTEGER");
    assert_int_equal(isExists, FALSE);

    _Bool isEqual;
    typeTable *tbl_ = typeTableConst();

    typeTableAdd(tbl, strdup("INTEGER"), CATE_BUILD_IN, NULL);
    typeTableAdd(tbl_, strdup("INTEGER"), CATE_BUILD_IN, NULL);
    isEqual = typeTableIsEqual(tbl, tbl_);
    assert_int_equal(isEqual, TRUE);

    int idx = 0;
    while (idx < 100) {
        typeTableAdd(tbl, numberToStr(idx), CATE_BUILD_IN, NULL);
        ++idx;
    }

    idx = 0;
    while (idx < 100) {
        isExists = typeTableIsTypeExists(tbl, numberToStr(idx));
        assert_int_equal(isExists, TRUE);
        ++idx;
    }
}

#endif /* MIB2DOC_UNIT_TESTING */

/* end of list.c */

