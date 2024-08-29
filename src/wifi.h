#ifndef WIFI_H
#define WIFI_H

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_TAG               "WIFI"

#define WIFI_LOG_LEVEL         LOG_LEVEL
#define ESP_WIFI_DEBUG(...) \
  if(WIFI_LOG_LEVEL == 1) \
    ESP_LOGI(WIFI_TAG, __VA_ARGS__)

#define HOSTNAME               "esp32"

#define MAX_SSID_SIZE          32
#define MAX_PASSWD_SIZE        64
#define MIN_PASSWD_SIZE        8

#define AP_AUTH_MODE           WIFI_AUTH_WPA2_PSK
#define AP_SSID                "esp32"
#define AP_PASSWD              "esp32"
#define AP_BANDWIDTH           WIFI_BW_HT20
#define AP_CHANNEL             1
#define AP_SSID_HIDDEN         0
#define AP_IP_ADDR             "10.0.0.1"
#define AP_GW_ADDR            "10.0.0.1"
#define AP_IP_NETMASK          "255.255.255.0"
#define AP_MAX_CONN            3
#define AP_BEACON_INTVL        100

#define WIFI_TASK_PRIORITY     1

typedef enum wifi_event_code_t {
  NONE = 0,
  WIFI_TASK_START_AP,
  WIFI_TASK_STOP_AP,
} wifi_event_code_t;

typedef struct {
  wifi_event_code_t code;
  void * param;
} wifi_message;

void wifi_start();

#ifdef __cplusplus
}
#endif


#endif//WIFI_H
