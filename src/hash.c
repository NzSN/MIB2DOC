/* hash.c */

#include "type.h"
#include "hash.h"
#include "string.h"

static hashChain * hashChainConstruct(pair_kv pair);
static hashChain * hashChainSearch(hashChain *chain, pair_kv pair);
static int hashChainAppend(hashChain *chain, pair_kv pair);

hashMap * hashMapConstruct(int size, hashFunction func) {
    hashMap *mem;
    
    if (isNullPtr(func) || size <= 0) {
        return null; 
    }
    
    mem = (hashMap *)malloc(sizeof(hashMap));
    memset(mem, 0, size);

    mem->size = size;
    mem->hashFunc = func;
    mem->space = (hashElem *)malloc(size * sizeof(hashElem));
    memset(mem->space, 0, size * sizeof(hashElem));

    return mem;
}

int hashMapRelease(hashMap *map) {

}

void * hashMapGet(hashMap *map, pair_kv pair) {
    if (isNullPtr(map)) {
        return NULL; 
    }
    
    int hashValue = map->hashFunc(PAIR_KEY(pair)) % map->size; 
    hashElem *pElem = map->space + hashValue;
    
    if (HASH_ELEM_IS_COLLIDE(pElem)) {
        return hashChainSearch(pElem->chain, pair);             
    }
    return PAIR_VAL(pElem->pair);
}

int hashMapPut(hashMap *map, pair_kv pair) {
    if (isNullPtr(map)) {
        return FALSE; 
    }

    int hashValue = map->hashFunc(PAIR_KEY(pair)) % map->size; 
    hashElem *pElem = map->space + hashValue;

    if (HASH_ELEM_IS_COLLIDE(pElem)) {
        return hashChainAppend(pElem->chain, pair);        
    }
    PAIR_VAL_SET(pElem->pair, PAIR_VAL(pair));
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
    if (isNullPtr(chain))
        return NULL;
    return containerOf(listNodeNext(&chain->node), hashChain, node);
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
        
        if (selectedKey->isEqual(selectedKey, beChecked)) {
            break;
        } else {
            // Continue to check next chian node.     
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

try_key * tryKeyConstruct(int key) {
    try_key *pK = (try_key *)malloc(sizeof(try_key));
    pK->key = key;
    pK->base.isEqual = tryKeyEqual;

    return pK;
}

try_val * tryValConstruct(int val) {
   try_val *pV = (try_val *)malloc(sizeof(try_val)); 
   pV->val = val;
   pV->base.isEqual = tryValEqual;

   return pV;
}

void hashTesting(void **state) {
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
    PAIR_KEY_SET(p2, tK2);
    PAIR_VAL_SET(p2, tP2);
    hashChainAppend(pChain, p2);
    found = hashChainSearch(pChain, p2);
    if (isNullPtr(found)) {
        fail(); 
    }  
    tFoundKey = found->pair.key;
    assert_int_equal(tFoundKey->key, 2);    

    // HashMap testing
    int simpleHashing(pair_val_base *key) {
        try_key *tK = (try_key *)key;    
        int keyVal = tK->key;

        return (keyVal << 5) + key;
    }

    hashMap *pMap = hashMapConstruct(10, simpleHashing);  
    // Situation: No collide 
    hashMapGet(pMap, p1);

    // Situation: collide
}



#endif /* MIB2DOC_UNIT_TESTING */


