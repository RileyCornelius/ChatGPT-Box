#include "image_download_api.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "app_ui_ctrl.h"
#include "OpenAI.h"
#include "tts_api.h"
#include "app_sr.h"
#include "bsp/esp-bsp.h"
#include "bsp_board.h"
#include "app_audio.h"
#include "app_wifi.h"
#include "settings.h"
#include "ui.h"
#include "esp_spiffs.h"
#include <dirent.h>
#include "esp_crt_bundle.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "esp_spiffs.h"

// #define MAX_FILE_SIZE (1 * 1024 * 1024)
// #define FILE_SIZE (256000)

static char *TAG = "image_download";

char *image_url = NULL;

void scan_spiffs(void)
{
    struct dirent *p_dirent = NULL;
    DIR *p_dir_stream = opendir("/spiffs");

    /* Scan files in storage */
    while (true)
    {
        p_dirent = readdir(p_dir_stream);
        if (NULL != p_dirent)
        {
            ESP_LOGI(TAG, "Found file %s", p_dirent->d_name);
        }
        else
        {
            closedir(p_dir_stream);
            break;
        }
    }
}

void write_spiffs(void)
{
    // Define a buffer with the data you want to write
    char data_to_write[] = "Hello, SPIFFS!";

    // Open a file for writing. The second argument specifies the mode ("w" for write).
    FILE *file = fopen("/spiffs/image.png", "w");
    if (file == NULL)
    {
        ESP_LOGE(TAG, "Failed to open file for writing");
    }
    else
    {
        // Write data to the file
        int bytes_written = fwrite(data_to_write, 1, sizeof(data_to_write), file);
        if (bytes_written < sizeof(data_to_write))
        {
            ESP_LOGE(TAG, "Failed to write all data to the file");
        }

        // Close the file
        fclose(file);
    }
}

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
        scan_spiffs();

        ESP_LOGI(TAG, "Create a new style for the image");

        lv_obj_t *img = lv_img_create(ui_PanelSettings);
        lv_img_set_src(img, "S:/spiffs/image.png");
        lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

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

void download_image_request(char *url)
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
}

void create_image_ui()
{
    lv_obj_t *img = lv_img_create(ui_PanelSettings);
    lv_img_set_src(img, "S:/spiffs/emoji_u1f914.png");
    lv_obj_align(img, LV_ALIGN_CENTER, 0, 0);

    scan_spiffs();

    write_spiffs();
}