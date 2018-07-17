#ifndef _SYMBOLTBL_H_
#define _SYMBOLTBL_H_

#include "type.h"
#include "list.h"

#define MAX_CHAR_OF_MOD_IDENT (64)

typedef struct nodeMeta_t {
    char *nodeIdent;
    char *parentIdent;
    char *suffix;
} nodeMeta_t;

typedef struct leaveMeta_t {
    char *ident;
    char *parent;
    char *suffix;
    char *type;
    char *permission;
} leaveMeta_t;

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

typedef struct symbol_t {
    symbolType symType;
    char *symIdent;
    union {
       leaveMeta_t leaveMeta;
       nodeMeta_t nodeMeta;
    } symInfo;
    listNode symNode;
} symbol_t;

typedef struct symbolTable {
    char *modName;
    symbol_t *symbol;
    listNode symTblNode;
} symbolTable;

/* symbol_t and symbolTable */
symbolTable * symbolTableConstruct(char *name);
symbolTable * symbolTablePrev(symbolTable *tbl);
symbolTable * symbolTableNext(symbolTable *tbl);
symbolTable * symbolTableSearch(symbolTable *tblRoot, char *modName);
symbolTable * symbolModuleAdd(symbolTable *tblRoot, symbolTable *newTbl);
symbol_t * symbolPrev(symbol_t *sym);
symbol_t * symbolNext(symbol_t *sym);
symbol_t * symbolAdd(symbolTable *symTbl, symbol_t *newSym, char *modName);
symbol_t *symbolTravel(symbolTable *symbolTblRoot, int (*func)(symbol_t *sym, void *arg), void *arg);
int symbolSearchingByParent(symbolTable *symTblRoot, char *parent, symbol_t *sym);
int symbolSearching(symbolTable *symTblRoot, char *sym);

#endif /* _SYMBOLTBL_H_ */
