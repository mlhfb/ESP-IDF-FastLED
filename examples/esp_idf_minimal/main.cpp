#include <FastLED.h>

constexpr int kDataPin = 2;
constexpr int kNumLeds = 60;
CRGB leds[kNumLeds];

extern "C" void app_main(void) {
    FastLED.addLeds<WS2812B, kDataPin, GRB>(leds, kNumLeds);
    FastLED.setBrightness(128);

    while (true) {
        static uint8_t hue = 0;
        fill_rainbow(leds, kNumLeds, hue++, 5);
        FastLED.show();
        vTaskDelay(pdMS_TO_TICKS(16));
    }
}
