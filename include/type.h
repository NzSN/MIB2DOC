//
// Created by ayden on 2017/4/19.
//

#ifndef GL5610_MIB_DOC_GEN_TYPE_H
#define GL5610_MIB_DOC_GEN_TYPE_H

#include <stdio.h>

/* Necesary Declaration */
extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);

/* Token Id Definition */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
enum yytokentype {
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
    OBJ_IDEN_ = 276,
    L_BRACE = 277,
    R_BRACE = 278,
    TYPE = 279,
    NUM = 280,
    FROM_ = 281,
    IMPORTS_ = 282,
    COMMENT = 283
};
#endif

/* Structure used to store Info of MIB node */
typedef struct {
    char name[200];
    char OID[200];
    char RW[10];
    char type[32];
    char DESC[32][255];
} MIB_NODE_INFO;
#endif //GL5610_MIB_DOC_GEN_TYPE_H
