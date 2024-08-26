#include <unity.h>
#include <string.h>
#include <strutils.h>
#include <content_type.h>

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

void test_uri_extension() {
  char ret[10];

  uri_extension("", ret);
  TEST_ASSERT_EQUAL_STRING("", ret);

  uri_extension(".", ret);
  TEST_ASSERT_EQUAL_STRING("", ret);

  uri_extension("foo/", ret);
  TEST_ASSERT_EQUAL_STRING("", ret);

  uri_extension("/", ret);
  TEST_ASSERT_EQUAL_STRING("", ret);

  uri_extension("foo.html", ret);
  TEST_ASSERT_EQUAL_STRING("html", ret);

  uri_extension("foo/bob.json", ret);
  TEST_ASSERT_EQUAL_STRING("json", ret);

  uri_extension("foo/booboo.css", ret);
  TEST_ASSERT_EQUAL_STRING("css", ret);

  uri_extension("foo/bar/baz.js", ret);
  TEST_ASSERT_EQUAL_STRING("js", ret);

  uri_extension("foo.js", ret);

  TEST_ASSERT_GREATER_THAN(0, strcasecmp(js_ext, html_ext));
  TEST_ASSERT_EQUAL_STRING(js_ext, ret);
}

void runTests() {
  UNITY_BEGIN();
  RUN_TEST(test_ends_with);
  RUN_TEST(test_uri_extension);
  UNITY_END();
}

int main() {
  runTests();

  return 0;
}
