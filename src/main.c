#include <stdio.h>
#include "list.h"
#include "type.h"
#include "docGenerate.h"
#include "dispatcher.h"
extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);
extern char *beginFrom;

slice sliceContainer;
char currentTable[SIZE_OF_CURRENT_TABLE];
char *sectionRecord[SIZE_OF_SECTION_RECORD];
extern mibObjectTreeNode mibObjectTreeRoot;

int main(int argc, char *argv[]) {
    int token;
    int ret;
    FILE *writeTo;
    mibObjectTreeNode *node;
    
    beginFrom = "gponConfig";
    dispatchInit();
    mibObjectTreeInit(&mibObjectTreeRoot);
    node = &mibObjectTreeRoot;
    //optionsInit(argc, argv);
    
    yyin = fopen("src/case", "r");
    if (yyin == NULL)
        printf("%s\n", "open failed");
    
    ret = yyparse();
    if (ret == ABORT) {
        return -1;
    }

    writeTo = fopen("src/result/result", "w");
    if (writeTo == NULL) {
        printf("%s\n", "fatal:open failed");
        return -1;
    }

    documentGen(&mibObjectTreeRoot, writeTo);
    showTree(&mibObjectTreeRoot);
    return 0;
}
