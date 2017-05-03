//
// Created by ayden on 2017/4/19.
//

#ifndef GL5610_MIB_DOC_GEN_TYPE_H
#define GL5610_MIB_DOC_GEN_TYPE_H

#include <stdio.h>
#include "mibTreeObjTree.h"
#include "list.h"

/* define */
#define SIZE_OF_CURRENT_TABLE 64
#define SIZE_OF_SECTION_RECORD 1024
#define SIZE_OF_OID_STRING 256
#define IS_PTR_NULL(PTR) (PTR ? 0:1)
#define RELEASE_PTR(PTR) ({free(PTR); PTR=NULL;})

/* Extern */
extern char *sectionRecord[SIZE_OF_SECTION_RECORD];
extern mibObjectTreeNode mibObjectTreeRoot;
extern elementList elistHead;
/* Necesary Declaration */
extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);

/* Token Id Definition */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
typedef enum yytokentype {
    IDENTIFIER = 258,
    OBJ_SPECIFIER = 259,
    SYNTAX_SPECIFIER = 260,
    ACCESS_SPECIFIER = 261,
    ACCESS_VALUE = 262,
    STATUS_SPECIFIER = 263,
    STATUS_VALUE = 264,
    DESC_SPECIFIER = 265,
    DESC_VALUE = 266,
    MOUNT_POINT = 267,
    ASSIGNED = 268,
    BEGIN_ = 269,
    END_ = 270,
    DEF = 271,
    SEQ = 272,
    COMMA = 273,
    SEMICOLON = 274,
    INDEX_ = 275,
    TRAP_SPECIFIER = 276,
    OBJ_IDEN_ = 277,
    L_BRACE = 278,
    R_BRACE = 279,
    OBJECTS_ = 280,
    TYPE = 281,
    NUM = 282,
    FROM_ = 283,
    IMPORTS_ = 284
} tokenType;
#endif

typedef enum unitType {
    OBJECT = 1,
    TRAP,
    OBJECT_IDENTIFIER,
    SEQUENCE
} unitType;

enum elementType {
    IDENTIFIER_EL = 1,
    TYPE_EL,
    RW_EL,
    DESCRIPTION_EL,
    PARENT_EL,
    SUFFIX_EL
};

#endif //GL5610_MIB_DOC_GEN_TYPE_H
