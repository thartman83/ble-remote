#include <unity.h>
#include <strutils.h>

void setUp() {
}

void tearDown() {
}

void test_ends_with() {
  TEST_ASSERT_EQUAL(1, ends_with("FOO.BAR", "bar"));
  TEST_ASSERT_EQUAL(0, ends_with("FOO.BAR", "baz"));
  TEST_ASSERT_EQUAL(1, ends_with("FOO.htMl", "html"));
  TEST_ASSERT_EQUAL(0, ends_with("", ""));
  TEST_ASSERT_EQUAL(0, ends_with("foo.html", ""));
  TEST_ASSERT_EQUAL(0, ends_with("", "html"));
}

void runTests() {
  UNITY_BEGIN();
  RUN_TEST(test_ends_with);
  UNITY_END();
}

#ifdef NATIVE
int main() {
  runTests();
  return 0;
}
#else
void app_main() {
  runTests();
}
#endif//NATIVE
