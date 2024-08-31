#include <wifi.h>
#include <http.h>
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_log.h>
#include <esp_spiffs.h>
#include <sys/stat.h>
#include <storage_utils.h>

void app_main()
{
  init_storage_volume();
  wifi_start();
  http_app_start();
}
