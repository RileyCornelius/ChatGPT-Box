idf_component_register(SRCS "src/tusb.c" "src/common/tusb_fifo.c"
                    INCLUDE_DIRS "." "src"
                    REQUIRES freertos soc)

set(DCD_DRIVER_SRC "")

if(CONFIG_USING_SYNOPSYS_DWC2_DRIVER)
  set(DCD_DRIVER_SRC espressif/esp32sx/dcd_esp32sx.c)
else()
  set(DCD_DRIVER_SRC synopsys/dwc2/dcd_dwc2.c)
endif()

target_sources(${COMPONENT_LIB} PRIVATE
  "src/device/usbd.c"
  "src/device/usbd_control.c"
  "src/class/audio/audio_device.c"
  "src/class/bth/bth_device.c"
  "src/class/cdc/cdc_device.c"
  "src/class/dfu/dfu_device.c"
  "src/class/dfu/dfu_rt_device.c"
  "src/class/hid/hid_device.c"
  "src/class/midi/midi_device.c"
  "src/class/msc/msc_device.c"
  "src/class/net/ncm_device.c"
  "src/class/net/ecm_rndis_device.c"
  "src/class/usbtmc/usbtmc_device.c"
  "src/class/vendor/vendor_device.c"
  "src/class/video/video_device.c"
  "src/portable/${DCD_DRIVER_SRC}"
)

target_compile_options(${COMPONENT_LIB} PUBLIC
  "-Wno-format"
  "-DCFG_TUSB_OS=OPT_OS_FREERTOS"
)

if(IDF_TARGET STREQUAL "esp32s2")
  target_compile_options(${COMPONENT_LIB} PUBLIC
    "-DCFG_TUSB_MCU=OPT_MCU_ESP32S2")
elseif(IDF_TARGET STREQUAL "esp32s3")
  target_compile_options(${COMPONENT_LIB} PUBLIC
    "-DCFG_TUSB_MCU=OPT_MCU_ESP32S3")
else()
  message(FATAL_ERROR "${IDF_TARGET} not supported by tinyusb")
endif()