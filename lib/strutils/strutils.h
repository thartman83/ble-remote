#ifndef STRUTILS_H
#define STRUTILS_H

#define MAX_EXT_LEN 10

int ends_with(const char * str, const char * suffix);
void uri_extension(const char * uri, char * ret);

#endif//STRUTILS_H
