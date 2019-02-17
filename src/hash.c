/* hash.c */

#include "type.h"
#include "hash.h"
#include "string.h"

static hashChain * hashChainConstruct(pair_kv pair);
static hashChain * hashChainSearch(hashChain *chain, pair_kv pair);
static int hashChainAppend(hashChain *chain, pair_kv pair);
static int hashChainRelease(hashChain *chainNode);
static int hashChainRelease_STATIC(hashChain *chainNode);
static int hashChainDelete(hashChain *chain, pair_kv pair);
static int pairRelease(pair_kv *p);

hashMap * hashMapConstruct(int size, hashFunction func) {
    hashMap *mem;
    
    if (isNullPtr(func) || size <= 0) {
        return null; 
    }
    
    mem = (hashMap *)malloc(sizeof(hashMap));
    memset(mem, 0, sizeof(hashMap));

    mem->size = size;
    mem->hashFunc = func;
    mem->space = (hashElem *)malloc(size * sizeof(hashElem));
    memset(mem->space, 0, size * sizeof(hashElem));

    return mem;
}

int hashMapRelease(hashMap *map) {
    hashElem *selectedElem;
    int index = 0, size = map->size;  
    
    if (isNullPtr(map)) {
        return FALSE; 
    }

    // Chain release.
    while (index < size) {
        selectedElem = HASH_MAP_ELEM_SELECT(map, index);
        hashChainRelease_STATIC(&selectedElem->chain);
        ++index; 
    } 

    // Element release.
    RELEASE_MEM(map->space);
    // Map release.
    RELEASE_MEM(map);

    return TRUE;
}

void * hashMapGet(hashMap *map, pair_kv pair) {
    hashChain *chainBeFound;
    pair_key_base *beChecked, *selectedKey;

    if (isNullPtr(map)) {
        return NULL; 
    }
     
    hashElem *pElem = HASH_MAP_HASH_GET(map, PAIR_KEY(pair));
    
    beChecked = PAIR_KEY(pair); 

    if (HASH_ELEM_IS_COLLIDE(pElem)) {
        selectedKey = PAIR_KEY(pElem->pair);

        if (isNullPtr(selectedKey)) {
            return NULL;
        }
        if (selectedKey->isEqual(selectedKey, beChecked)) { 
            return PAIR_VAL(pElem->pair); 
        }
        chainBeFound = hashChainSearch(HASH_ELEM_CHAIN_REF(pElem), pair);             
        if (isNullPtr(chainBeFound)) 
            return NULL;
        return PAIR_VAL(chainBeFound->pair);
    }
    return PAIR_VAL(pElem->pair);
}

int hashMapPut(hashMap *map, pair_kv pair) {
    if (isNullPtr(map)) {
        return FALSE; 
    }

    hashElem *pElem = HASH_MAP_HASH_GET(map, PAIR_KEY(pair)); 

    if (HASH_ELEM_IS_USED(pElem)) {
        int match;
        pair_key_base *key = PAIR_KEY(pElem->pair);

        // First I need to check is the key exists.
        match = key->isEqual(key, PAIR_KEY(pair)); 
        if (match || hashChainSearch(HASH_ELEM_CHAIN_REF(pElem), pair)) {
            // The key is already exist in the map. 
            return FALSE;
        }

        pElem->collide = HASH_ELEM_COLLIDE;  
        return hashChainAppend(HASH_ELEM_CHAIN_REF(pElem), pair);        
    }

    pElem->used = HASH_ELEM_USED;

    PAIR_KEY_SET(pElem->pair, PAIR_KEY(pair));
    PAIR_VAL_SET(pElem->pair, PAIR_VAL(pair));

    return TRUE;
}

int hashMapDelete(hashMap *map, pair_kv pair) {
    hashElem *pElem;
    hashChain *pChain;
    pair_key_base *pKey = PAIR_KEY(pair);    
    
    if (isNullPtr(map) || isNullPtr(pKey)) 
        return FALSE;
    
    pElem = HASH_MAP_HASH_GET(map, PAIR_KEY(pair));

    // Try to matching with the key outside of chain.
    if (pKey->isEqual(pKey, PAIR_KEY(pElem->pair))) {
        PAIR_KEY_SET(pElem->pair, NULL);
        PAIR_VAL_SET(pElem->pair, NULL);
    } else if (hashChainDelete(HASH_ELEM_CHAIN_REF(pElem), pair)) {
       // return true at the end of function block. 
    } else {
        return FALSE; 
    }
    return TRUE;
}

