/* inih -- unit tests

This works simply by dumping a bunch of info to standard output, which is
redirected to an output file (baseline_*.txt) and checked into the Subversion
repository. This baseline file is the test output, so the idea is to check it
once, and if it changes -- look at the diff and see which tests failed.

See unittest.bat and unittest.sh for how to run this (with tcc and gcc,
respectively).

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ini.h"

int User;
char Prev_section[50];

#if INI_HANDLER_LINENO
int dumper(void* user, const char* section, const char* name,
           const char* value, int lineno)
#else
int dumper(void* user, const char* section, const char* name,
           const char* value)
#endif
{
    User = *((int*)user);
    if (!name || strcmp(section, Prev_section)) {
        printf("... [%s]\n", section);
        strncpy(Prev_section, section, sizeof(Prev_section));
        Prev_section[sizeof(Prev_section) - 1] = '\0';
    }
    if (!name) {
        return 1;
    }

#if INI_HANDLER_LINENO
    printf("... %s%s%s;  line %d\n", name, value ? "=" : "", value ? value : "", lineno);
#else
    printf("... %s%s%s;\n", name, value ? "=" : "", value ? value : "");
#endif

    if (!value) {
        // Happens when INI_ALLOW_NO_VALUE=1 and line has no value (no '=' or ':')
        return 1;
    }

    return strcmp(name, "user")==0 && strcmp(value, "parse_error")==0 ? 0 : 1;
}

void parse(const char* fname) {
    static int u = 100;
    int e;

    *Prev_section = '\0';
    e = ini_parse(fname, dumper, &u);
    printf("%s: e=%d user=%d\n", fname, e, User);
    u++;
}

#ifdef CONFIG_IDF_TARGET
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spiffs.h"
#define BASE_PATH "/spiffs"
#define TAG "inih"

void spiffs_init(void) {
    esp_vfs_spiffs_conf_t conf = {
        .base_path = BASE_PATH,
        .partition_label = NULL,
        .max_files = 5,
        .format_if_mount_failed = false
    };

    // Use settings defined above to initialize and mount SPIFFS filesystem.
    // Note: esp_vfs_spiffs_register is an all-in-one convenience function.
    esp_err_t ret = esp_vfs_spiffs_register(&conf);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount or format filesystem");
        } else if (ret == ESP_ERR_NOT_FOUND) {
            ESP_LOGE(TAG, "Failed to find SPIFFS partition");
        } else {
            ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
        }
        return;
    }

    size_t total = 0, used = 0;
    ret = esp_spiffs_info(NULL, &total, &used);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s)", esp_err_to_name(ret));
    } else {
        ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
    }
}

void app_main(void)
#else
#define FOLDER_PATH
int main(void)
#endif
{
#ifdef CONFIG_IDF_TARGET
#define FOLDER_PATH BASE_PATH "/"
    spiffs_init();
#endif
    parse(FOLDER_PATH "no_file.ini");
    parse(FOLDER_PATH "normal.ini");
    parse(FOLDER_PATH "bad_section.ini");
    parse(FOLDER_PATH "bad_comment.ini");
    parse(FOLDER_PATH "user_error.ini");
    parse(FOLDER_PATH "multi_line.ini");
    parse(FOLDER_PATH "bad_multi.ini");
    parse(FOLDER_PATH "bom.ini");
    parse(FOLDER_PATH "duplicate_sections.ini");
    parse(FOLDER_PATH "no_value.ini");
#ifndef CONFIG_IDF_TARGET
    return 0;
#endif
}
