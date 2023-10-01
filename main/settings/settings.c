/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_check.h"
#include "bsp/esp-bsp.h"
#include "nvs_flash.h"
#include "nvs.h"
#include "esp_tinyuf2.h"
#include "settings.h"
#include "esp_ota_ops.h"

static const char *TAG = "settings";

const char *uf2_nvs_partition = "nvs";
const char *uf2_nvs_namespace = "configuration";

static nvs_handle_t my_handle;
static sys_param_t g_sys_param = {0};

static void uf2_nvs_modified_cb()
{
    ESP_LOGI(TAG, "uf2 nvs modified");
    settings_read_parameter_from_nvs();
}

void uf2_nvs_storage_init(void)
{
    tinyuf2_nvs_config_t nvs_config = DEFAULT_TINYUF2_NVS_CONFIG();
    nvs_config.part_name = uf2_nvs_partition;
    nvs_config.namespace_name = uf2_nvs_namespace;
    nvs_config.modified_cb = uf2_nvs_modified_cb;

    ESP_LOGI(TAG, "uf2 nvs storage init");
    ESP_ERROR_CHECK(esp_tinyuf2_install(NULL, &nvs_config));
}

esp_err_t settings_reset_parameter_to_nvs(void)
{
    ESP_LOGW(TAG, "default ssid|password:[%s:%s]", DEFAULT_ESP_WIFI_SSID, DEFAULT_ESP_WIFI_PASS);
    if ((0 == strlen(DEFAULT_ESP_WIFI_SSID)) || (0 == strlen(DEFAULT_ESP_WIFI_PASS)) || (0 == strlen(DEFAULT_OPENAI_KEY)))
    {
        memcpy(&g_sys_param.ssid[0], "", strlen(""));
        memcpy(&g_sys_param.password[0], "", strlen(""));
        memcpy(&g_sys_param.password[0], "", strlen(""));
    }
    else
    {
        memcpy(&g_sys_param.ssid[0], DEFAULT_ESP_WIFI_SSID, strlen(DEFAULT_ESP_WIFI_SSID));
        memcpy(&g_sys_param.password[0], DEFAULT_ESP_WIFI_PASS, strlen(DEFAULT_ESP_WIFI_PASS));
        memcpy(&g_sys_param.key[0], DEFAULT_OPENAI_KEY, strlen(DEFAULT_OPENAI_KEY));
    }

    // Write default values to NVS
    return settings_write_parameter_to_nvs();
}

esp_err_t settings_read_parameter_from_nvs(void)
{
    esp_err_t ret = nvs_open_from_partition(uf2_nvs_partition, uf2_nvs_namespace, NVS_READONLY, &my_handle);
    if (ESP_ERR_NVS_NOT_FOUND == ret)
    {
        ESP_LOGI(TAG, "Credentials not found");
        goto err;
    }

    ESP_GOTO_ON_FALSE(ESP_OK == ret, ret, err, TAG, "nvs open failed (0x%x)", ret);
    size_t len = 0;

    // Read SSID
    len = sizeof(g_sys_param.ssid);
    ret = nvs_get_str(my_handle, "ssid", g_sys_param.ssid, &len);
    if (ret != ESP_OK || len == 0)
    {
        ESP_LOGI(TAG, "No SSID found");
        goto err;
    }

    // Read password
    len = sizeof(g_sys_param.password);
    ret = nvs_get_str(my_handle, "password", g_sys_param.password, &len);
    if (ret != ESP_OK || len == 0)
    {
        ESP_LOGI(TAG, "No Password found");
        goto err;
    }

    // Read key
    len = sizeof(g_sys_param.key);
    ret = nvs_get_str(my_handle, "ChatGPT_key", g_sys_param.key, &len);
    if (ret != ESP_OK || len == 0)
    {
        ESP_LOGI(TAG, "No OpenAI key found");
        goto err;
    }
    nvs_close(my_handle);

    ESP_LOGI(TAG, "got ssid:%s", g_sys_param.ssid);
    ESP_LOGI(TAG, "got password:%s", g_sys_param.password);
    ESP_LOGI(TAG, "got OpenAI:%s", g_sys_param.key);

    if (0 == strlen(g_sys_param.ssid) || 0 == strlen(g_sys_param.password) || 0 == strlen(g_sys_param.key))
    {
        ESP_LOGI(TAG, "No SSID or Password or OpenAI key found on nvs");
        goto err;
    }

    return ESP_OK;

err:
    if (my_handle)
    {
        nvs_close(my_handle);
    }

    // If any error occurred, reset to default values;
    return settings_reset_parameter_to_nvs();
}

esp_err_t settings_write_parameter_to_nvs(void)
{
    ESP_LOGI(TAG, "Saving settings");
    esp_err_t err = nvs_open_from_partition(uf2_nvs_partition, uf2_nvs_namespace, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        ESP_ERROR_CHECK(nvs_set_str(my_handle, "ssid", g_sys_param.ssid));
        ESP_ERROR_CHECK(nvs_commit(my_handle));

        ESP_ERROR_CHECK(nvs_set_str(my_handle, "password", g_sys_param.password));
        ESP_ERROR_CHECK(nvs_commit(my_handle));

        ESP_ERROR_CHECK(nvs_set_str(my_handle, "ChatGPT_key", g_sys_param.key));
        ESP_ERROR_CHECK(nvs_commit(my_handle));

        ESP_LOGI(TAG, "stored ssid:%s", g_sys_param.ssid);
        ESP_LOGI(TAG, "stored password:%s", g_sys_param.password);
        ESP_LOGI(TAG, "stored ChatGPT key:%s", g_sys_param.key);
    }
    nvs_close(my_handle);

    return ESP_OK == err ? ESP_OK : ESP_FAIL;
}

void settings_set_parameter(sys_param_t *param)
{
    memcpy(&g_sys_param, param, sizeof(sys_param_t));
}

sys_param_t *settings_get_parameter(void)
{
    return &g_sys_param;
}