static hashChain * hashChainConstruct(pair_kv pair) {
    hashChain *pChain = (hashChain *)malloc(sizeof(hashChain));

    PAIR_KEY_SET(pChain->pair, PAIR_KEY(pair));
    PAIR_VAL_SET(pChain->pair, PAIR_VAL(pair));
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

static int hashChainDelete(hashChain *chain, pair_kv pair) {
    hashChain *found;
    if (isNullPtr(chain))
        return FALSE;
    found = hashChainSearch(chain, pair); 
    if (found) {
        listNodeDelete(HASH_CHAIN_NODE_R(chain)); 
        RELEASE_MEM(chain);
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

static hashChain * hashChainSearch(hashChain *chainNode, pair_kv pair) {
    pair_key_base *selectedKey, *beChecked;

    if (isNullPtr(chainNode))
        return NULL;
    do {
        selectedKey = PAIR_KEY(chainNode->pair);
        beChecked = PAIR_KEY(pair);
                     
        if (!isNullPtr(selectedKey)) {
            if (selectedKey->isEqual(selectedKey, beChecked)) {
                break;
            } else {
                // Continue to check next chian node.     
            }
        }
    } while(chainNode = hashChainNext(chainNode));

    return chainNode;
}

static int hashChainAppend(hashChain *chainNode, pair_kv pair) {
    hashChain *tailNode, *new = hashChainConstruct(pair);
    
    if (isNullPtr(chainNode)) 
        return FALSE;

    listNodeAppend(&chainNode->node, &new->node);
    return TRUE;
}

static int hashChainRelease(hashChain *chainNode) {
    hashChain *released;    
    
    do {
        // Release pair
        if (pairRelease(HASH_CHAIN_PAIR_R(chainNode)) == FALSE)
            return FALSE;
        released = chainNode;  
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

    do {
        // Release pair
        if (pairRelease(HASH_CHAIN_PAIR_R(chainNode)) == FALSE)
            return FALSE;
        released = chainNode;
        chainNode = hashChainNext(chainNode);
        RELEASE_MEM(released);
    } while(chainNode);

    return TRUE;
}

static int pairRelease(pair_kv *p) {
    int ret;
    pair_key_base *key;
    pair_val_base *val;

    if (isNullPtr(p)) 
        return FALSE; 

    key = PAIR_KEY_R(p);
    val = PAIR_VAL_R(p);
    
    if (!key) {
        ret = key->release(key); 
    }
    if (!val) {
        ret = ret && val->release(key); 
    }
    return ret;
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

try_key * tryKeyConstruct(int key) {
    try_key *pK = (try_key *)malloc(sizeof(try_key));
    pK->key = key;
    pK->base.isEqual = tryKeyEqual;
    pK->base.value = tryKeyValue;
    pK->base.release = tryKeyRelease;
    return pK;
}

try_val * tryValConstruct(int val) {
    try_val *pV = (try_val *)malloc(sizeof(try_val)); 

    pV->val = val;
    pV->base.isEqual = tryValEqual;
    pV->base.value = tryValValue;
    pV->base.release = tryValRelease;
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
    pair_kv p1; 
    PAIR_KEY_SET(p1, tK1);
    PAIR_VAL_SET(p1, tP1);
    
    // Situation: only one node
    hashChain *found, *pChain = hashChainConstruct(p1);
    found = hashChainSearch(pChain, p1); 
    
    try_key *tFoundKey = found->pair.key;
    assert_int_equal(tFoundKey->key, 1);
    
    // Situation: several node
    pair_kv p2;
    try_val *retVal;
    int chain_count = 0;
     
    while (chain_count < 1000) {
        PAIR_KEY_SET(p2, tryKeyConstruct(chain_count));
        PAIR_VAL_SET(p2, tryValConstruct(chain_count));
        hashChainAppend(pChain, p2);
        found = hashChainSearch(pChain, p2);
        retVal = PAIR_VAL(found->pair);
        assert_int_equal(retVal->val, chain_count);

        ++chain_count;
    }
    hashMap *pMap = hashMapConstruct(100000, simpleHashing);  

    // Situation: No collide 
    try_val *tVal = hashMapGet(pMap, p1);
    if (NOT isNullPtr(tVal)) {
        fail(); 
    }
    hashMapPut(pMap, p1);
    tVal = hashMapGet(pMap, p1);
    assert_int_equal(tVal->val, 1);

    // Situation: collide
    pair_kv collidePair;
    PAIR_KEY_SET(collidePair, tryKeyConstruct(1));
    PAIR_VAL_SET(collidePair, tryValConstruct(3));

    // Key unique check.
    if (hashMapPut(pMap, collidePair) != FALSE)
        fail();

    int count = 0;
    pair_kv temp;
    while (count < 1000000) {
        PAIR_KEY_SET(temp, tryKeyConstruct(count));
        PAIR_VAL_SET(temp, tryValConstruct(count));
        hashMapPut(pMap, temp);
        tVal = hashMapGet(pMap, temp);
    
        assert_int_equal(tVal->val, count);
        ++count;
    }
    
    // Release testing
    hashMapRelease(pMap);     
    
    // Hash map deletion testing.
    try_val *valDel; 
    pair_kv pairDel;

    PAIR_KEY_SET(pairDel, tryKeyConstruct(1));
    PAIR_VAL_SET(pairDel, tryValConstruct(1));

    hashMap *mapDelTest = hashMapConstruct(10, simpleHashing);
    hashMapDelete(mapDelTest, pairDel);
    valDel = hashMapGet(mapDelTest, pairDel);
    if (!isNullPtr(valDel))
        fail();
} 
#endif /* MIB2DOC_UNIT_TESTING */


