/* typeTable.h */

#ifndef _TYPE_TABLE_
#define _TYPE_TABLE_

#include "list.h"

typedef enum {
    CATE_ERROR = 0,
    CATE_BUILD_IN = 1,
    CATE_SEQUENCE = 2,
    CATE_CUSTOM   = 3
} typeCate;

typedef struct typeItem {
    char *type;
    typeCate cate;

    union {
        char *typeDef;
        struct typeItem *ref;
    };

    listNode node;
} typeItem;

typedef struct typeTable {
    unsigned int numOfTypes;
    typeItem items;
} typeTable;

/* Member functions implemented as macros */

#define TYPE_ITEM_TYPE(ITEM) ((ITEM)->type)
#define TYPE_ITEM_SET_TYPE(ITEM, T) ((ITEM)->type = (T))

// Caution: typeDef field and ref field is in same union
//          which means their memory space is overlap.
#define TYPE_ITEM_DEF_VAL(ITEM) ((ITEM)->typeDef)
#define TYPE_ITEM_SET_DEF_VAL(ITEM, VAL) ((ITEM)->typeDef = (VAL))

#define TYPE_ITEM_REF(ITEM) ((ITEM)->ref)
#define TYPE_ITEM_SET_REF(ITEM, REF) ((ITEM)->ref = (REF))


extern typeTable * typeTableConst();
extern typeTable * typeTableCopy(const typeTable *tbl);
extern int typeTableDestruct(typeTable *tbl);
extern int typeTableAssignment(typeTable *lval, const typeTable *rval);
extern int typeTableIsEqual(const typeTable *firstTbl, const typeTable *secTbl);
extern int typeTableIsTypeExists(const typeTable *tbl, const char *type);
extern int typeTableAdd(typeTable *tbl, char *type, typeCate cate, const void *ref);
extern int typeTableDel(typeTable *tbl, char *type);
extern typeCate typeTableTypeCate(const typeTable *tbl, const char *type);
extern int typeTableRelBuild(typeTable *tbl);
extern typeItem * typeTableSearch(typeTable *tbl, const char *typeName);

#ifdef MIB2DOC_UNIT_TESTING

void list__TYPE_TABLE(void **state);

#endif

#endif /* _TYPE_TABLE_ */

/* typeTable.h */

