/* hash.c */

#include "type.h"
#include "hash.h"
#include <string.h>
#include <malloc.h>

static hashChain * hashChainConstruct(pair_key_base *key, pair_val_base *val);
static hashChain * hashChainSearch(hashChain *chain, pair_key_base *key);
static int hashChainAppend(hashChain *chain, hashChain *new);
static int hashChainRelease(hashChain *chainNode);
static int hashChainRelease_STATIC(hashChain *chainNode);
static hashChain * hashChainDup(hashChain *origin);
static int hashChainDelete(hashChain *chain, pair_key_base *key);
static hashChain *hashChainPrev(hashChain *chain);
static hashChain * hashChainNext(hashChain *chain);
static int hashChainInsert(hashChain *chainNode, hashChain *newNode);


hashMap * hashMapInit(hashMap *map, int size, hashFunction func) {
    if (isNullPtr(map) || size < 1 || isNullPtr(func))
        return NULL;
    map->size = size;
    map->hashFunc = func;
    map->space = (hashElem *)malloc(size * sizeof(hashElem));
    memset(map->space, 0, size * sizeof(hashElem));

    return map;
}

hashMap * hashMapConstruct(int size, hashFunction func) {
    hashMap *mem;

    if (isNullPtr(func) || size <= 0) {
        return null;
    }

    mem = (hashMap *)malloc(sizeof(hashMap));

    return hashMapInit(mem, size, func);
}

hashMap * hashMapDup(hashMap *origin) {
    int idx, size;
    hashMap *copy;
    hashChain *copyChain, *copyFrom;
    hashElem *pElem_orig, *pElem_copy;

    if (isNullPtr(origin)) {
        return NULL;
    }

    copy = hashMapConstruct(origin->size, origin->hashFunc);

    memcpy(copy->space, origin->space, origin->size * sizeof(hashElem));

    idx = 0;
    size = origin->size;

    while (idx < size) {
        pElem_copy = HASH_MAP_ELEM_SELECT(copy, idx);
        pElem_orig = HASH_MAP_ELEM_SELECT(origin, idx);

        if (pElem_orig->key)
            pElem_copy->key = pElem_orig->key->copy(pElem_orig->key);
        if (pElem_orig->val)
            pElem_copy->val = pElem_orig->val->copy(pElem_orig->val);

        copyFrom = &(HASH_MAP_ELEM_SELECT(origin, idx)->chain);
        copyFrom = hashChainNext(copyFrom);
        if (isNullPtr(copyFrom)) {
            ++idx;
            continue;
        }
        copyChain = hashChainDup(copyFrom);
        memset(&HASH_MAP_ELEM_SELECT(copy, idx)->chain.node, 0, sizeof(listNode));
        hashChainInsert(&HASH_MAP_ELEM_SELECT(copy, idx)->chain, copyChain);

        ++idx;
    }

    return copy;
}

int hashMapRelease(hashMap *map) {
    int index = 0, size = map->size;
    hashElem *selectedElem;
    pair_key_base *key;
    pair_val_base *val;

    if (isNullPtr(map)) {
        return FALSE;
    }

    // Chain release.
    while (index < size) {
        selectedElem = HASH_MAP_ELEM_SELECT(map, index);
        if (selectedElem->used == 1) {
            key = selectedElem->key;
            val = selectedElem->val;

            if (key && key->release) key->release(key);
            if (val && val->release) val->release(val);

            hashChainRelease_STATIC(&selectedElem->chain);
        }
        ++index;
    }

    // Element release.
    RELEASE_MEM(map->space);
    // Map release.
    RELEASE_MEM(map);

    return TRUE;
}

void * hashMapGet(hashMap *map, pair_key_base *key) {
    hashChain *chainBeFound;
    pair_key_base *beChecked, *selectedKey;

    if (isNullPtr(map) || isNullPtr(key)) {
        return NULL;
    }

    hashElem *pElem = HASH_MAP_HASH_GET(map, key);

    beChecked = key;

    selectedKey = pElem->key;

    if (isNullPtr(selectedKey)) {
        return NULL;
    }
    if (selectedKey->isEqual(selectedKey, beChecked)) {
        return pElem->val;
    }

    if (HASH_ELEM_IS_COLLIDE(pElem)) {
        chainBeFound = hashChainSearch(HASH_ELEM_CHAIN_REF(pElem), key);
        if (isNullPtr(chainBeFound))
            return NULL;
        return chainBeFound->val;
    }
    return null;
}

