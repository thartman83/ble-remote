#ifndef CONTENT_TYPE_H
#define CONTENT_TYPE_H

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

const char * get_content_type(const char * uri);

#endif//CONTENT_TYPE_H
