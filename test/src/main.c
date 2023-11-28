#include "unity.h"
#include "unity_internals.h"
#include "vu8.h"
#include "rvu8.h"
#include "str.h"
#include "rvstr.h"
#include "vstr.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_vu8_stack_lifetime(void) {
    Vu8 v = (Vu8){ .last = 16, .items = (unsigned char []){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}};
    TEST_ASSERT_EQUAL(16, vu8_length(&v));
    for(size_t i = 0; i < vu8_length(&v); i++) {
        unsigned char val = vu8_get_at(&v, i);
        TEST_ASSERT_EQUAL(i, val);
    }
}

void test_vu8_push_back(void) {
    Vu8 v = {0};
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&v));

    size_t n = 0x10000;
    for(size_t i = 0; i < n; i++) {
        vu8_push_back(&v, i % 0x100);
    }
    for(size_t i = 0; i < n; i++) {
        unsigned char val = vu8_get_at(&v, i);
        TEST_ASSERT_EQUAL(i % 0x100, val);
    }

    vu8_free(&v);
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&v));
}

void test_vu8_push_front(void) {
    Vu8 v = {0};
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&v));
    size_t n = 0x1000;
    for(size_t i = 0; i < n; i++) {
        vu8_push_front(&v, i % 0x100);
    }
    for(size_t i = 0; i < n; i++) {
        unsigned char val = vu8_get_at(&v, n - i - 1);
        TEST_ASSERT_EQUAL(i % 0x100, val);
    }

    vu8_free(&v);
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&v));
}

void test_vu8_push_at(void) {
    Vu8 seq = {0};
    Vu8 v = {0};
    TEST_ASSERT_EQUAL(0, vu8_length(&seq));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&seq));
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&v));

    vu8_push_at(&v, 0, 70);
    TEST_ASSERT_EQUAL(1, vu8_length(&v));

    vu8_push_front(&v, 100);
    vu8_push_front(&v, 100);
    vu8_pop_front(&v, 0);
    vu8_pop_front(&v, 0);

    vu8_push_back(&v, 10);
    vu8_push_back(&v, 20);
    vu8_push_back(&v, 30);
    TEST_ASSERT_EQUAL(4, vu8_length(&v));

    vu8_push_at(&v, 0, 40);
    vu8_push_at(&v, 2, 50);
    TEST_ASSERT_EQUAL(6, vu8_length(&v));

    TEST_ASSERT_EQUAL(40, vu8_get_at(&v, 0));
    TEST_ASSERT_EQUAL(70, vu8_get_at(&v, 1));
    TEST_ASSERT_EQUAL(50, vu8_get_at(&v, 2));
    TEST_ASSERT_EQUAL(10, vu8_get_at(&v, 3));
    TEST_ASSERT_EQUAL(20, vu8_get_at(&v, 4));
    TEST_ASSERT_EQUAL(30, vu8_get_at(&v, 5));

    //TEST_ASSERT_EQUAL(30, vu8_get_back(&v));
    //TEST_ASSERT_EQUAL(30, vu8_get_at(&v, -1));

    vu8_free(&v);
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(0, vu8_reserved(&v));
}

void test_vu8_pop_at(void) {
    uint8_t val = 0;
    Vu8 v = {0};
    vu8_push_back(&v, 10);
    vu8_push_back(&v, 20);
    vu8_push_back(&v, 30);
    vu8_push_back(&v, 40);
    vu8_push_back(&v, 50);
    TEST_ASSERT_EQUAL(5, vu8_length(&v));

    vu8_pop_at(&v, 1, &val);
    TEST_ASSERT_EQUAL(4, vu8_length(&v));
    TEST_ASSERT_EQUAL(20, val);

    vu8_push_at(&v, 1, val);
    TEST_ASSERT_EQUAL(5, vu8_length(&v));

    vu8_pop_front(&v, &val);
    TEST_ASSERT_EQUAL(1, v.first);
    TEST_ASSERT_EQUAL(4, vu8_length(&v));
    TEST_ASSERT_EQUAL(10, val);

    vu8_pop_at(&v, 1, &val);
    TEST_ASSERT_EQUAL(3, vu8_length(&v));
    TEST_ASSERT_EQUAL(30, val);

    vu8_pop_at(&v, 2, &val);
    TEST_ASSERT_EQUAL(2, vu8_length(&v));
    TEST_ASSERT_EQUAL(50, val);

    vu8_pop_at(&v, 0, &val);
    TEST_ASSERT_EQUAL(1, vu8_length(&v));
    TEST_ASSERT_EQUAL(20, val);

    vu8_pop_at(&v, 0, &val);
    TEST_ASSERT_EQUAL(0, vu8_length(&v));
    TEST_ASSERT_EQUAL(40, val);

    vu8_free(&v);
}

