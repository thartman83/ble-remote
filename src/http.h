#ifndef HTTP_H
#define HTTP_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HTTP_LOG_LEVEL         LOG_LEVEL
#define ESP_HTTP_DEBUG(...) \
  if(HTTP_LOG_LEVEL == 1) \
    ESP_LOGI(HTTP_TAG, __VA_ARGS__)

void http_app_start();

void http_app_stop();

#ifdef __cplusplus
}
#endif

#endif//HTTP_H
