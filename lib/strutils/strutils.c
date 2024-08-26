#include <string.h>
#include <ctype.h>
#include <strings.h>
#include "strutils.h"

int ends_with(const char * str, const char * suffix) {
  if(!str || !suffix)  return 0;

  size_t lenstr = strlen(str);
  size_t lensuffix = strlen(suffix);

  if(lensuffix > lenstr || lenstr == 0 || lensuffix == 0) {
    return 0;
  }

  return strcasecmp(str + lenstr - lensuffix, suffix) == 0;
}

void uri_extension(const char * uri, char * ret)
{
  size_t len = strlen(uri);

  while(len != 0) {
    --len;
    if(uri[len] == '.')
      break;
  }

  if(len == 0) {
    strcpy(ret, "");
  } else {
    strcpy(ret, uri + len+1);
  }
}
