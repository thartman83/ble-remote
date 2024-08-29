#ifndef URI_INFO_H
#define URI_INFO_H

#define MAX_EXT_LEN 10
#define MAX_CONTENT_TYPE_LEN 255
#define MAX_URI_PATH_LEN 255
#define BASE_PATH "/spiffs"

typedef struct {
  char ext[MAX_EXT_LEN];
  char content_type[MAX_CONTENT_TYPE_LEN];
  char filelocation[MAX_URI_PATH_LEN];
} uri_info_t;

const static char index_html[] = "/index.html";

const static char js_ext[] = "js";
const static char html_ext[] = "html";
const static char css_ext[] = "css";
const static char json_ext[] = "json";
const static char icon_ext[] = "ico";

const static char http_content_type_html[] = "text/html";
const static char http_content_type_js[] = "text/javascript";
const static char http_content_type_css[] = "text/css";
const static char http_content_type_json[] = "application/json";
const static char http_content_type_icon[] = "image/x-icon";
const static char http_content_type_text[] = "text/text";

void uri_extension(const char * uri, char * ret);
void decode_uri_info(const char * uri, uri_info_t * uri_info);

#endif//URI_INFO_H
