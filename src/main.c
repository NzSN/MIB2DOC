#include <stdio.h>
#include "../include/type.h"
#include "../include/tokenOp.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);

int main() {
    int token;
    MIB_NODE_INFO nodeInfo;

    yyin = fopen("src/case", "r");
    if (yyin == NULL)
        printf("%s\n", "open failed");

    yyparse();
    return 0;
}

