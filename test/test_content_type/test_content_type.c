#include <unity.h>
#include <content_type.h>

void setupUp() {
}

void tearDown() {
}

void test_get_content_type() {
  TEST_ASSERT_EQUAL_STRING(http_content_type_html, get_content_type("foo.html"));
  TEST_ASSERT_EQUAL_STRING(http_content_type_js, get_content_type("foo.js"));
  TEST_ASSERT_EQUAL_STRING(http_content_type_css, get_content_type("foo.css"));
  TEST_ASSERT_EQUAL_STRING(http_content_type_json, get_content_type("foo.json"));
  TEST_ASSERT_EQUAL_STRING(http_content_type_icon, get_content_type("favicon.ico"));
  TEST_ASSERT_EQUAL_STRING(http_content_type_text, get_content_type("foo"));
  TEST_ASSERT_EQUAL_STRING(http_content_type_text, get_content_type("foo.bar"));
}

void runTests() {
  UNITY_BEGIN();
  RUN_TEST(test_get_content_type);
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
