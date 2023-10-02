/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

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
#include "openai_api.h"
#include "image_download_api.h"

#define SCROLL_START_DELAY_S (1.5)
static char *TAG = "app_main";
static sys_param_t *sys_param = NULL;

extern uint8_t openai_service_type;

/* program flow. This function is called in app_audio.c if chat option is selected */
esp_err_t start_openai(uint8_t *audio, int audio_len)
{
    ui_ctrl_show_panel(UI_CTRL_PANEL_GET, 0);
    char *text = openai_transcribe_audio(audio, audio_len);

    if (text == NULL)
    {
        ui_ctrl_label_show_text(UI_CTRL_LABEL_LISTEN_SPEAK, "API Key is not valid");
        return ESP_FAIL;
    }

    if (strcmp(text, "invalid_request_error") == 0 || strcmp(text, "server_error") == 0 || strcmp(text, "") == 0)
    {
        ui_ctrl_label_show_text(UI_CTRL_LABEL_LISTEN_SPEAK, "Sorry, I can't understand.");
        ui_ctrl_show_panel(UI_CTRL_PANEL_SLEEP, 2000);
        return ESP_FAIL;
    }

    // UI listen success
    ui_ctrl_label_show_text(UI_CTRL_LABEL_REPLY_QUESTION, text);
    ui_ctrl_label_show_text(UI_CTRL_LABEL_LISTEN_SPEAK, text);

    if (openai_service_type == SERVICE_TYPE_CHAT)
    {
        char *response = openai_chat(text);

        if (response != NULL && (strcmp(response, "invalid_request_error") == 0 || strcmp(response, "server_error") == 0))
        {
            // UI listen fail
            ui_ctrl_label_show_text(UI_CTRL_LABEL_LISTEN_SPEAK, "Sorry, I can't understand.");
            ui_ctrl_show_panel(UI_CTRL_PANEL_SLEEP, 2000);
            return ESP_FAIL;
        }

        // UI listen success
        ui_ctrl_label_show_text(UI_CTRL_LABEL_REPLY_QUESTION, text);
        ui_ctrl_label_show_text(UI_CTRL_LABEL_LISTEN_SPEAK, response);

        if (strcmp(response, "invalid_request_error") == 0)
        {
            ui_ctrl_label_show_text(UI_CTRL_LABEL_LISTEN_SPEAK, "Sorry, I can't understand.");
            ui_ctrl_show_panel(UI_CTRL_PANEL_SLEEP, 2000);
            return ESP_FAIL;
        }

        ui_ctrl_label_show_text(UI_CTRL_LABEL_REPLY_CONTENT, response);
        ui_ctrl_show_panel(UI_CTRL_PANEL_REPLY, 0);
        esp_err_t status = text_to_speech_request(response, AUDIO_CODECS_MP3);

        if (status != ESP_OK)
        {
            ESP_LOGE(TAG, "Error creating ChatGPT request: %s\n", esp_err_to_name(status));
            // UI reply audio fail
            ui_ctrl_show_panel(UI_CTRL_PANEL_SLEEP, 0);
        }
        else
        {
            // Wait a moment before starting to scroll the reply content
            vTaskDelay(pdMS_TO_TICKS(SCROLL_START_DELAY_S * 1000));
            ui_ctrl_reply_set_audio_start_flag(true);
        }

        // Clearing resources
        openai_chat_cleanup();
    }
    else if (openai_service_type == SERVICE_TYPE_IMAGE)
    {
        char *image_url = openai_create_image_url(text);
        ESP_LOGI(TAG, "image_url: %s", image_url);
        image_download_request(image_url);

        ui_ctrl_go_to_image_screen();
    }
    free(text);

    return ESP_OK;
}

/* play audio function */
static void audio_play_finish_cb(void)
{
    ESP_LOGI(TAG, "replay audio end");
    if (ui_ctrl_reply_get_audio_start_flag())
    {
        ui_ctrl_reply_set_audio_end_flag(true);
    }
}

/* print memory usage */
void monitor_memory_task(void)
{
    while (true)
    {
        ESP_LOGD(TAG, "\tDescription\tInternal\tSPIRAM");
        ESP_LOGD(TAG, "Current Free Memory\t%d\t\t%d",
                 heap_caps_get_free_size(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
                 heap_caps_get_free_size(MALLOC_CAP_SPIRAM));
        ESP_LOGD(TAG, "Min. Ever Free Size\t%d\t\t%d",
                 heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT | MALLOC_CAP_INTERNAL),
                 heap_caps_get_minimum_free_size(MALLOC_CAP_SPIRAM));
        vTaskDelay(pdMS_TO_TICKS(5 * 1000));
    }
}

void app_main()
{
    settings_nvs_init();
    settings_read_parameter_from_nvs();
    sys_param = settings_get_parameter();
    openai_init(sys_param->key);

    bsp_spiffs_mount();
    bsp_i2c_init();
    bsp_display_start();
    bsp_board_init();
    bsp_display_backlight_on();

    ui_ctrl_init();
    app_network_start();

    app_sr_start();
    audio_register_play_finish_cb(audio_play_finish_cb);

    monitor_memory_task();
}
