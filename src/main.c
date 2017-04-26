#include <stdio.h>
#include "../include/type.h"
#include "../include/mibTreeGen.h"
#include "../include/queue.h"
#include "../include/mibTreeObjTree.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);

Queue dataQueue;
char currentTable[SIZE_OF_CURRENT_TABLE];
char *sectionRecord[SIZE_OF_SECTION_RECORD];

int main() {
    int token;
    MIB_NODE_INFO nodeInfo;

    yyin = fopen("src/case", "r");
    if (yyin == NULL)
        printf("%s\n", "open failed");

    yyparse();
    return 0;
}

