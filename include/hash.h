/* hash.h */

#include "list.h"

#ifndef _MIB2DOC_HASHMAP_H_
#define _MIB2DOC_HASHMAP_H_

/* Defines */
typedef int (*hashFunction)(void *);

#define HASH_ELEM_COLLIDE (1)
#define HASH_ELEM_NOT_COLLIDE (2)
#define HASH_ELEM_IS_COLLIDE(ELEM) (ELEM->isCollide == HASH_ELEM_COLLIDE)

#define PAIR_KEY_REF(P) (P->key)
#define PAIR_VAL_REF(P) (P->val)

#define PAIR_KEY(P) (P.key)
#define PAIR_VAL(P) (P.val)

#define PAIR_KEY_SET(P, K) (P.key = K)
#define PAIR_VAL_SET(P, V) (P.val = V)

typedef struct {
    void * key;
    void * val;
} pair_kv;

typedef struct {
    pair_kv pair;
    listNode node;
} hashChain;

typedef struct {
    int isCollide;
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

