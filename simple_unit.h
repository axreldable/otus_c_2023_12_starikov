#pragma once
#include <stdio.h>

typedef int (*test_fn)(void);

static int run_tests(const test_fn *tests, size_t count) {
    int failures = 0;

    for (size_t i = 0; i < count; ++i)
        failures += tests[i]();

    if (failures == 0)
        printf("ALL TESTS PASSED\n");

    return failures;
}

#define ASSERT_EQ(expected, actual) \
    do { \
        if ((expected) != (actual)) { \
            printf("FAIL: %s:%d expected %d, got %d\n", \
            __FILE__, __LINE__, (expected), (actual)); \
            return 1; \
        } \
    } while (0)

#define EXECUTE_TESTS(...) \
    do { \
        static const test_fn tests[] = { __VA_ARGS__ }; \
        return run_tests(tests, sizeof(tests) / sizeof(tests[0])); \
    } while (0)
