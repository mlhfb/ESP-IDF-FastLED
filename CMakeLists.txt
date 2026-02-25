cmake_minimum_required(VERSION 3.16)

if(NOT COMMAND idf_component_register)
    message(FATAL_ERROR "ESP-IDF-FastLED must be built inside an ESP-IDF project")
endif()

set(FASTLED_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src")

file(GLOB_RECURSE FASTLED_SRCS
    "${FASTLED_SRC_DIR}/*.cpp"
)

if(FASTLED_SRCS STREQUAL "")
    message(FATAL_ERROR "ESP-IDF-FastLED: no source files found in src/")
endif()

set(FASTLED_REQUIRES freertos esp_timer driver)
set(FASTLED_PRIV_REQUIRES)

set(FASTLED_IDF_COMPONENTS_DIR "")
if(DEFINED IDF_PATH)
    set(FASTLED_IDF_COMPONENTS_DIR "${IDF_PATH}/components")
elseif(DEFINED ENV{IDF_PATH})
    set(FASTLED_IDF_COMPONENTS_DIR "$ENV{IDF_PATH}/components")
endif()

foreach(COMP IN ITEMS
        driver
        esp_driver_gpio
        esp_driver_i2s
        esp_driver_rmt
        esp_driver_spi
        esp_driver_uart
        esp_adc
        led_strip
        esp_lcd
        esp_mm
        esp_netif
        esp_pm
        esp_wifi)
    # Use IDF component directory presence, not current build graph membership.
    # FastLED unity build includes optional ESP headers that need include paths
    # even if the app itself did not directly require those components.
    if(FASTLED_IDF_COMPONENTS_DIR AND EXISTS "${FASTLED_IDF_COMPONENTS_DIR}/${COMP}")
        list(APPEND FASTLED_PRIV_REQUIRES "${COMP}")
    endif()
endforeach()

idf_component_register(
    SRCS ${FASTLED_SRCS}
    INCLUDE_DIRS "src"
    REQUIRES ${FASTLED_REQUIRES}
    PRIV_REQUIRES ${FASTLED_PRIV_REQUIRES}
)

target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)

# ManCaveScroller (and similar ESP-IDF apps) compile with -Werror=all.
# Keep FastLED warnings visible but do not promote them to hard failures.
target_compile_options(${COMPONENT_LIB} PRIVATE -Wno-error)
