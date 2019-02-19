#include "type.h"
#include "list.h"
#include "hash.h"

#ifndef _SYMBOLTBL_H_
#define _SYMBOLTBL_H_

#define SIZE_OF_SYMBOL_TBL (256)
#define MAX_CHAR_OF_MOD_IDENT (64)

typedef enum symErrorCode {
    SYM_TABLE_EXISTS,
    SYM_TABLE_NOT_FOUND,
    SYM_EXISTS,
    SYM_NOT_FOUND
} symErrorCode;

extern symErrorCode symTblError;

typedef enum symbolType {
    SYMBOL_TYPE_NODE,
    SYMBOL_TYPE_LEAVE
} symbolType;

typedef struct nodeMeta_t {
    char *parentIdent;
    char *suffix;
} nodeMeta_t;

typedef struct leaveMeta_t {
    char *parent;
    char *suffix;
    char *type;
    char *permission;
} leaveMeta_t;

typedef struct symbol_t {
    pair_val_base base;
    symbolType symType;
    char *symIdent;
    union {
       leaveMeta_t leaveMeta;
       nodeMeta_t nodeMeta;
    } symInfo;
} symbol_t;

typedef struct symbolTable {
    int numOfSymbols;
    hashMap *symbolMap; 
} symbolTable;

/* symbolTable functions */
symbolTable * symbolTableInit();
symbolTable * symbolTableConstruct();
symbol_t * symbolTableSearch(symbolTable *tbl, char *symIdent);
int symbolTableAdd(symbolTable *tbl, symbol_t *sym);
int symbolTableDelete(symbolTable *tbl, char *symIdent);
int symbolTableRelease(symbolTable *tbl);
int symbolTableRelease_static(symbolTable *tbl);

/* symbol_t functions */
symbol_t * symbolNodeConst(char *ident, char *parent, char *suffix);
symbol_t * symbolLeaveConst(char *ident, char *parent, char *suffix,
    char *type, char *perm);

#ifdef MIB2DOC_UNIT_TESTING

void list_symbolTable(void **state);

#endif /* MIB2DOC_UNIT_TESTING */

#endif /* _SYMBOLTBL_H_ */

