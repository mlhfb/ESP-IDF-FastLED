# ESP-IDF Usage Guide

## Goal

Use FastLED directly from native ESP-IDF projects while keeping FastLED timing behavior and API compatibility.

## Component Integration

This component builds FastLED's unity-build `.cpp` entrypoints from `src/` and exports `src` as public include path.

Key integration details:

- Defines `ESP32=1` for FastLED platform routing in legacy headers.
- Defines `F_CPU=(CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ*1000000UL)` for timing math.
- Maps `IDF_TARGET` to common Arduino-style target macros (`ESP32C3`, `ESP32S3`, etc.).
- Uses available ESP-IDF driver components conditionally (`esp_driver_rmt`, `esp_lcd`, `driver`, etc.).

## Build Flags You May Want

### RMT Driver Selection (ESP-IDF 5.x)

- `-DFASTLED_RMT5=1` (default if available): IDF5 path
- `-DFASTLED_RMT5=0`: legacy RMT4 path

If smoothness under Wi-Fi/BLE contention is inconsistent, test both.

### Optional Backend Selection

- `-DFASTLED_ESP32_I2S=1` for I2S parallel path
- `-DFASTLED_ESP32_USE_CLOCKLESS_SPI=1` for WS2812-over-SPI path

Default behavior is RMT where available.

## Recommended Render Loop Pattern

Use a fixed frame period and keep `show()` cadence predictable:

```cpp
const TickType_t frame = pdMS_TO_TICKS(16);  // ~60 FPS
TickType_t last = xTaskGetTickCount();

for (;;) {
    // update pixels
    FastLED.show();
    vTaskDelayUntil(&last, frame);
}
```

## Known Constraints

- `idf.py`/toolchain validation is required on real hardware for final timing quality.
- Different ESP32 families can prefer different backends under RF load.
- Very high UART/log output can still cause visible jitter in tight timing workloads.
