#include <string.h>
#include <dirent.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spiffs.h"
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "image_download_api.h"

static char *TAG = "image_download";

static esp_err_t http_event_handler(esp_http_client_event_t *evt)
{
    static uint32_t file_total_len = 0;
    static FILE *file = NULL;

    switch (evt->event_id)
    {
    case HTTP_EVENT_ERROR:
        ESP_LOGE(TAG, "HTTP_EVENT_ERROR");
        break;
    case HTTP_EVENT_ON_CONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
        if (file == NULL)
        {
            remove("/spiffs/image.png");
            file = fopen("/spiffs/image.png", "w");
            if (file == NULL)
            {
                ESP_LOGE(TAG, "Failed to open file for writing");
            }
        }
        break;
    case HTTP_EVENT_HEADER_SENT:
        ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
        break;
    case HTTP_EVENT_ON_HEADER:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", evt->header_key, evt->header_value);
        break;
    case HTTP_EVENT_ON_DATA:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_DATA, len=%d", evt->data_len);

        if (file)
        {
            int bytes_written = fwrite(evt->data, 1, evt->data_len, file);
            if (bytes_written < evt->data_len)
            {
                ESP_LOGE(TAG, "Failed to write all data to the file");
            }
            file_total_len += evt->data_len;
            ESP_LOGI(TAG, "Written image length %d", file_total_len);
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
        fclose(file);
        file = NULL;
        file_total_len = 0;
        break;
    case HTTP_EVENT_DISCONNECTED:
        ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
        break;
    case HTTP_EVENT_REDIRECT:
        ESP_LOGI(TAG, "HTTP_EVENT_REDIRECT");
        break;
    }

    return ESP_OK;
}

esp_err_t image_download_request(char *url)
{
    esp_http_client_config_t config = {
        .url = url,
        .event_handler = http_event_handler,
        .buffer_size = 228000,
        .buffer_size_tx = 4000,
        .timeout_ms = 40000,
        .crt_bundle_attach = esp_crt_bundle_attach,
    };

    esp_http_client_handle_t client = esp_http_client_init(&config);
    esp_err_t err = esp_http_client_perform(client);
    esp_http_client_cleanup(client);

    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "HTTP request failed, error = %d", err);
    }

    return err;
}