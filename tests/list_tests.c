#include "minunit.h"
#include <lcthw/list.h>
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create() {
    list = List_create();
    mu_assert(list != NULL, "Failed to create list.");
    return NULL;
}

char *all_tests() {
    mu_suite_start();

    mu_run_test(test_create);
    return NULL;
}

RUN_TESTS(all_tests);
