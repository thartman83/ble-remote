#include <unity.h>
#include <string.h>
#include <uri_info.h>

void setupUp() {
}

void tearDown() {
}

void test_decode_index_uri() {
  uri_info_t uri_info;

  decode_uri_info("/", &uri_info);
  TEST_ASSERT_EQUAL_STRING(index_html, uri_info.filelocation);
  TEST_ASSERT_EQUAL_STRING(html_ext, uri_info.ext);
  TEST_ASSERT_EQUAL_STRING(http_content_type_html, uri_info.content_type);
}

void test_decode_javascript_uri() {
  uri_info_t uri_info;
  const char path[] = "/src/foo.js";

  decode_uri_info(path, &uri_info);
  TEST_ASSERT_EQUAL_STRING(path, uri_info.filelocation);
  TEST_ASSERT_EQUAL_STRING(js_ext, uri_info.ext);
  TEST_ASSERT_EQUAL_STRING(http_content_type_js, uri_info.content_type);
}

void test_decode_css_uri() {
  uri_info_t uri_info;

  const char path[] = "/css/foo.css";

  decode_uri_info(path, &uri_info);
  TEST_ASSERT_EQUAL_STRING(path, uri_info.filelocation);
  TEST_ASSERT_EQUAL_STRING(css_ext, uri_info.ext);
  TEST_ASSERT_EQUAL_STRING(http_content_type_css, uri_info.content_type);
}

void test_decode_json_uri() {
  uri_info_t uri_info;

  const char path[] = "/data/important/bar.json";

  decode_uri_info(path, &uri_info);
  TEST_ASSERT_EQUAL_STRING(path, uri_info.filelocation);
  TEST_ASSERT_EQUAL_STRING(json_ext, uri_info.ext);
  TEST_ASSERT_EQUAL_STRING(http_content_type_json, uri_info.content_type);
}

void test_decode_ico_uri() {
  uri_info_t uri_info;

  const char path[] = "/favicon.ico";

  decode_uri_info(path, &uri_info);
  TEST_ASSERT_EQUAL_STRING(path, uri_info.filelocation);
  TEST_ASSERT_EQUAL_STRING(icon_ext, uri_info.ext);
  TEST_ASSERT_EQUAL_STRING(http_content_type_icon, uri_info.content_type);
}

void test_decode_fallback_uri() {
  uri_info_t uri_info;

  const char path[] = "/favtext.notanext";

  decode_uri_info(path, &uri_info);
  TEST_ASSERT_EQUAL_STRING(path, uri_info.filelocation);
  TEST_ASSERT_EQUAL_STRING("notanext", uri_info.ext);
  TEST_ASSERT_EQUAL_STRING(http_content_type_text, uri_info.content_type);
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
  RUN_TEST(test_decode_index_uri);
  RUN_TEST(test_decode_javascript_uri);
  RUN_TEST(test_decode_css_uri);
  RUN_TEST(test_decode_json_uri);
  RUN_TEST(test_decode_ico_uri);
  RUN_TEST(test_decode_fallback_uri);
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
