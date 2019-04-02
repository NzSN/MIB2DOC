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
    int ret;
    char *srcMib, *texPath;
    FILE *writeTo;
    mibObjectTreeNode *node;

    beginFrom = "org";

    // Option Manager Initialization
    // It will read option you provide in command line
    // and applie options to modules.
    optionsInit(argc, argv);

    // Syntax parser predeal
    // This function will initialize global variables that
    // may use during parsing.
    syntaxParserInit();

    // Dispatcher initialization
    // Set default dispatching mode correctly which
    // decide the action for some CFG bodys.
    dispatchInit();

    // Mib object tree global variables initialization.
    mibObjectTreeInit();

    // Callback set up for symbol collecting purposes.
    symbolCollectingInit();

    // Symbol table Initialization.
    symTableInit();

    // Type table initialization.
    typeTableInit();

    srcMib = optMngGetOptVal(optionsManager, OPT_KEY_SRC_MIB_FILE);
    if (isNullPtr(srcMib))
        abortWithMsg("Please give a path to a mib file.\n");

    texPath = optMngGetOptVal(optionsManager, OPT_KEY_TARGET_PDF);
    if (isNullPtr(texPath))
        texPath = "./mibDocs.tex";

    yyin = fopen(srcMib, "r");
    if (yyin == NULL)
        abortWithMsg("Can not open file : %s\n", srcMib);

    ret = yyparse();
    if (ret == ABORT) abortWithMsg("yyparse error\n");

    writeTo = fopen(texPath, "w+");
    if (writeTo == NULL)
        abortWithMsg("Can not open file : %s\n", texPath);

    documentGen(&trees, writeTo);
    showTree(&trees);

    fclose(yyin);
    fclose(writeTo);

    // PDF Gen
    //system("pdflatex result.tex");

    return 0;
}


