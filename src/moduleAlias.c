/* moduleAlias.c */

#include "util.h"

/* Private Prototypes */
static char * isMatch(char *);

/* Private Variables */
static char *aliasArray[][2] = {
    { "RFC1271-MIB", "RMON-MIB" },
    { NULL, NULL }
};

/* Public Procedures */
char * moduleAliasTrans(char *orig) {
     char *after = isMatch(orig);
     if (after)
         return after;
     return orig;
}

/* Private Procedures */
static char * isMatch(char *modName) {
    int i = 0;

    while (true) {
        char *modName_ = aliasArray[i][0];
        if (modName_ == NULL)
            break;

        if (isStringEqual(modName, modName_))
            return strdup(aliasArray[i][1]);

        ++i;
    }
    return NULL;
}

#ifdef MIB2DOC_UNIT_TESTING

#include "test.h"

void moduleAliasTest(void **state) {
    char *before = "RFC1271-MIB", *after;

    after = moduleAliasTrans(before);
    assert_string_equal(after, aliasArray[0][1]);
}

#endif /* MIB2DOC_UNIT_TESTING */
