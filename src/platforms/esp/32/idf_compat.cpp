#if defined(ESP_PLATFORM)

#include "fl/int.h"

extern "C" {
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rom/ets_sys.h"
}

namespace fl {
namespace platforms {

void delayMicroseconds(fl::u32 us) {
    if (us == 0) {
        return;
    }

    // Yield to scheduler for long delays, then finish with microsecond delay.
    if (us >= 1000) {
        const TickType_t ticks = pdMS_TO_TICKS(us / 1000);
        if (ticks > 0) {
            vTaskDelay(ticks);
        }
        us %= 1000;
    }
    if (us > 0) {
        ets_delay_us(us);
    }
}

fl::u32 millis() {
    return static_cast<fl::u32>(esp_timer_get_time() / 1000ULL);
}

fl::u32 micros() {
    return static_cast<fl::u32>(esp_timer_get_time());
}

void println(const char* /*str*/) {
    // Keep ESP-IDF builds lean: default println is a no-op.
}

void initChannelDrivers() {
    // The ESP-IDF component currently targets classic FastLED controller usage.
    // Channel-manager backends are intentionally not initialized here.
}

}  // namespace platforms
}  // namespace fl

#endif  // ESP_PLATFORM
