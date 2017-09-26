#include <stdio.h>
#include "list.h"
#include "type.h"
#include "docGenerate.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);
extern char *beginFrom;

elementList elistHead;
char currentTable[SIZE_OF_CURRENT_TABLE];
char *sectionRecord[SIZE_OF_SECTION_RECORD];


int main() {
    int token;
    FILE *writeTo;
    mibObjectTreeNode *node;
    mibObjectTreeInit(&mibObjectTreeRoot);
    node = &mibObjectTreeRoot;

    beginFrom = "gponConfig";

    yyin = fopen("src/case", "r");
    if (yyin == NULL)
        printf("%s\n", "open failed");

    yyparse();

    writeTo = fopen("src/result/result", "w");
    if (writeTo == NULL)
        printf("%s\n", "open failed");

    documentGen(&mibObjectTreeRoot, writeTo);

    showTree(&mibObjectTreeRoot);

    return 0;
}
