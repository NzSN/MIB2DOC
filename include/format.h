/* format.h 
 * Presentation layer specify the format of generated documentaion.
 */

#ifndef _FORMAT_H_
#define _FORMAT_H_

#include "type.h"
#include "mibTreeObjTree.h"

typedef struct tableInfo {
    char *identifier;
    char *oid;
    char *rw;
    char *type;
    char *desc;
    // Total length of all string
    int length;
} tableInfo;

typedef struct formatInfo {
    int beginOid;
} formatInfo;

/* Latex format */
int docGenInLatex(mibObjectTreeNode *from, char *filePath, formatInfo *info);


/* Plain text */

#endif /* _FORMAT_H_ */

