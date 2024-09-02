#ifndef WIFI_H
#define WIFI_H

#include <common.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WIFI_TAG "WIFI"

#define WIFI_LOG_LEVEL LOG_LEVEL
#define ESP_WIFI_DEBUG(...)                                                  \
  if (WIFI_LOG_LEVEL == 1)                                                   \
  ESP_LOGI(WIFI_TAG, __VA_ARGS__)

#define HOSTNAME "esp32"

#define MAX_SSID_SIZE 32
#define MAX_PASSWD_SIZE 64
#define MIN_PASSWD_SIZE 8

#define AP_AUTH_MODE WIFI_AUTH_WPA2_PSK
#define AP_SSID "esp32"
#define AP_PASSWD "esp32"
#define AP_BANDWIDTH WIFI_BW_HT20
#define AP_CHANNEL 1
#define AP_SSID_HIDDEN 0
#define AP_IP_ADDR "10.0.0.1"
#define AP_GW_ADDR "10.0.0.1"
#define AP_IP_NETMASK "255.255.255.0"
#define AP_MAX_CONN 3
#define AP_BEACON_INTVL 100

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <lwip/ip4_addr.h>

#define WIFI_TASK_PRIORITY 1

typedef enum wifi_event_code_t {
  NONE = 0,
  WIFI_REQ_SCAN,
  WIFI_REQ_START_AP,
  WIFI_REQ_STOP_AP,
  WIFI_REQ_CONNECT_STA,

  WIFI_STATUS_START_AP,
  WIFI_STATUS_STOP_AP,
  WIFI_STATUS_SCAN_START,
  WIFI_STATUS_SCAN_DONE,
  WIFI_STATUS_STA_CONNECTING,
  WIFI_STATUS_STA_CONNECTED,
} wifi_event_code_t;

typedef struct {
  wifi_event_code_t code;
  void *param;
} wifi_message_t;

typedef enum wifi_ap_sta_mode_t {
  WIFI_OFF = 0,
  WIFI_AP  = 1,
  WIFI_STA_CONNECTED = 2,
  WIFI_IDLE = 3
} wifi_ap_sta_mode_t;

typedef struct wifi_status_t {
  wifi_ap_sta_mode_t mode;
  char ip_addr[IP4ADDR_STRLEN_MAX];
  char gw_addr[IP4ADDR_STRLEN_MAX];
  char netmask[IP4ADDR_STRLEN_MAX];
} wifi_status_t;


//static SemaphoreHandle_t wifi_ap_list_mutex = NULL;

/* static EventGroupHandle_t wifi_event_group; */

/* const int WIFI_STA_CONNECTED_BIT = BIT0; */
/* const int WIFI_AP_STARTED_BIT = BIT1; */
/* const int WIFI_AP_SCANNING_BIT = BIT2; */

BaseType_t wifi_send_message(wifi_event_code_t code, void * param);

esp_err_t get_wifi_status(wifi_status_t * status);

void wifi_scan();
void wifi_start();

#ifdef __cplusplus
}
#endif

#endif // WIFI_H
