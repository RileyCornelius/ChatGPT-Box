set(UI_DIR ./ui)
file(GLOB_RECURSE UI_SRCS ${UI_DIR}/*.c)

set(APP_DIR ./app)
file(GLOB_RECURSE APP_SRCS ${APP_DIR}/*.c)

set(SETTINGS_DIR ./settings)
file(GLOB_RECURSE SETTINGS_SRCS ${SETTINGS_DIR}/*.c)

set(SERVICES_DIR ./services)
file(GLOB_RECURSE SERVICES_SRCS ${SERVICES_DIR}/*.c)

idf_component_register(
    SRCS
        "main.c"
        ${UI_SRCS}
        ${APP_SRCS}
        ${SETTINGS_SRCS}
        ${SERVICES_SRCS}
    INCLUDE_DIRS
        ${UI_DIR}
        ${APP_DIR}
        ${SETTINGS_DIR}
        ${SERVICES_DIR}
)

spiffs_create_partition_image(storage ../spiffs FLASH_IN_PROJECT)
