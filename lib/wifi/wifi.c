#include <stdio.h>
#include <string.h>
#include <freertos/FreeRTOS.h>
#include <esp_log.h>
#include <esp_err.h>
#include <nvs_flash.h>
#include <esp_task.h>
#include <freertos/task.h>
#include <esp_wifi.h>
#include <lwip/netdb.h>
#include "wifi.h"

QueueHandle_t wifi_msg_queue;

wifi_ap_record_t * ap_record;

static EventGroupHandle_t wifi_event_group;

wifi_status_t wifi_status;
SemaphoreHandle_t wifi_status_mutex = NULL;

static TaskHandle_t task_wifi = NULL;
static esp_netif_t* stn_netif = NULL;
static esp_netif_t* ap_netif = NULL;

static void process_wifi_event(void * arg, int32_t event_id, void * event_data) {
  switch(event_id) {
  case WIFI_EVENT_WIFI_READY:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_WIFI_READY");
    break;
  case WIFI_EVENT_SCAN_DONE:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_SCAN_DONE");
    break;
  case WIFI_EVENT_STA_START:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_STA_START");
    break;
  case WIFI_EVENT_STA_STOP:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_STA_STOP");
    break;
  case WIFI_EVENT_STA_CONNECTED:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_STA_CONNECTED");
    break;
  case WIFI_EVENT_STA_DISCONNECTED:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_STA_DISCONNECTED");
    break;
  case WIFI_EVENT_AP_START:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_AP_START");
    break;
  case WIFI_EVENT_AP_STOP:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_AP_STOP");
    break;
  case WIFI_EVENT_AP_STACONNECTED:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_AP_STACONNECTED");
    break;
  case WIFI_EVENT_AP_STADISCONNECTED:
    ESP_WIFI_DEBUG("Processing event WIFI_EVENT_AP_STADISCONNECTED");
    break;
  default:
    ESP_WIFI_DEBUG("Processed unhandled IP event %d", (int)event_id);
  }
}

static void process_ip_event(void * arg, int32_t event_id, void * event_dat) {
  switch(event_id) {
  case IP_EVENT_STA_GOT_IP:
    ESP_WIFI_DEBUG("Processed event IP_EVENT_STA_GOT_IP");
    break;
  case IP_EVENT_GOT_IP6:
    ESP_WIFI_DEBUG("Processed event IP_EVENT_GOT_IP6");
    break;
  case IP_EVENT_STA_LOST_IP:
    ESP_WIFI_DEBUG("Processed event IP_EVENT_STA_LOST_IP");
    break;
  default:
    ESP_WIFI_DEBUG("Processed unhandled IP event %d", (int)event_id);
  }
}

static void wifi_event_handler(void * arg, esp_event_base_t event_base,
                               int32_t event_id, void * event_data) {
  if(event_base == WIFI_EVENT)
    process_wifi_event(arg, event_id, event_data);
  else if(event_base == IP_EVENT) {
    process_ip_event(arg, event_id, event_data);
  }
}

static void update_ip_status(wifi_ap_sta_mode_t mode, const char * ip_addr,
                             const char * gw_addr, const char * netmask) {
  if(wifi_status_mutex) {
    ESP_WIFI_DEBUG("Updating ip status");

    xSemaphoreTake(wifi_status_mutex, portMAX_DELAY);
    wifi_status.mode = mode;
    strcpy(wifi_status.ip_addr, ip_addr);
    strcpy(wifi_status.gw_addr, gw_addr);
    strcpy(wifi_status.netmask, netmask);
    xSemaphoreGive(wifi_status_mutex);
  }
}

esp_err_t wifi_start_ap() {
  ESP_WIFI_DEBUG("Starting access point");

  esp_netif_ip_info_t ap_ip;

  // AP wifi configuration
	wifi_config_t ap_cfg = {
		.ap = {
			.ssid_len = 0,
			.channel = AP_CHANNEL,
			.ssid_hidden = AP_SSID_HIDDEN,
			.max_connection = AP_MAX_CONN,
			.beacon_interval = AP_BEACON_INTVL,
		},
	};
	memcpy(ap_cfg.ap.ssid, AP_SSID , sizeof(AP_SSID));

  ESP_WIFI_DEBUG("Setting Password and auth mode");
  // The shortest password length for wpa2 is 8 chars, if anything
  // less is set use open mode instead
  if(strlen( (char*) AP_PASSWD) < MIN_PASSWD_SIZE) {
    ap_cfg.ap.authmode = WIFI_AUTH_OPEN;
    memset(ap_cfg.ap.password, 0x00, sizeof(ap_cfg.ap.password));
  } else {
    ap_cfg.ap.authmode = AP_AUTH_MODE;
    memcpy(ap_cfg.ap.password, (char*)AP_PASSWD, sizeof(AP_PASSWD) );
  }

  /* before setting the static ip info we need to make sure the dhcp
     server is stopped */
  ESP_WIFI_DEBUG("Shutting down dhcp server");
  esp_netif_dhcps_stop(ap_netif);

  ESP_WIFI_DEBUG("Setting static ip info");
  memset(&ap_ip, 0x00, sizeof(ap_ip));
  inet_pton(AF_INET, AP_IP_ADDR, &ap_ip.ip);
  inet_pton(AF_INET, AP_GW_ADDR, &ap_ip.gw);
  inet_pton(AF_INET, AP_IP_NETMASK, &ap_ip.netmask);
  ESP_ERROR_CHECK(esp_netif_set_ip_info(ap_netif, &ap_ip));

  ESP_WIFI_DEBUG("Spinning the dhcp server back up");
  ESP_ERROR_CHECK(esp_netif_dhcps_start(ap_netif));

  ESP_WIFI_DEBUG("Turn on the AP");
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_cfg));
  ESP_ERROR_CHECK(esp_wifi_set_bandwidth(WIFI_IF_AP, AP_BANDWIDTH));

  // maximum power, just kidding sleeping wifi doesn't make sense in ap mode
  ESP_ERROR_CHECK(esp_wifi_set_ps(WIFI_PS_NONE));

  ESP_WIFI_DEBUG("Turning wifi on");
  ESP_ERROR_CHECK(esp_wifi_start());

  update_ip_status(WIFI_AP, AP_IP_ADDR, AP_GW_ADDR, AP_IP_NETMASK);

  return ESP_OK;
}

