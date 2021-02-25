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

START_TEST (test_container_create) {
    MacroContainer c = macro_container();
    ck_assert_int_eq(c.length, 0);
}

START_TEST (test_ord_arg) {
    MacroPattern pattern = macro_compile("Test pattern <ord>");
    MacroContainer c = macro_container();

    int result_failure = macro_parse(pattern, "Test pattern ", &c);
    ck_assert_int_eq(c.length, 0);
    ck_assert_int_eq(result_failure, 0);

    int result_ok = macro_parse(pattern, "Test pattern is ok", &c);
    ck_assert_int_eq(c.length, 1);
    ck_assert_int_eq(result_ok, 1);

    ck_assert_str_eq(c.values[0], "is ok");
}

START_TEST (test_ord_arg_2) {
    MacroPattern pattern = macro_compile("Test <ord> pattern 2");
    MacroContainer c = macro_container();

    int result_failure_1 = macro_parse(pattern, "Test pattern", &c);
    ck_assert_int_eq(c.length, 0);
    ck_assert_int_eq(result_failure_1, 0);

    int result_failure_2 = macro_parse(pattern, "Something unrelated", &c);
    ck_assert_int_eq(c.length, 0);
    ck_assert_int_eq(result_failure_2, 0);

    int result_ok = macro_parse(pattern, "Test Test pattern pattern 2", &c);
    ck_assert_int_eq(c.length, 1);
    ck_assert_int_eq(result_ok, 1);

    ck_assert_str_eq(c.values[0], "Test pattern");
}


START_TEST (test_ord_arg_3) {
    MacroPattern pattern = macro_compile("<ord> is fine but i want <ord2>!");
    MacroContainer c = macro_container();

    int result_ok_1 = macro_parse(pattern, "it is fine but i want something else!", &c);
    ck_assert_int_eq(result_ok_1, 1);
    ck_assert_int_eq(c.length, 2);

    ck_assert_str_eq(c.values[0], "it");
    ck_assert_str_eq(c.values[1], "something else");

    macro_container_clean(&c);

    int result_ok_2 = macro_parse(pattern, "it is fine but i want it is fine but i want that!", &c);
    ck_assert_int_eq(result_ok_2, 1);
    ck_assert_int_eq(c.length, 2);

    ck_assert_str_eq(c.values[0], "it");
    ck_assert_str_eq(c.values[1], "it is fine but i want that");
}

START_TEST (test_json_cmp) {
    MacroPattern pattern = macro_compile("<ord1>, <ord2>");
    MacroContainer c = macro_container();
    macro_parse(pattern, "apple, cucumber", &c);
    char* json = malloc(128);
    macro_container_to_json(c, &json);
    ck_assert_str_eq(json, "{\"ord1\":\"apple\",\"ord2\":\"cucumber\"}");
}


Suite * macro_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Macro");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_ord_arg_cmp);
    tcase_add_test(tc_core, test_f2_arg_cmp);
    tcase_add_test(tc_core, test_container_create);
    tcase_add_test(tc_core, test_ord_arg);
    tcase_add_test(tc_core, test_ord_arg_2);
    tcase_add_test(tc_core, test_ord_arg_3);
    tcase_add_test(tc_core, test_json_cmp);

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