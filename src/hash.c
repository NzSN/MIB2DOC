/* hash.c */

#include "type.h"
#include "hash.h"
#include "string.h"

static hashChain * hashChainConstruct(HASH_KEY key, void *value);
static void * hashChainSearch(hashChain *chain, HASH_KEY key);
static int hashChainAppend(hashChain *chain, HASH_KEY key, void *value);

hashMap * hashMapConstruct(hashMap *mem, int size, hashFunction func) {
    if (isNullPtr(mem) || isNullPtr(func) || size <= 0) {
        return null; 
    }
    memset(mem, 0, size);

    mem->size = size;
    mem->hashFunc = func;
    mem->space = (hashElem *)malloc(size * sizeof(hashElem));
    memset(mem->space, 0, size * sizeof(hashElem));

    return mem;
}

void * hashMapGet(hashMap *map, HASH_KEY key) {
    if (isNullPtr(map) || key < 0) {
        return NULL; 
    }


    int hashValue = map->hashFunc(key) % map->size; 
    hashElem *pElem = map->space + hashValue;
    
    if (HASH_ELEM_IS_COLLIDE(pElem)) {
        return hashChainSearch(pElem->chain, key);             
    }
    return pElem->value;
}

int hashMapPut(hashMap *map, HASH_KEY key, void *value) {
    if (isNullPtr(map) || key < 0 || isNullPtr(value)) {
        return FALSE; 
    }

    int hashValue = map->hashFunc(key) % map->size; 
    hashElem *pElem = map->space + hashValue;

    if (HASH_ELEM_IS_COLLIDE(pElem)) {
        return hashChainAppend(pElem->chain, key, value);        
    }
    pElem->value = value;
    return TRUE;
}

static hashChain * hashChainConstruct(HASH_KEY key, void *value) {
    if (key < 0 || isNullPtr(value)) {
        return NULL; 
    }

    hashChain *pChain = (hashChain *)malloc(sizeof(hashChain));
    pChain->key = key;
    pChain->value = value;
    memset(&pChain->node, 0, sizeof(listNode));

    return pChain;
}

static void * hashChainSearch(hashChain *chain, HASH_KEY key) {
       
}

static int hashChainAppend(hashChain *chain, HASH_KEY key, void *value) {

}

