/* typeTable.h */

#ifndef _TYPE_TABLE_
#define _TYPE_TABLE_

typedef struct typeItem {
    char *type;
    listNode node;
} typeItem;

typedef struct typeTable {
    unsigned int numOfTypes;
    typeItem items;
} typeTable;

extern typeTable * typeTableConst();
extern typeTable * typeTableCopy(const typeTable *tbl);
extern int typeTableDestruct(typeTable *tbl);
extern int typeTableAssignment(typeTable *lval, const typeTable *rval);
extern int typeTableIsEqual(const typeTable *firstTbl, const typeTable *secTbl);
extern int typeTableIsTypeExists(const typeTable *tbl, const char *type);
extern int typeTableAdd(typeTable *tbl, char *type);
extern int typeTableDel(typeTable *tbl, char *type);

#ifdef MIB2DOC_UNIT_TESTING

void list__TYPE_TABLE(void **state);

#endif 

#endif /* _TYPE_TABLE_ */

/* typeTable.h */

