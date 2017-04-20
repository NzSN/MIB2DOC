//
// Created by ayden on 2017/4/19.
//

#ifndef GL5610_MIB_DOC_GEN_TYPE_H
#define GL5610_MIB_DOC_GEN_TYPE_H
/* Token Id Definition */
enum yytokentype {
    IDENTIFIER = 258,
    OBJ_SPECIFIER = 259,
    SYNTAX_SPECIFIER = 260,
    SYNTAX_VALUE = 261,
    ACCESS_SPECIFIER = 262,
    ACCESS_VALUE = 263,
    STATUS_SPECIFIER = 264,
    STATUS_VALUE = 265,
    DESC_SPECIFIER = 266,
    DESC_VALUE = 267,
    MOUNT_POINT = 268,
    ASSIGNED = 269
};

/* Structure used to store Info of MIB node */
typedef struct {
    char name[200];
    char OID[200];
    char RW[10];
    char type[32];
    char DESC[32][255];
} MIB_NODE_INFO;
#endif //GL5610_MIB_DOC_GEN_TYPE_H
