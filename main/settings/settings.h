/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Unlicense OR CC0-1.0
 */
#pragma once

#include "OpenAI.h"

#define DEFAULT_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define DEFAULT_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#define DEFAULT_OPENAI_KEY CONFIG_OPENAI_API_KEY

#define SSID_SIZE 32
#define PASSWORD_SIZE 64
#define KEY_SIZE 64

typedef struct
{
    char ssid[SSID_SIZE];         /* SSID of target AP. */
    char password[PASSWORD_SIZE]; /* Password of target AP. */
    char key[KEY_SIZE];           /* OpenAI key. */
} sys_param_t;

esp_err_t settings_nvs_init(void);
void uf2_nvs_storage_init(void);
esp_err_t settings_reset_parameter_to_nvs(void);
esp_err_t settings_read_parameter_from_nvs(void);
esp_err_t settings_write_parameter_to_nvs(void);
sys_param_t *settings_get_parameter(void);
void settings_set_parameter(sys_param_t *param);