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
    SYNTAX_VALUE = 261,
    ACCESS_SPECIFIER = 262,
    ACCESS_VALUE = 263,
    STATUS_SPECIFIER = 264,
    STATUS_VALUE = 265,
    DESC_SPECIFIER = 266,
    DESC_VALUE = 267,
    MOUNT_POINT = 268,
    ASSIGNED = 269,
    BEGIN_ = 270,
    END_ = 271,
    DEF = 272,
    SEQ = 273,
    COMMA = 274,
    SEMICOLON = 275,
    OBJ_IDEN_ = 276,
    LBRACE = 277,
    RBRACE = 278,
    TYPE = 279,
    NUM = 280,
    FROM_ = 281,
    IMPORTS_ = 282
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
