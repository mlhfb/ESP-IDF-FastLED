# ESP-IDF-FastLED

ESP-IDF-native packaging of [FastLED](https://github.com/FastLED/FastLED) for ESP32 targets, focused on smooth clockless output (RMT/I2S/SPI backends) without requiring the Arduino framework.

## What This Repository Contains

- Vendored FastLED source tree in `src/`
- ESP-IDF component build integration (`CMakeLists.txt`, `idf_component.yml`)
- Upstream FastLED platform porting guide (`PORTING.md`)
- ESP-IDF usage notes in [`docs/ESP_IDF_USAGE.md`](docs/ESP_IDF_USAGE.md)
- AI/automation maintainer guidance in [`claude.md`](claude.md)

## Upstream Baseline

- FastLED upstream: `https://github.com/FastLED/FastLED`
- Snapshot commit used for this port: `5d7ce6f`
- Sync helper: `scripts/sync_from_upstream.ps1`

## Install

### Option 1: As a local component (recommended)

1. Add this repository under your ESP-IDF project:
   - `components/fastled/`
2. In your app code:
   - `#include <FastLED.h>`

No Arduino component is required.

### Option 2: As a managed component

Add to your project's `idf_component.yml`:

```yaml
dependencies:
  mlhfb/esp-idf-fastled: "*"
```

## Minimal Example

```cpp
#include <FastLED.h>

constexpr int kDataPin = 2;
constexpr int kNumLeds = 60;
CRGB leds[kNumLeds];

extern "C" void app_main(void) {
    FastLED.addLeds<WS2812B, kDataPin, GRB>(leds, kNumLeds);
    FastLED.setBrightness(128);

    while (true) {
        static uint8_t hue = 0;
        fill_rainbow(leds, kNumLeds, hue++, 4);
        FastLED.show();
        vTaskDelay(pdMS_TO_TICKS(16));
    }
}
```

## Smoothness Notes

If your scroll/render still jitters under Wi-Fi/BLE load:

1. Prefer RMT backend (default on ESP32 targets).
2. On ESP-IDF 5.x, try both:
   - `-DFASTLED_RMT5=1` (default modern path)
   - `-DFASTLED_RMT5=0` (legacy RMT4 path, often steadier for some workloads)
3. Keep `FastLED.show()` cadence stable (fixed-timestep render loop).
4. Avoid long critical sections and verbose logging in real-time render paths.

More details: [`docs/ESP_IDF_USAGE.md`](docs/ESP_IDF_USAGE.md)

## License

This repository is MIT licensed. It includes FastLED code, also under MIT. See `LICENSE` and `NOTICE`.
