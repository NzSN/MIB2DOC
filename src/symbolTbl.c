/* symbolTbl.c */

#include "util.h"
#include "symbolTbl.h"


/* symbol_t and symbolTable operation function define */
symErrorCode symTblError;
extern symbolTable symTable;

typedef struct {
    pair_key_base base;
    char *ident;
} symbol_k;

static symbol_k * symbolKInit(symbol_k *key, char *name);

static symbol_k * symbolKCopy(symbol_k *key) {
    symbol_k *copy;

    if (isNullPtr(key))
        return NULL;

    copy = (symbol_k *)Malloc(sizeof(symbol_k));
    return symbolKInit(copy, strdup(key->ident));
}

static int symbolKRelease(symbol_k *key) {
    symbol_k *pKey = key;

    if (isNullPtr(key))
        return FALSE;

    RELEASE_MEM(pKey->ident);
    RELEASE_MEM(pKey);

    return TRUE;
}

static symbol_k * symbolKValue(symbol_k *key) {
    if (isNullPtr(key))
        return NULL;

    return (void *)key->ident;
}

static int symbolKIsEqual(symbol_k *lKey, symbol_k *rKey) {
    if (isNullPtr(lKey) || isNullPtr(rKey))
        return FALSE;

    return !strncmp(lKey->ident, rKey->ident, strlen(lKey->ident));
}

static symbol_k * symbolKInit(symbol_k *key, char *name) {
    if (isNullPtr(key) || isNullPtr(name))
        return NULL;

    key->ident = name;
    key->base.isEqual = (pairKeyIsEqual)symbolKIsEqual;
    key->base.release = (pairKeyRelease)symbolKRelease;
    key->base.value = (pairKeyValue)symbolKValue;
    key->base.copy = (pairKeyCopy)symbolKCopy;
    return key;
}

static symbol_k * symbolKConst(char *name) {
    symbol_k *pKey;

    if (isNullPtr(name))
        return NULL;

    pKey = (symbol_k *)Malloc(sizeof(symbol_k));

    return symbolKInit(pKey, name);
}

static int symbolTblHash(symbol_k *key) {
    char *keyStr;
    int hashVal, size, index, value;

    if (isNullPtr(key))
        return FALSE;

    keyStr = key->ident;

    hashVal = 0;
    size = strlen(keyStr);

    for (index = 0; index < size; ++index) {
        value = keyStr[index];
        hashVal += HASH_DEFAULT_METHOD(value);
    }
    return hashVal;
}

int symTableInit() {
    symbolTableInit(&symTable);
    symbolTableAdd(&symTable, symbolNodeConst("iso", "N/A", "1"));
    return OK;
}

symbolTable * symbolTableInit(symbolTable *tbl) {
    if (isNullPtr(tbl))
        return NULL;

    tbl->numOfSymbols = 0;
    tbl->symbolMap = hashMapConstruct(SIZE_OF_SYMBOL_TBL, (hashFunction)symbolTblHash);

    return tbl;
}

symbolTable * symbolTableConstruct() {
    symbolTable *newTable;

    newTable = (symbolTable *)Malloc(sizeof(symbolTable));
    return symbolTableInit(newTable);
}

int symbolTableRelease(symbolTable *tbl) {
    if (isNullPtr(tbl))
        return FALSE;

    hashMapRelease(tbl->symbolMap);
    RELEASE_MEM(tbl);

    return TRUE;
}

int symbolTableRelease_static(symbolTable *tbl) {
    if (isNullPtr(tbl))
        return FALSE;

    hashMapRelease(tbl->symbolMap);

    return TRUE;
}

symbol_t * symbolTableSearch(symbolTable *tbl, char *sym) {
    symbol_k key;
    hashMap *symMap;

    if (isNullPtr(tbl) || isNullPtr(sym))
        return NULL;

    symbolKInit(&key, sym);

    symMap = tbl->symbolMap;
    return (symbol_t *)hashMapGet(symMap, (pair_key_base *)&key);
}

int symbolTableAdd(symbolTable *tbl, symbol_t *sym) {
    symbol_k *key;

    if (isNullPtr(tbl) || isNullPtr(sym))
        return FALSE;

    key = symbolKConst(strdup(sym->symIdent));
    ++tbl->numOfSymbols;

    return hashMapPut(tbl->symbolMap, (pair_key_base *)key, (pair_val_base *)sym);
}

