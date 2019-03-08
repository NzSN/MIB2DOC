/* typeCheck.h */

#ifndef _TYPE_CHECK_H_
#define _TYPE_CHECK_H_

#include "typeTable.h"

_Bool typeCheck_isValid(const typeTable *tbl, const char *type);
_Bool typeCheck_isBuiltInType(const typeTable *tbl, const char *type);
_Bool typeCheck_isCustomType(const typeTable *tbl, const char *type);
_Bool typeCheck_isSequenceType(const typeTable *tbl, const char *type);

#endif /* _TYPE_CHECK_H_ */

/* typeCheck.h */

