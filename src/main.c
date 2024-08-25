#include "wifi.h"
#include "http.h"
#include <esp_event.h>
#include <esp_wifi.h>
#include <esp_log.h>

void app_main()
{
  wifi_start();
  http_app_start();
}
