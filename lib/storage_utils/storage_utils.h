#ifndef STORAGE_UTILS_H
#define STORAGE_UTILS_H

#include <stdio.h>
#include <esp_log.h>
#include <esp_err.h>
#include "common.h"

#define FILE_CHUNK_SIZE 4096
#define STORAGE_LOG_LEVEL LOG_LEVEL
#define STORAGE_TAG "Storage"
#define STORAGE_VOLUME_BASE_PATH "/spiffs"

#define ESP_STORAGE_DEBUG(...) \
  if(STORAGE_LOG_LEVEL == 1) \
    ESP_LOGI(STORAGE_TAG, __VA_ARGS__)

void init_storage_volume();
unsigned int read_file_chunk(FILE * fp, unsigned char * buf);


#endif//STORAGE_UTILS_H
