/* hash.h */

#include "list.h"

#ifndef _MIB2DOC_HASHMAP_H_
#define _MIB2DOC_HASHMAP_H_

/* Defines */
typedef char * HASH_KEY;
typedef int (*hashFunction)(HASH_KEY);

#define HASH_ELEM_COLLIDE (1)
#define HASH_ELEM_NOT_COLLIDE (2)
#define HASH_ELEM_IS_COLLIDE(ELEM) (ELEM->isCollide == HASH_ELEM_COLLIDE)

typedef struct {
    HASH_KEY key;
    void *value;
    listNode node;
} hashChain;

typedef struct {
    int isCollide;
    HASH_KEY key;
    void *value;
    hashChain *chain;
} hashElem;

typedef struct {
    int size;
    hashFunction hashFunc;    
    hashElem *space;
} hashMap;

/* Function declarations */
hashMap * hashMapConstruct(hashMap *mem, int size, hashFunction func);
void * hashMapGet(hashMap *map, HASH_KEY key);

#endif /* _MIB2DOC_HASHMAP_H_ */

