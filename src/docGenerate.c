//
// Created by ayden on 2017/5/3.
//

#include <stdio.h>
#include <mem.h>
#include "../include/mibTreeObjTree.h"
#include "../include/docGenerate.h"

/* Local */
static int sectionLatex(char *secName, char *OID, int depth, FILE *writeTo);

int docGenerate(mibObjectTreeNode *root, char *beginNode,FILE *output) {

}

static int sectionLatex(char *secName, char *OID, int depth, FILE *writeTo) {
    enum {
        section = 1,
        subsection,
        subsubsection,
        subsubsubsection,
        paragraph,
        subparagraph
    };

    char *prefix;
    int i;

    for (i=0; i < SIZE_OF_SECTION_RECORD; i++) {
        if (strncmp(sectionRecord[i], secName, strlen(secName)) == 0)
            return 0;
        if (sectionRecord[i] == NULL)
            sectionRecord[i] = secName;
    }

    /*
     * Pending casue I should define a custome section command
     * in laTex
     */
    switch (depth) {
        case section:
            break;
        case subsection:
            break;
        case subsubsection:
            break;
        case subsubsubsection:
            break;
        case paragraph:
            break;
        default:
            break;
    }
    fprintf(writeTo, "\\%s {%s (%s)}\n", prefix, secName, OID);

    return 0;
}

static int tableLaTex();