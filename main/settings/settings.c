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
    /* install UF2 NVS */
    tinyuf2_nvs_config_t nvs_config = DEFAULT_TINYUF2_NVS_CONFIG();
    nvs_config.part_name = uf2_nvs_partition;
    nvs_config.namespace_name = uf2_nvs_namespace;
    nvs_config.modified_cb = uf2_nvs_modified_cb;

    ESP_ERROR_CHECK(esp_tinyuf2_install(NULL, &nvs_config));
}

esp_err_t settings_factory_reset(void)
{
    ESP_LOGW(TAG, "ssid|password:[%s:%s]", DEFAULT_ESP_WIFI_SSID, DEFAULT_ESP_WIFI_PASS);
    if ((0 == strlen(DEFAULT_ESP_WIFI_SSID)) || (0 == strlen(DEFAULT_ESP_WIFI_PASS)))
    {
        memcpy(&g_sys_param.ssid[0], "DEFAULT_ESP_WIFI_SSID", strlen("DEFAULT_ESP_WIFI_SSID"));
        memcpy(&g_sys_param.password[0], "DEFAULT_ESP_WIFI_PASS", strlen("DEFAULT_ESP_WIFI_PASS"));
    }
    else
    {
        memcpy(&g_sys_param.ssid[0], DEFAULT_ESP_WIFI_SSID, strlen(DEFAULT_ESP_WIFI_SSID));
        memcpy(&g_sys_param.password[0], DEFAULT_ESP_WIFI_PASS, strlen(DEFAULT_ESP_WIFI_PASS));
    }

    settings_write_parameter_to_nvs();

    esp_restart();
    return ESP_OK;
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

    ESP_LOGI(TAG, "stored ssid:%s", g_sys_param.ssid);
    ESP_LOGI(TAG, "stored password:%s", g_sys_param.password);
    ESP_LOGI(TAG, "stored OpenAI:%s", g_sys_param.key);
    return ESP_OK;

err:
    if (my_handle)
    {
        nvs_close(my_handle);
    }
    // settings_factory_reset();
    settings_write_parameter_to_nvs();
    uf2_nvs_storage_init();
    return ret;
}

esp_err_t settings_write_parameter_to_nvs(void)
{
    ESP_LOGI(TAG, "Saving settings");

    size_t buf_len_long;
    esp_err_t err = nvs_open_from_partition(uf2_nvs_partition, uf2_nvs_namespace, NVS_READWRITE, &my_handle);
    if (err != ESP_OK)
    {
        ESP_LOGI(TAG, "Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        buf_len_long = sizeof(g_sys_param.ssid);
        err = nvs_get_str(my_handle, "ssid", g_sys_param.ssid, &buf_len_long);
        if (err != ESP_OK || buf_len_long == 0)
        {
            ESP_ERROR_CHECK(nvs_set_str(my_handle, "ssid", CONFIG_ESP_WIFI_SSID));
            ESP_ERROR_CHECK(nvs_commit(my_handle));
            ESP_LOGI(TAG, "no ssid, give a init value to nvs");
        }
        else
        {
            ESP_LOGI(TAG, "stored ssid:%s", g_sys_param.ssid);
        }

        buf_len_long = sizeof(g_sys_param.password);
        err = nvs_get_str(my_handle, "password", g_sys_param.password, &buf_len_long);
        if (err != ESP_OK || buf_len_long == 0)
        {
            ESP_ERROR_CHECK(nvs_set_str(my_handle, "password", CONFIG_ESP_WIFI_PASSWORD));
            ESP_ERROR_CHECK(nvs_commit(my_handle));
            ESP_LOGI(TAG, "no password, give a init value to nvs");
        }
        else
        {
            ESP_LOGI(TAG, "stored password:%s", g_sys_param.password);
        }

        buf_len_long = sizeof(g_sys_param.key);
        err = nvs_get_str(my_handle, "ChatGPT_key", g_sys_param.key, &buf_len_long);
        if (err != ESP_OK || buf_len_long == 0)
        {
            ESP_ERROR_CHECK(nvs_set_str(my_handle, "ChatGPT_key", CONFIG_OPENAI_API_KEY));
            ESP_ERROR_CHECK(nvs_commit(my_handle));
            ESP_LOGI(TAG, "no ChatGPT key, give a init value to key");
        }
        else
        {
            ESP_LOGI(TAG, "stored ChatGPT key:%s", g_sys_param.key);
        }
    }
    nvs_close(my_handle);

    return ESP_OK == err ? ESP_OK : ESP_FAIL;
}

sys_param_t *settings_get_parameter(void)
{
    return &g_sys_param;
}