void test_vu8_reverse(void)
{
    Vu8 v = {0};
    for(size_t i = 0; i < 256; i++) {
        TEST_ASSERT_EQUAL(0, vu8_push_back(&v, i));
    }
    vu8_reverse(&v);
    for(size_t i = 0; i < 256; i++) {
        TEST_ASSERT_EQUAL(i, vu8_get_at(&v, 255-i));
    }
    vu8_free(&v);
}

void test_vu8(void) {
    RUN_TEST(test_vu8_stack_lifetime);
    RUN_TEST(test_vu8_push_back);
    RUN_TEST(test_vu8_push_front);
    RUN_TEST(test_vu8_push_at);
    RUN_TEST(test_vu8_pop_at);
    RUN_TEST(test_vu8_reverse);
}

void test_rvu8_reverse(void)
{
    RVu8 v = {0};
    for(size_t i = 0; i < 256; i++) {
        TEST_ASSERT_EQUAL(0, rvu8_push_back(&v, (unsigned char *)&i));
    }
    rvu8_reverse(&v);
    for(size_t i = 0; i < 256; i++) {
        TEST_ASSERT_EQUAL(i, *rvu8_get_at(&v, 255-i));
    }
    rvu8_free(&v);
}

void test_rvu8(void) {
    RUN_TEST(test_rvu8_reverse);
}

void test_rvstr_basic(void) {
    RVStr rvs = {0};
    size_t n = 10000;
    //size_t reserved = 0;
    for(size_t i = 0; i < n; i++) {
        Str str = {0};
        TEST_ASSERT_EQUAL(0, str_fmt(&str, "%u", i));
        str_reverse(&str);
        TEST_ASSERT_EQUAL(0, rvstr_push_back(&rvs, &str));
        //reserved += str_reserved(&str);
    }
    rvstr_reverse(&rvs);
    rvstr_reverse(&rvs);
    for(size_t i = 0; i < n; i++) {
        Str *str = rvstr_get_at(&rvs, i);
        str_reverse(str);
        TEST_ASSERT_EQUAL(i, atoi(str->s));
        /* change is reflected within the original vector */
        Str *str2 = rvstr_get_at(&rvs, i);
        TEST_ASSERT_EQUAL(i, atoi(str2->s));
    }
    //printf("rvs reserved : %zu+%zu\n", rvstr_reserved(&rvs), reserved);
    rvstr_free(&rvs);
}

void test_vstr_basic(void) {
    VStr vs = {0};
    size_t n = 10000;
    for(size_t i = 0; i < n; i++) {
        Str str = {0};
        TEST_ASSERT_EQUAL(0, str_fmt(&str, "%u", i));
        str_reverse(&str);
        TEST_ASSERT_EQUAL(0, vstr_push_back(&vs, str));
    }
    vstr_reverse(&vs);
    vstr_reverse(&vs);
    for(size_t i = 0; i < n; i++) {
        Str str = vstr_get_at(&vs, i);
        str_reverse(&str);
        TEST_ASSERT_EQUAL(i, atoi(str.s));
        /* change is reflected within the original vector */
        Str str2 = vstr_get_at(&vs, i);
        TEST_ASSERT_EQUAL(i, atoi(str2.s));
    }
    vstr_free(&vs);
}

void test_rvstr(void) {
    RUN_TEST(test_rvstr_basic);
}

void test_vstr(void) {
    RUN_TEST(test_vstr_basic);
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    test_vu8();
    test_rvu8();
    test_vstr();
    test_rvstr();
    return UNITY_END();
}

