#include <unity.h>

void test_dummy() {
    // Placeholder test: vervang dit later door echte logica-tests
    TEST_ASSERT_TRUE(true);
}

int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_dummy);
    return UNITY_END();
}