int hashMapPut(hashMap *map, pair_key_base *key, pair_val_base *val) {
    if (isNullPtr(map)) {
        return FALSE;
    }

    hashElem *pElem = HASH_MAP_HASH_GET(map, key);

    if (HASH_ELEM_IS_USED(pElem)) {
        int match;
        pair_key_base *selectedKey = pElem->key;

        // First I need to check is the key exists.
        match = selectedKey->isEqual(selectedKey, key);
        if (match || hashChainSearch(HASH_ELEM_CHAIN_REF(pElem), key)) {
            // The key is already exist in the map.
            return FALSE;
        }

        pElem->collide = HASH_ELEM_COLLIDE;
        return hashChainAppend(HASH_ELEM_CHAIN_REF(pElem), hashChainConstruct(key, val));
    }

    pElem->used = HASH_ELEM_USED;

    pElem->key = key;
    pElem->val = val;

    return TRUE;
}

int hashMapDelete(hashMap *map, pair_key_base *key) {
    hashElem *pElem;
    hashChain *pChain;
    pair_key_base *theKey;
    pair_val_base *theVal;

    if (isNullPtr(map) || isNullPtr(key))
        return FALSE;

    pElem = HASH_MAP_HASH_GET(map, key);

    // Try to matching with the key outside of chain.
    if (key->isEqual && key->isEqual(key, pElem->key)) {
        theKey = pElem->key;
        theVal = pElem->val;

        theKey->release(theKey);
        theVal->release(theVal);

        pElem->key = NULL;
        pElem->val = NULL;
    } else if (hashChainDelete(HASH_ELEM_CHAIN_REF(pElem), key)) {
        // return true at the end of function block.
    } else {
        return FALSE;
    }
    return TRUE;
}

static hashChain * hashChainConstruct(pair_key_base *key, pair_val_base *val) {
    hashChain *pChain;

    if (isNullPtr(key) || isNullPtr(val))
        return NULL;

    pChain = (hashChain *)malloc(sizeof(hashChain));

    pChain->key = key;
    pChain->val = val;

    memset(&pChain->node, 0, sizeof(listNode));

    return pChain;
}

static hashChain *hashChainPrev(hashChain *chain) {
    if (isNullPtr(chain))
        return NULL;
    return containerOf(listNodePrev(&chain->node), hashChain, node);
}

static hashChain * hashChainNext(hashChain *chain) {
    if (isNullPtr(chain) || HASH_CHAIN_IS_LAST(chain)) {
        return NULL;
    }
    return containerOf(listNodeNext(&chain->node), hashChain, node);
}

static hashChain * hashChainDup(hashChain *origin) {
    hashChain *copy, *current, copyHead;
    pair_key_base *key;
    pair_val_base *val;

    if (isNullPtr(origin))
        return NULL;

    copyHead.key = NULL;
    copyHead.val = NULL;
    memset(&copyHead.node, 0, sizeof(listNode));

    for (current = origin; !isNullPtr(current); current = hashChainNext(current)) {
        key = current->key;
        val = current->val;

        copy = hashChainConstruct(key->copy(key), val->copy(val));
        hashChainAppend(&copyHead, copy);
    }

    copy = hashChainNext(&copyHead);
    copy->node.prev = NULL;

    return copy;
}

static int hashChainDelete(hashChain *chain, pair_key_base *key) {
    hashChain *found;
    if (isNullPtr(chain) || isNullPtr(key))
        return FALSE;
    found = hashChainSearch(chain, key);
    if (found) {
        listNodeDelete(&found->node);
        // fixme: Memory leak may happend cause of the
        // shallow free
        RELEASE_MEM(found);
        return TRUE;
    }
    return FALSE;
}

