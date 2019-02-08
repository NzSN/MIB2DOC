#include <stdio.h>
#include "list.h"
#include "type.h"
#include "docGenerate.h"
#include "options.h"
#include "dispatcher.h"
#include "yy_syn.tab.h"

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
    mibLeaveInfo *pInfo; 

    beginFrom = "sys";

    syntaxParserInit();
    dispatchInit();
    mibObjectTreeInit(&mibObjectTreeRoot);

    node = &mibObjectTreeRoot;
    optionsInit(argc, argv);
    
    yyin = fopen("src/case.mib", "r");
    if (yyin == NULL)
        printf("%s\n", "case open failed");
    
    ret = yyparse();
    if (ret == ABORT) {
        return -1;
    }

    writeTo = fopen("src/result", "w+");
    if (writeTo == NULL)
        printf("%s\n", "result open failed");

    documentGen(&mibObjectTreeRoot, writeTo);
    showTree(&mibObjectTreeRoot);
    return 0;
}
