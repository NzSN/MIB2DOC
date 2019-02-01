/* hash.h */

#include "list.h"

#ifndef _MIB2DOC_HASHMAP_H_
#define _MIB2DOC_HASHMAP_H_

/* Defines */
#define HASH_ELEM_USED (1)
#define HASH_ELEM_NOT_USED (0)
#define HASH_ELEM_IS_USED(ELEM) (ELEM->used == HASH_ELEM_USED)
#define HASH_ELEM_COLLIDE (1)
#define HASH_ELEM_NOT_COLLIDE (0)
#define HASH_ELEM_IS_COLLIDE(ELEM) (ELEM->collide == HASH_ELEM_COLLIDE)

#define HASH_CHAIN_IS_LAST(C) (C->node.next == NULL)

#define PAIR_KEY_REF(P) (P->key)
#define PAIR_VAL_REF(P) (P->val)

#define PAIR_KEY(P) (P.key)
#define PAIR_VAL(P) (P.val)

#define PAIR_KEY_SET(P, K) (P.key = K)
#define PAIR_VAL_SET(P, V) (P.val = V)

typedef int (*hashFunction)(void *);

typedef struct {
    void * (*value)(void *key);
    // Compare 
    int (*isEqual)(void *lKey, void *rKey);
} pair_key_base;

typedef struct {
    // Compare
    int (*isEqual)(void *lVal, void *rVal);
} pair_val_base;

typedef struct {
    pair_key_base *key;
    pair_val_base *val;
} pair_kv;

typedef struct {
    pair_kv pair;
    listNode node;
} hashChain;

typedef struct {
    int used;
    int collide;
    pair_kv pair;
    hashChain *chain;
} hashElem;

typedef struct {
    int size;
    hashFunction hashFunc;    
    hashElem *space;
} hashMap;


/* Function declarations */
hashMap * hashMapConstruct(int size, hashFunction func);
int hashMapRelease(hashMap *map);
void * hashMapGet(hashMap *map, pair_kv pair);
int hashMapPut(hashMap *map, pair_kv pair);

#ifdef MIB2DOC_UNIT_TESTING

void hashTesting(void **state);

#endif /* MIB2DOC_UNIT_TESTING */

#endif /* _MIB2DOC_HASHMAP_H_ */