static hashChain * hashChainTail(hashChain *chainNode) {
    if (isNullPtr(chainNode) || HASH_CHAIN_IS_LAST(chainNode))
        return NULL;
    chainNode = containerOf(listNodeTail(&chainNode->node), hashChain, node);
    return chainNode;
}

static hashChain * hashChainSearch(hashChain *chainNode, pair_key_base *key) {
    pair_key_base *selectedKey, *beChecked;

    if (isNullPtr(chainNode))
        return NULL;
    do {
        selectedKey = chainNode->key;
        beChecked = key;

        if (!isNullPtr(selectedKey) && selectedKey->isEqual) {
            if (selectedKey->isEqual(selectedKey, beChecked)) {
                break;
            } else {
                // Continue to check next chian node.
            }
        }
    } while(chainNode = hashChainNext(chainNode));

    return chainNode;
}

static int hashChainAppend(hashChain *chainNode, hashChain *newNode) {

    if (isNullPtr(chainNode) || isNullPtr(newNode))
        return FALSE;

    listNodeAppend(&chainNode->node, &newNode->node);
    return TRUE;
}

static int hashChainInsert(hashChain *chainNode, hashChain *newNode) {
    if (isNullPtr(chainNode) || isNullPtr(newNode))
        return FALSE;

    listNodeInsert(&chainNode->node, &newNode->node);
    return TRUE;
}

static int hashChainRelease(hashChain *chainNode) {
    hashChain *released;

    do {
        // Release pair
        if (chainNode->key)
            chainNode->key->release(chainNode->key);
        if (chainNode->val)
            chainNode->val->release(chainNode->val);
        released = chainNode;
        chainNode = hashChainNext(chainNode);
        RELEASE_MEM(released);
    } while(chainNode = hashChainNext(chainNode));

    return TRUE;
}

