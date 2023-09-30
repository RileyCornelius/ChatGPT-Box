# esp-inih (INI Not Invented Here)

[![Tests](https://github.com/leeebo/esp-inih/actions/workflows/tests.yml/badge.svg)](https://github.com/leeebo/esp-inih/actions/workflows/tests.yml)

**This is a ESP-IDF component warpped version of [inih](https://github.com/benhoyt/inih), which can be directly used in ESP-IDF projects.**

**inih (INI Not Invented Here)** is a simple [.INI file](http://en.wikipedia.org/wiki/INI_file) parser written in C. It's only a couple of pages of code, and it was designed to be _small and simple_, so it's good for embedded systems. It's also more or less compatible with Python's [ConfigParser](http://docs.python.org/library/configparser.html) style of .INI files, including RFC 822-style multi-line syntax and `name: value` entries.

To use it, just give `ini_parse()` an INI file, and it will call a callback for every `name=value` pair parsed, giving you strings for the section, name, and value. It's done this way ("SAX style") because it works well on low-memory embedded systems, but also because it makes for a KISS implementation.

You can also call `ini_parse_file()` to parse directly from a `FILE*` object, `ini_parse_string()` to parse data from a string, or `ini_parse_stream()` to parse using a custom fgets-style reader function for custom I/O.

## Compile-time options

You can using `idf.py menuconfig` to configure all the compile-time options. the options are is decribed in [compile-time-options](https://github.com/benhoyt/inih#compile-time-options)

## Simple example in C

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ini.h"

typedef struct
{
    int version;
    const char* name;
    const char* email;
} configuration;

static int handler(void* user, const char* section, const char* name,
                   const char* value)
{
    configuration* pconfig = (configuration*)user;

    #define MATCH(s, n) strcmp(section, s) == 0 && strcmp(name, n) == 0
    if (MATCH("protocol", "version")) {
        pconfig->version = atoi(value);
    } else if (MATCH("user", "name")) {
        pconfig->name = strdup(value);
    } else if (MATCH("user", "email")) {
        pconfig->email = strdup(value);
    } else {
        return 0;  /* unknown section/name, error */
    }
    return 1;
}

void app_main(void)
{
    configuration config;

    if (ini_parse("test.ini", handler, &config) < 0) {
        printf("Can't load 'test.ini'\n");
        return 1;
    }
    printf("Config loaded from 'test.ini': version=%d, name=%s, email=%s\n",
        config.version, config.name, config.email);
    return 0;
}
```

## Related links

* [inih (INI Not Invented Here)](https://github.com/benhoyt/inih)
