#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_err.h>
#include "http.h"

const static char http_200_hdr[] = "200 OK";
static const char HTTP_TAG[] = "http_server";

static httpd_handle_t httpd_handle = NULL;

static esp_err_t http_server_get_handler(httpd_req_t *req)
{
  ESP_HTTP_DEBUG( "Entered http server get handler: %s", req->uri);

  httpd_resp_set_status(req, http_200_hdr);
  httpd_resp_set_type(req, "text/html");
  httpd_resp_send(req, "<html>Hello, World!</html>",
                  sizeof("<html>Hello, World!</html>"));

  return ESP_OK;
}

static const httpd_uri_t http_server_get_request = {
    .uri       = "*",
    .method    = HTTP_GET,
    .handler   = http_server_get_handler
};

void http_app_start()
{
  esp_err_t err;

  if(httpd_handle == NULL) {
    httpd_config_t cfg = HTTPD_DEFAULT_CONFIG();

    cfg.uri_match_fn = httpd_uri_match_wildcard;
    cfg.lru_purge_enable = false;

    ESP_HTTP_DEBUG( "Starting http server");
    err = httpd_start(&httpd_handle, &cfg);
    if (err == ESP_OK) {
      httpd_register_uri_handler(httpd_handle, &http_server_get_request);
      ESP_HTTP_DEBUG( "Started http server");
    } else {
      ESP_LOGE(HTTP_TAG, "UNABLE TO START HTTP SERVER");
    }
  }
}

void http_app_stop()
{
  if(httpd_handle) {
    httpd_stop(httpd_handle);
    httpd_handle = NULL;
  }
}