int symbolTableDelete(symbolTable *tbl, char *symIdent) {
    symbol_k key;

    if (isNullPtr(tbl) || isNullPtr(symIdent))
        return FALSE;
    symbolKInit(&key, symIdent);
    --tbl->numOfSymbols;
    return hashMapDelete(tbl->symbolMap, (pair_key_base *)&key);
}

inline static int nodeMetaRelease(nodeMeta_t *nm) {
    if (isNullPtr(nm))
        return FALSE;

    RELEASE_MEM(nm->parentIdent);
    RELEASE_MEM(nm->suffix);

    return TRUE;
}

inline static int leaveMetaRelease(leaveMeta_t *lm) {
    if (isNullPtr(lm))
        return FALSE;

    RELEASE_MEM(lm->parent);
    RELEASE_MEM(lm->suffix);
    RELEASE_MEM(lm->type);
    RELEASE_MEM(lm->permission);

    return TRUE;
}

symbol_t * symbolCopy(symbol_t *origin) {
    nodeMeta_t *node;
    leaveMeta_t *leave;
    if (isNullPtr(origin))
        return NULL;

    if (origin->symType == SYMBOL_TYPE_NODE) {
        node = &origin->symInfo.nodeMeta;
        return symbolNodeConst(strdup(origin->symIdent),
                               strdup(node->parentIdent), strdup(node->suffix));
    } else if (origin->symType == SYMBOL_TYPE_LEAVE) {
        leave = &origin->symInfo.leaveMeta;
        return symbolLeaveConst(strdup(origin->symIdent),
                                strdup(leave->parent), strdup(leave->suffix),
                                strdup(leave->type), strdup(leave->suffix));
    }
    return NULL;
}

int symbolRelease(symbol_t *sym) {
    if (isNullPtr(sym))
        return FALSE;

    if (sym->symType == SYMBOL_TYPE_NODE)
        nodeMetaRelease(&sym->symInfo.nodeMeta);
    else
        leaveMetaRelease(&sym->symInfo.leaveMeta);

    RELEASE_MEM(sym->symIdent);
    RELEASE_MEM(sym);

    return TRUE;
}

char * symbolIdent(symbol_t *sym) {
    if (isNullPtr(sym))
        return NULL;
    return sym->symIdent;
}

int symbolIsEqual(symbol_t *symLeft, symbol_t *symRight) {
    if (isNullPtr(symLeft) || isNullPtr(symRight))
        return FALSE;
    return !strncmp(symLeft->symIdent, symRight->symIdent, strlen(symLeft->symIdent));
}

symbol_t * symbolNodeConst(char *ident, char *parent, char *suffix) {
    symbol_t *pSym;

    if (isNullPtr(ident) || isNullPtr(parent) || isNullPtr(suffix))
        return NULL;

    pSym = (symbol_t *)Malloc(sizeof(symbol_t));
    pSym->symType = SYMBOL_TYPE_NODE;
    pSym->symIdent = ident;
    pSym->symInfo.nodeMeta.parentIdent = parent;
    pSym->symInfo.nodeMeta.suffix = suffix;

    pSym->base.release = (pairValRelease)symbolRelease;
    pSym->base.value = (pairValValue)symbolIdent;
    pSym->base.isEqual = (pairValIsEqual)symbolIsEqual;
    pSym->base.copy = (pairValCopy)symbolCopy;

    return pSym;
}

symbol_t * symbolLeaveConst(char *ident, char *parent, char *suffix, char *type, char *perm) {
    symbol_t *pSym;

    if (isNullPtr(ident) || isNullPtr(parent) || isNullPtr(suffix) ||
            isNullPtr(type) || isNullPtr(perm))
        return NULL;

    pSym = (symbol_t *)Malloc(sizeof(symbol_t));
    pSym->symType = SYMBOL_TYPE_LEAVE;
    pSym->symIdent = ident;

    pSym->symInfo.leaveMeta.parent = parent;
    pSym->symInfo.leaveMeta.suffix = suffix;
    pSym->symInfo.leaveMeta.type = type;
    pSym->symInfo.leaveMeta.permission = perm;

    pSym->base.release = (pairValRelease)symbolRelease;
    pSym->base.value = (pairValValue)symbolIdent;
    pSym->base.isEqual = (pairValIsEqual)symbolIsEqual;
    pSym->base.copy = (pairValCopy)symbolCopy;

    return pSym;
}