esp_err_t start_scan() {
  ESP_WIFI_DEBUG("Beginning wifi scan");

  wifi_scan_config_t scan_cfg = {
    .ssid = 0,
    .bssid = 0,
    .channel = 0,
    .show_hidden = true
  };

  ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_cfg, false));

  return ESP_OK;
}

void wifi_task_loop( void * params) {
  ESP_WIFI_DEBUG("Entering main wifi task loop");
  wifi_message_t msg;
  BaseType_t status;

  /* initialize the tcp stack */
	ESP_ERROR_CHECK(esp_netif_init());

	/* event loop for the wifi driver */
	ESP_ERROR_CHECK(esp_event_loop_create_default());

  /* initialize the default netif devices */
  ap_netif = esp_netif_create_default_wifi_ap();
  stn_netif = esp_netif_create_default_wifi_sta();

  /* get the default wifi configuration beginning and then initialize */
  wifi_init_config_t wifi_init_cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_init_cfg));
  //ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));

  // Register the event handler for wifi and ip events
  esp_event_handler_t wifi_event;
  esp_event_handler_t ip_event;

  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &wifi_event_handler, &wifi_event));
  ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
                                             &wifi_event_handler, &ip_event));

  // for the moment we are going to start in ap mode
  ESP_ERROR_CHECK(wifi_start_ap());

  while(1) {
    status = xQueueReceive(wifi_msg_queue, &msg, portMAX_DELAY);

    if(status != pdPASS)
      continue;

    switch (msg.code) {

      // Request events
    case WIFI_REQ_SCAN:
      ESP_WIFI_DEBUG("Handling WIFI_REQ_SCAN");
      start_scan();
      break;

    case WIFI_REQ_START_AP:
      ESP_WIFI_DEBUG("Etering WIFI_REQ_START_AP");
      break;
    case WIFI_REQ_STOP_AP:
      ESP_WIFI_DEBUG("ntering WIFI_REQ_STOP_AP");
      break;
    case WIFI_REQ_CONNECT_STA:
      ESP_WIFI_DEBUG("Enteing WIFI_REQ_CONNECT_STA");
      break;
    case WIFI_STATUS_SCAN_DONE:
      ESP_WIFI_DEBUG("Scan complete");
      wifi_event_sta_scan_done_t *evt_scan_done =
        (wifi_event_sta_scan_done_t*)msg.param;

      // scan did not complete successfully
      if(evt_scan_done->status != 0) {
        break;
      }

      break;
    default:

      ESP_WIFI_DEBUG("Unhandled wifi event %d", (int)msg.code);
      break;
    }

    // Yield the task for 250ms
    vTaskDelay(pdMS_TO_TICKS(TASK_YIELD_MS));
  }

  vTaskDelete(NULL);
}

BaseType_t wifi_send_message(wifi_event_code_t code, void * param) {
  wifi_message_t msg;
  msg.code = code;
  msg.param = param;
  return xQueueSend(wifi_msg_queue, &msg, portMAX_DELAY);
}

esp_err_t get_wifi_status(wifi_status_t * status) {
  if(!wifi_status_mutex)
    return ESP_FAIL;

  if(xSemaphoreTake(wifi_status_mutex, portMAX_DELAY) != pdTRUE)
    return ESP_FAIL;

  status->mode = wifi_status.mode;
  strcpy(status->ip_addr, wifi_status.ip_addr);
  strcpy(status->gw_addr, wifi_status.gw_addr);
  strcpy(status->netmask, wifi_status.netmask);

  return ESP_OK;
}

void wifi_start() {
  ESP_WIFI_DEBUG("Starting wifi...");

  esp_err_t ret = nvs_flash_init();

  // Check if the NVS part was truncated or contains data that is in a
  // format we can't read and re-init instead
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }

  // setup the shared data semphores
  ESP_WIFI_DEBUG("Creating status mutex");
  wifi_status_mutex = xSemaphoreCreateMutex();

  ESP_WIFI_DEBUG("Initializing message queue");
  wifi_msg_queue = xQueueCreate( 3, sizeof(wifi_message_t));

  ESP_WIFI_DEBUG("Creating event group");
  wifi_event_group = xEventGroupCreate();

  ESP_WIFI_DEBUG("Starting wifi task");
  xTaskCreate(&wifi_task_loop, "wifi", 4096, NULL, WIFI_TASK_PRIORITY, &task_wifi);

  ESP_WIFI_DEBUG("Wifi started.");
}

void wifi_stop() {
  ESP_WIFI_DEBUG("ERROR: not implemented");
}
