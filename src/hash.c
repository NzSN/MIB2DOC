/* hash.c */

#include "type.h"
#include "hash.h"
#include "string.h"

static hashChain * hashChainConstruct(pair_kv pair);
static void * hashChainSearch(hashChain *chain, pair_kv pair);
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

static void * hashChainSearch(hashChain *chain, pair_kv pair) {
           
}

static int hashChainAppend(hashChain *chain, pair_kv pair) {

}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"
#include <stdio.h>

int hashing(void *key) {
    int iKey = (int)key;
    iKey *= 10;
    
    return iKey;
}

void hashTesting(void **state) {
    pair_kv pair;
    int value;
    PAIR_KEY_SET(pair, 1);
    PAIR_VAL_SET(pair, 3);
    hashMap *pMap = hashMapConstruct(10, hashing);
    if (isNullPtr(pMap))
        fail();
    hashMapPut(pMap, pair);
    value = (int)hashMapGet(pMap, pair);

    assert_int_equal(value, 3);
}

#endif /* MIB2DOC_UNIT_TESTING */


