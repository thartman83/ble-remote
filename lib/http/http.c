#include <esp_http_server.h>
#include <esp_log.h>
#include <esp_err.h>
#include <uri_info.h>
#include <sys/stat.h>
#include <storage_utils.h>
#include "http.h"
#include "wifi.h"

const static char http_200_hdr[] = "200 OK";
static const char HTTP_TAG[] = "http_server";
static const char STATUS_RESP_BASE_JSON[] = "{ mode: %d, ip_addr: '%s', "
                                            "gw_addr: '%s', netmask: '%s'}";
static httpd_handle_t httpd_handle = NULL;

static esp_err_t http_server_get_handler(httpd_req_t *req)
{
  ESP_HTTP_DEBUG( "Entered http server get handler: %s", req->uri);
  uri_info_t uri_info;
  struct stat st;
  int ret;
  esp_err_t err = ESP_OK;
  //  unsigned char * buf = malloc(FILE_CHUNK_SIZE);
  unsigned char buf[FILE_CHUNK_SIZE];

  decode_uri_info(req->uri, &uri_info);
  // locate the file and return if available
  ret = stat(uri_info.filelocation, &st);

  if(ret != 0) {

    ESP_HTTP_DEBUG("File not found %s, lets bounce", uri_info.filelocation);
    httpd_resp_send_404(req);

  } else {
    ESP_HTTP_DEBUG("Found the file");

    httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
    httpd_resp_set_status(req, http_200_hdr);
    httpd_resp_set_type(req, uri_info.content_type);

    FILE *fp = fopen(uri_info.filelocation, "r");

    ESP_HTTP_DEBUG("The file is open");
    memset(buf,0,FILE_CHUNK_SIZE);

    // we should be able to grab the file all in one go
    if (st.st_size < FILE_CHUNK_SIZE) {
      ESP_HTTP_DEBUG("Read all in one go...");
      ret = read_file_chunk(fp, buf);
      ESP_HTTP_DEBUG("Sending data...");
      err = httpd_resp_send(req, (void *)buf, st.st_size);
    } else {
      unsigned int bytes_read = 0;
      while (bytes_read < st.st_size) {
        ret = fread(buf, FILE_CHUNK_SIZE, 1, fp);
        err = httpd_resp_send_chunk(req, (void *)buf,
                                    ret == 1 ? FILE_CHUNK_SIZE
                                             : st.st_size - bytes_read);
        if (err != ESP_OK)
          break;

        bytes_read += FILE_CHUNK_SIZE;
      }

      // we have send a final chunk of no data to indicate we are done
      if (err == ESP_OK)
        err = httpd_resp_send_chunk(req, 0, 0);
    }

    ESP_HTTP_DEBUG("Closing file pointer");

    fclose(fp);
  }

  return err;
}

static esp_err_t http_server_api_scan_handler(httpd_req_t *req) {
  wifi_send_message(WIFI_REQ_SCAN, (void *)NULL);

  char resp[] = "{status: 'Ok'}";

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_status(req,http_200_hdr);
  httpd_resp_set_type(req, http_content_type_json);
  httpd_resp_send(req, resp, strlen(resp));

  return ESP_OK;
}

static esp_err_t http_server_api_wifi_status_handler(httpd_req_t *req) {
  char resp[128];
  wifi_status_t wifi_status;
  esp_err_t ret;

  ret = get_wifi_status(&wifi_status);

  if(ret != ESP_OK)
    return ret;

  sprintf(resp, STATUS_RESP_BASE_JSON, wifi_status.mode, wifi_status.ip_addr,
          wifi_status.gw_addr, wifi_status.netmask);

  httpd_resp_set_hdr(req, "Access-Control-Allow-Origin", "*");
  httpd_resp_set_status(req,http_200_hdr);
  httpd_resp_set_type(req, http_content_type_json);
  httpd_resp_send(req, resp, strlen(resp));

  return ESP_OK;
}

static const httpd_uri_t http_server_api_scan_wifi_request = {
  .uri         = "/api/wifi/scan",
  .method      = HTTP_GET,
  .handler     = http_server_api_scan_handler
};

static const httpd_uri_t http_server_api_wifi_status = {
  .uri         = "/api/wifi",
  .method      = HTTP_GET,
  .handler     = http_server_api_wifi_status_handler
};

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
      httpd_register_uri_handler(httpd_handle, &http_server_api_scan_wifi_request);
      httpd_register_uri_handler(httpd_handle, &http_server_api_wifi_status);
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
