#include "unity.h"
#include "unity_config.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_ADTFunctionality(void) {
    TEST_ASSERT_EQUAL(1, 1);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_ADTFunctionality);
    return UNITY_END();
}