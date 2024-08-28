#include <unity.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include "storage_utils.h"

#define SMALL_TXT "test/test_storage_utils/test_small.txt"
#define EXPECTED_SMALL_TXT "This is a small test text file\n"
#define LARGE_TXT "test/test_storage_utils/test_large.txt"
#define BUFLEN_TXT "test/test_storage_utils/test_buflen.txt"

void setUp() {

}

void tearDown() {

}

void test_small_text() {
  FILE * fp = fopen(SMALL_TXT, "r");
  unsigned char buf[FILE_CHUNK_SIZE];
  unsigned int read_len = 0;
  struct stat st;

  TEST_ASSERT_EQUAL(0, stat(SMALL_TXT, &st));

  read_len = read_file_chunk(fp, buf);

  TEST_ASSERT_EQUAL(0, read_len);
  TEST_ASSERT_EQUAL_STRING(EXPECTED_SMALL_TXT, buf);
  TEST_ASSERT_EQUAL(strlen(EXPECTED_SMALL_TXT), strlen((char*)buf));

  fclose(fp);
}

void test_large_text() {
  FILE * fp = fopen(LARGE_TXT, "r");
  unsigned char buf[FILE_CHUNK_SIZE];
  struct stat st;

  TEST_ASSERT_EQUAL(0, stat(BUFLEN_TXT, &st));
  TEST_ASSERT_EQUAL(1, read_file_chunk(fp, buf));

  memset(buf, 0, FILE_CHUNK_SIZE);

  TEST_ASSERT_EQUAL(0, read_file_chunk(fp, buf));
  TEST_ASSERT_EQUAL(st.st_size - FILE_CHUNK_SIZE, strlen((char*)buf));

  fclose(fp);
}

void runTests() {
  UNITY_BEGIN();
  RUN_TEST(test_small_text);
  RUN_TEST(test_large_text);
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
