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

set(FASTLED_REQUIRES freertos esp_timer)
set(FASTLED_PRIV_REQUIRES)

idf_build_get_property(FASTLED_ALL_COMPONENTS COMPONENTS)

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
    if("${COMP}" IN_LIST FASTLED_ALL_COMPONENTS)
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
