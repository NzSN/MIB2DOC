//
// Created by ayden on 2017/4/25.
//

#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <malloc.h>
#include <string.h>
#include "../../include/type.h"
#include "../../include/list.h"

elementList elistHead;
char *sectionRecord[SIZE_OF_SECTION_RECORD];

/* A test case that does nothing and succeeds. */
static void list_test(void **state) {
    elementList *new1;
    elementList *new2;
    elementList *got;

    memset(&elistHead, 0, sizeof(elementList));

    new1 = buildElement(IDENTIFIER_EL, "LLLL");
    appendElement_el(&elistHead, new1);

    new2 = buildElement(TYPE_EL, "lllll");
    appendElement_el(&elistHead, new2);

    got = getElement_el(&elistHead, IDENTIFIER_EL);
    if ( (got->content != new1->content) || (got->key != new1->key))
        fail();

    got = getElement_el(&elistHead, TYPE_EL);
    if ( (got->content != new2->content) || (got->key != new2->key))
        fail();


    flushAll_el(&elistHead);
    got = getElement_el(&elistHead, IDENTIFIER_EL);
    if (got != NULL)
        fail();

}

int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(list_test),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}