/* Generic Pair */

#ifndef _PAIR_H_
#define _PAIR_H_

typedef struct pair {
    void *left;
    void *right;
    int (*isEqual)(void *, void *);
    int (*copy)(void *);
    int (*release)(void *);
} pair;

/* Procedures implement as macro */
#define pairLeft(p) ((p)->left)
#define pairRight(p) ((p)->right)
#define pairSetLeft(p, l) ((p)->left = l)
#define pairSetRight(p, r) ((p)->right = r)
#define pairSetEqualMethod(p, m) ((p)->isEqual = m)
#define pairSetCopyMethod(p, m) ((p)->copy = m)
#define pairSetReleaseMethod(p, m) ((p)->release = m)

/* Prototype */
pair * pairConst();
pair * pairConstWithContent(void *l, void *r);
int    pairIsEqual(pair *l, pair *r);
pair * pairCopy(pair *p);
int    pairAssignment(pair *p_l, pair *p_r);

#endif /* _PAIR_H_ */
