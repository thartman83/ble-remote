#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "content_type.h"
#include "strutils.h"

const char * get_content_type(const char * uri) {
  char ext[MAX_EXT_LEN];

  uri_extension(uri, ext);

  if(strcasecmp(ext, html_ext) == 0)
    return http_content_type_html;
  else if(strcasecmp(ext, js_ext) == 0)
    return http_content_type_js;
  else if(strcasecmp(ext, css_ext) == 0)
    return http_content_type_css;
  else if(strcasecmp(ext, json_ext) == 0)
    return http_content_type_json;
  else if(strcasecmp(ext, icon_ext) == 0)
    return http_content_type_icon;
  else
    return http_content_type_text;
}
