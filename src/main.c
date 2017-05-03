#include <stdio.h>
#include "../include/list.h"
#include "../include/type.h"
#include "../include/mibTreeObjTree.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);

elementList elistHead;
char currentTable[SIZE_OF_CURRENT_TABLE];
char *sectionRecord[SIZE_OF_SECTION_RECORD];

int main() {
    int token;
    mibObjectTreeNode *node;
    mibObjectTreeInit(&mibObjectTreeRoot);
    node = &mibObjectTreeRoot;
    yyin = fopen("src/case", "r");
    if (yyin == NULL)
        printf("%s\n", "open failed");

    yyparse();

    showTree(&mibObjectTreeRoot);

    return 0;
}

