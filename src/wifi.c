#include <stdio.h>
#include <string.h>
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

const int WIFI_CONNECTED_BIT = BIT0;
const int WIFI_AP_STARTED_BIT = BIT1;
static TaskHandle_t task_wifi = NULL;
static esp_netif_t* stn_netif = NULL;
static esp_netif_t* ap_netif = NULL;

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

  return ESP_OK;
}

void wifi_task_loop( void * params) {
  ESP_WIFI_DEBUG("Entering main wifi task loop");
  wifi_message msg;
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

  // for the moment we are going to start in ap mode
  wifi_start_ap();

  while(1) {
    status = xQueueReceive(wifi_msg_queue, &msg, portMAX_DELAY);
  }

  vTaskDelete(NULL);
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

  ESP_WIFI_DEBUG("Initializing message queue");
  wifi_msg_queue = xQueueCreate( 3, sizeof(wifi_message));

  ESP_WIFI_DEBUG("Creating event group");
  wifi_event_group = xEventGroupCreate();

  ESP_WIFI_DEBUG("Starting wifi task");
  xTaskCreate(&wifi_task_loop, "wifi", 4096, NULL, WIFI_TASK_PRIORITY, &task_wifi);

  ESP_WIFI_DEBUG("Wifi started.");
}
