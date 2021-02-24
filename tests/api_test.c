#include <check.h>
#include <stdlib.h>
#include "../src/macro.h"


START_TEST (test_ord_arg_cmp) {
    MacroPattern pattern = macro_compile("Test pattern <ord>");
    ck_assert_int_eq(pattern.length, 1);
    ck_assert_str_eq(pattern.pattern, "Test pattern <ord>");
    int i = 0;
    for (; i < pattern.length; i++) {
        ck_assert_str_eq(pattern.data[i].name, "ord");
        ck_assert_int_eq(pattern.data[i].after, 0);
    }
    ck_assert_int_eq(i, 1);
}

START_TEST (test_f2_arg_cmp) {
    MacroPattern pattern = macro_compile("Informative <!> <1>!");
    ck_assert_int_eq(pattern.length, 2);
    ck_assert_str_eq(pattern.pattern, "Informative <!> <1>!");
    int i = 0;
    for (; i < pattern.length; i++) {
        ck_assert_int_eq(pattern.data[i].after, 1);
        if (i == 0) ck_assert_int_eq(pattern.data[i].type, IGNORE);
        else if (i == 1) ck_assert_int_eq(pattern.data[i].type, ANYTHING);
        else break;
    }
    ck_assert_int_eq(i, 2);
}

Suite * macro_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Macro");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ord_arg_cmp);
    tcase_add_test(tc_core, test_f2_arg_cmp);

    suite_add_tcase(s, tc_core);

    return s;
}

int main(void) {
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = macro_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}