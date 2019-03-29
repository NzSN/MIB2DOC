/* typeCheck.c */

#include "typeCheck.h"

static typeCate typeCheck_CateOfType(const typeTable *tbl, const char *type) {
    if (isNullPtr(tbl) || isNullPtr(type))
        return ERROR;
    return typeTableTypeCate(tbl, type);
}

_Bool typeCheck_isBuiltInType(const typeTable *tbl, const char *type) {
    typeCate cate = typeCheck_CateOfType(tbl, type);

    if (cate == CATE_BUILD_IN) return TRUE;
    return FALSE;
}

_Bool typeCheck_isCustomType(const typeTable *tbl, const char *type) {
    typeCate cate = typeCheck_CateOfType(tbl, type);
    if (cate == CATE_CUSTOM) return TRUE;

    return FALSE;
}

_Bool typeCheck_isSequenceType(const typeTable *tbl, const char *type) {
    typeCate cate = typeCheck_CateOfType(tbl, type);
    if (cate == CATE_SEQUENCE) return TRUE;

    return FALSE;
}

_Bool typeCheck_isValid(const typeTable *tbl, const char *type) {
    _Bool isValid;

    isValid = typeCheck_isBuiltInType(tbl, type);
    if (isValid == TRUE) return TRUE;

    isValid = typeCheck_isCustomType(tbl, type);
    if (isValid == TRUE) return TRUE;

    isValid = typeCheck_isSequenceType(tbl, type);
    if (isValid == TRUE) return TRUE;

    return FALSE;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void typeCheck__TYPE_CHECK(void **state) {

}

#endif

/* typeCheck.c */

