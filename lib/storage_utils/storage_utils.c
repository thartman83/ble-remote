#include <stdio.h>
#include <esp_spiffs.h>
#include <esp_log.h>
#include <esp_err.h>
#include "storage_utils.h"

void init_storage_volume() {
  ESP_STORAGE_DEBUG("Entering init storage volume");

  esp_vfs_spiffs_conf_t conf = {
    .base_path = STORAGE_VOLUME_BASE_PATH,
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true
  };

  ESP_ERROR_CHECK(esp_vfs_spiffs_register(&conf));

  ESP_STORAGE_DEBUG("Leaving init storage volume");
}

unsigned int read_file_chunk(FILE * fp, unsigned char * buf) {
  ESP_STORAGE_DEBUG("Entering read_file_chunk");
  size_t read_len = fread(buf, FILE_CHUNK_SIZE, 1, fp);
  ESP_STORAGE_DEBUG("Leaving read_file_chunk");
  return read_len;
}
