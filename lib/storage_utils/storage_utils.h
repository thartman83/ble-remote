#include <stdio.h>

#ifndef STORAGE_UTILS_H
#define STORAGE_UTILS_H

#ifndef NATIVE
#include "common.h"
#include <esp_log.h>
#include <esp_spiffs.h>
#endif//NATIVE

#define FILE_CHUNK_SIZE 4096
#define STORAGE_LOG_LEVEL LOG_LEVEL
#define STORAGE_TAG "Storage"
#define STORAGE_VOLUME_BASE_PATH "/spiffs"

#ifndef NATIVE
#define ESP_STORAGE_DEBUG(...) \
  if(STORAGE_LOG_LEVEL == 1) \
    ESP_LOGI(STORAGE_TAG, __VA_ARGS__)
#else
#define ESP_STORAGE_DEBUG
#endif//NATIVE

#ifndef NATIVE
void init_storage_volume();
#endif//NATIVE

unsigned int read_file_chunk(FILE * fp, unsigned char * buf);


#endif//STORAGE_UTILS_H
