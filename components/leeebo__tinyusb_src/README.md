## TinyUSB Core Code for ESP32Sx

[![Push component to https://components.espressif.com](https://github.com/leeebo/tinyusb_src/actions/workflows/sync_esp_pkgmng.yml/badge.svg?branch=master)](https://github.com/leeebo/tinyusb_src/actions/workflows/sync_esp_pkgmng.yml)

The Core code of TinyUSB as ESP-IDF component, users can use the TinyUSB native API for project development based on ESP32Sx.

For more information about TinyUSB, please refer https://docs.tinyusb.org

## Version

|Component Version|TinyUSB Base Commit|
|--|--|
|0.0.2| Oct 25, 2022 [28f49c088bb](https://github.com/hathach/tinyusb/commit/28f49c088bb0c498d730d80943017172061cfd05)|
|0.0.3| Jun 21, 2023 [6cf735031f3](https://github.com/hathach/tinyusb/commit/6cf735031f35cd223231b7f94b8c3caa8286cb9e)|
|0.0.4| Jun 21, 2023 [6cf735031f3](https://github.com/hathach/tinyusb/commit/6cf735031f35cd223231b7f94b8c3caa8286cb9e)|

## Feature

1. Choose between `dcd_esp32sx` or `dcd_dwc2` through Kconfig
2. Include below device drivers by default
   1. audio
   2. bth
   3. cdc
   4. dfu
   5. hid
   6. midi
   7. msc
   8. net
   9. usbtmc
   10. vendor
   11. video

## Develop Guide

Like other native examples from TinyUSB repository, users need to add a configuration file `tusb_config.h` to the project and make it visible to the TinyUSB component. please refer:

```cmake
idf_component_get_property(tusb_lib leeebo__tinyusb_src COMPONENT_LIB)
target_include_directories(${tusb_lib} PRIVATE include)
```