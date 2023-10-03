#pragma once

#include "esp_system.h"

/**
 * Download image from url
 * @param url url of image
 * @return ESP_OK on success, ESP_FAIL on error
 */
esp_err_t image_download_request(char *url);