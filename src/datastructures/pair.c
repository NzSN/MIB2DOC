/* Generic Pair */

#include "type.h"
#include "util.h"
#include "pair.h"

pair * pairConst() {
    pair *p = (pair *)zMalloc(sizeof(pair));
    return p;
}

pair * pairConstWithContent(void *l, void *r) {
    pair *p = pairConst();
    p->left = l;
    p->right = r;

    return p;
}

int pairIsEqual(pair *l, pair *r) {
    if (isNullPtr(l) || isNullPtr(r))
        return false;

    int isleftEqual = l->isEqual(l->left, r->left);
    int isRightEqual = l->isEqual(l->right, r->right);

    return isleftEqual && isRightEqual;
}

pair * pairCopy(pair *p) {
    if (isNullPtr(p))
        return null;

    pair *nP = (pair *)Malloc(sizeof(pair));
    nP->left = p->copy(p->left);
    nP->right = p->copy(p->right);

    pairSetEqualMethod(nP, p->isEqual);
    pairSetCopyMethod(nP, p->copy);

    return nP;
}

int pairAssignment(pair *p_l, pair *p_r) {
    if (isNullPtr(p_l) || isNullPtr(p_r))
        return ERROR;

    p_l->release(p_l->left);
    p_l->left = p_l->copy(p_r->left);

    p_l->release(p_l->right);
    p_l->right = p_l->copy(p_r->right);

    return OK;
}
