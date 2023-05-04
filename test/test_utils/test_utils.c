#include <stdio.h>
#include "test_utils.h"

void test_numeric() {
    test_min();
    test_max();
    test_swap();
    test_float_equal();
    test_double_equal();

    printf("[PASS] numeric\n");
}

void test_random() {
    test_randint();
    test_uniform();
    test_shuffle();

    printf("[PASS] random\n");
}

void test_c_str() {
    test_str_clear();
    test_str_equal();
    test_str_equal_ignore_case();
    test_str_tolower();
    test_str_toupper();
    test_str_starts_with();
    test_str_ends_with();
    test_str_index_of_char();
    test_str_index_of_string();
    test_str_contains_string();
    test_str_reverse();
    test_str_strip();
    test_str_substring();
    test_str_count_substring();
    test_str_append_char();
    test_str_insert_char();
    test_str_insert_string();
    test_str_remove_char();
    test_str_remove_string();
    test_str_replace_char();
    test_str_replace_string();
    test_str_split();

    printf("[PASS] c_str\n");
}

void test_wrapper() {
    test_Character();
    test_UnsignedCharacter();
    test_Short();
    test_UnsignedShort();
    test_Integer();
    test_UnsignedInteger();
    test_Long();
    test_UnsignedLong();
    test_Float();
    test_Double();
    test_Boolean();

    printf("[PASS] wrapper\n");
}

void test_utils() {
    test_numeric();
    test_random();
    test_c_str();
    test_wrapper();
}
