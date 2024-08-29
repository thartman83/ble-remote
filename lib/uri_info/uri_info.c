#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uri_info.h"

void decode_uri_info(const char * uri, uri_info_t * uri_info) {

  char ext[MAX_EXT_LEN];

  // check for the special case of a root request and return index.html
  if(strcmp(uri, "/") == 0) {
    strcpy(uri_info->content_type, http_content_type_html);
    strcpy(uri_info->ext, html_ext);
    sprintf(uri_info->filelocation, "%s%s", BASE_PATH, index_html);
    return;
  }

  // copy the file location
  sprintf(uri_info->filelocation, "%s%s", BASE_PATH, uri);

  // get the file extension
  uri_extension(uri, ext);

  // copy the extension
  strcpy(uri_info->ext, ext);

  // assign the rest of the values based on the extension
  if(strcasecmp(ext, "") == 0 || strcasecmp(ext, html_ext) == 0) {
    strcpy(uri_info->content_type,http_content_type_html);
  } else if(strcasecmp(ext, js_ext) == 0) {
    strcpy(uri_info->content_type,http_content_type_js);
  } else if(strcasecmp(ext, css_ext) == 0) {
    strcpy(uri_info->content_type,http_content_type_css);
  } else if(strcasecmp(ext, json_ext) == 0) {
    strcpy(uri_info->content_type,http_content_type_json);
  } else if(strcasecmp(ext, icon_ext) == 0) {
    strcpy(uri_info->content_type,http_content_type_icon);
  } else {
    strcpy(uri_info->content_type, http_content_type_text);
  }
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