static int hashChainRelease_STATIC(hashChain *chainNode) {
    hashChain *released;

    if (isNullPtr(chainNode) || HASH_CHAIN_IS_LAST(chainNode)) {
        return TRUE;
    }

    chainNode = hashChainNext(chainNode);

    return hashChainRelease(chainNode);
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"
#include <stdio.h>

int hashing(void *key) {
    int iKey = (int)key;
    iKey *= 10;

    return iKey;
}

typedef struct {
    pair_key_base base;
    int key;
} try_key;

typedef struct {
    pair_val_base base;
    int val;
} try_val;



void * tryKeyValue(try_key *key) {
    return (void *)key->key;
}

void * tryValValue(try_val *val) {
    return (void *)val->val;
}

int tryKeyEqual(try_key *lK, try_key *rK) {
    if (isNullPtr(lK) || isNullPtr(rK))
        return FALSE;
    return lK->key == rK->key;
}

int tryValEqual(try_val *lV, try_val *rV) {
    if (isNullPtr(lV) || isNullPtr(rV))
        return FALSE;
    return lV->val == rV->val;
}

int tryKeyRelease(try_key *key) {
    if (isNullPtr(key))
        return FALSE;

    RELEASE_MEM(key);

    return TRUE;
}

int tryValRelease(try_val *val) {
    if (isNullPtr(val))
        return FALSE;

    RELEASE_MEM(val);

    return TRUE;
}


try_key * tryKeyConstruct(int key);
try_val * tryValConstruct(int val);

try_key * tryKeyCopy(try_key *origin) {
    if (isNullPtr(origin))
        return NULL;
    return tryKeyConstruct(origin->key);
}

try_val *tryValCopy(try_val *origin) {
    if (isNullPtr(origin))
        return NULL;
    return tryValConstruct(origin->val);
}

try_key * tryKeyConstruct(int key) {
    try_key *pK = (try_key *)malloc(sizeof(try_key));
    pK->key = key;
    pK->base.isEqual = tryKeyEqual;
    pK->base.value = tryKeyValue;
    pK->base.release = tryKeyRelease;
    pK->base.copy = tryKeyCopy;
    return pK;
}

try_val * tryValConstruct(int val) {
    try_val *pV = (try_val *)malloc(sizeof(try_val));

    pV->val = val;
    pV->base.isEqual = tryValEqual;
    pV->base.value = tryValValue;
    pV->base.release = tryValRelease;
    pV->base.copy = tryValCopy;

    return pV;
}

int simpleHashing(pair_key_base *key) {
    try_key *tK = (try_key *)key;
    int keyVal = tK->key;

    return (keyVal << 5) + keyVal;
}

void hash__HASH_BASIC(void **state) {
    // Key, Value pair testing
    try_key *tK1 = tryKeyConstruct(1);
    try_key *tK2 = tryKeyConstruct(2);

    assert_int_equal(tK1->base.isEqual(tK1, tK2), FALSE);

    try_key *tK3 = tryKeyConstruct(3);
    try_key *tK4 = tryKeyConstruct(3);

    assert_int_equal(tK3->base.isEqual(tK3, tK4), TRUE);

    try_val *tP1 = tryValConstruct(1);
    try_val *tP2 = tryValConstruct(2);

    assert_int_equal(tP1->base.isEqual(tP1, tP2), FALSE);

    try_val *tP3 = tryValConstruct(3);
    try_val *tP4 = tryValConstruct(3);

    assert_int_equal(tP3->base.isEqual(tP3, tP4), TRUE);

    // hashChain Testing
    // Situation: only one node
    hashChain *found, *pChain = hashChainConstruct(tK1, tP1);
    found = hashChainSearch(pChain, tK1);

    try_key *tFoundKey = found->key;
    assert_int_equal(tFoundKey->key, 1);

    // Situation: several node
    try_key *temp_key;
    try_val *temp_val;
    try_val *retVal;
    int chain_count = 0;

    while (chain_count < 1000) {
        temp_key = tryKeyConstruct(chain_count);
        temp_val = tryValConstruct(chain_count);

        hashChainAppend(pChain, hashChainConstruct(temp_key, temp_val));
        found = hashChainSearch(pChain, temp_key);
        retVal = found->val;
        assert_int_equal(retVal->val, chain_count);

        ++chain_count;
    }
    hashMap *pMap = hashMapConstruct(100000, simpleHashing);

    // Situation: No collide
    try_val *tVal = hashMapGet(pMap, tK1);
    if (NOT isNullPtr(tVal)) {
        fail();
    }
    hashMapPut(pMap, tK1, tP1);
    tVal = hashMapGet(pMap, tK1);
    assert_int_equal(tVal->val, 1);

    // Situation: collide
    temp_key = tryKeyConstruct(1);
    temp_val = tryValConstruct(3);

    // Key unique check.
    if (hashMapPut(pMap, temp_key, temp_val) != FALSE)
        fail();

    int count = 0;
    int range = 1000;
    while (count < range) {
        temp_key = tryKeyConstruct(count);
        temp_val = tryValConstruct(count);
        hashMapPut(pMap, temp_key, temp_val);
        tVal = hashMapGet(pMap, temp_key);

        assert_int_equal(tVal->val, count);
        ++count;
    }

    count = 0;
    while (count < range) {
        temp_key = tryKeyConstruct(count);
        temp_val = hashMapGet(pMap, temp_key);
        assert_int_equal(temp_val->val, count);
        ++count;
    }

    // hashMap duplication
    hashMap *pMap_copy = hashMapDup(pMap);

    count = 0;
    while (count < range) {
        temp_key = tryKeyConstruct(count);
        temp_val = hashMapGet(pMap_copy, temp_key);
        assert_int_equal(temp_val->val, count);
        ++count;
    }

    // Release testing
    hashMapRelease(pMap);

    // Hash map deletion testing.
    temp_key = tryKeyConstruct(1);
    temp_val = tryValConstruct(1);
    try_val *valDel;

    hashMap *mapDelTest = hashMapConstruct(10, simpleHashing);
    hashMapDelete(mapDelTest, temp_key);
    valDel = hashMapGet(mapDelTest, temp_key);
    if (!isNullPtr(valDel))
        fail();
}
#endif /* MIB2DOC_UNIT_TESTING */
