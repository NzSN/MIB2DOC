#include <stdio.h>
#include "list.h"
#include "type.h"
#include "docGenerate.h"
#include "options.h"
#include "dispatcher.h"
#include "mibTreeGen.h"
#include "symbolTbl.h"
#include "typeTable.h"

extern int yylex(void);
extern FILE *yyin;
extern int yyparse (void);
extern char *beginFrom;

slice sliceContainer;
char currentTable[SIZE_OF_CURRENT_TABLE];
char *sectionRecord[SIZE_OF_SECTION_RECORD];
extern mibObjectTreeNode mibObjectTreeRoot;

extern int syntaxParserInit(void);

int main(int argc, char *argv[]) {
    int token;
    int ret;
    FILE *writeTo;
    mibObjectTreeNode *node;
    mibLeaveInfo *pInfo; 

    beginFrom = "org";
    
    // Init
    syntaxParserInit();
    dispatchInit();
    mibObjectTreeInit();
    symbolCollectingInit();
    symTableInit();
    typeTableInit();
    
    node = &mibObjectTreeRoot;
    //optionsInit(argc, argv);
    
    yyin = fopen(argv[1], "r");
    if (yyin == NULL)
        printf("%s\n", "case open failed");
    
    ret = yyparse();
    if (ret == ABORT) {
        return -1;
    }

    writeTo = fopen("result.tex", "w+");
    if (writeTo == NULL)
        printf("%s\n", "result open failed");
    
    documentGen(&trees, writeTo);
    showTree(&trees);
    
    fclose(yyin); 
    fclose(writeTo);

    // PDF Gen
    system("pdflatex result.tex");

    return 0;
}


