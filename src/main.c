#include <wifi.h>
#include <http.h>
#include <storage_utils.h>

void app_main()
{
  init_storage_volume();
  wifi_start();
  http_app_start();
}
