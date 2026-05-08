//app_main.cpp
#include <cstdint>
#include <array>
#include <string>
#include <tinyusb.h>
#include <tinyusb_default_config.h>
#include <esp_log.h>
#include <led_strip.h>
#include <XInput.h>

constexpr uint8_t RGB_LED_BRIGHTNESS = 10;

led_strip_handle_t RGB_LED;
led_strip_config_t RGB_LED_CONFIG = {
    .strip_gpio_num = 48,
    .max_leds = 1, // at least one LED on board
    .led_model = LED_MODEL_WS2812,
    .color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
    .flags = 0
};
led_strip_rmt_config_t RGB_LED_RMT_CONFIG = {
    .clk_src = RMT_CLK_SRC_DEFAULT,
    .resolution_hz = 10 * 1000 * 1000, // 10MHz
    .mem_block_symbols = 0, // 0 = Default
    .flags{.with_dma = false}
};

enum class PLATFORMS {
    XINPUT
};

PLATFORMS MODE = PLATFORMS::XINPUT;

extern "C" void app_main() {

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&RGB_LED_CONFIG, &RGB_LED_RMT_CONFIG, &RGB_LED));
    led_strip_clear(RGB_LED);
    led_strip_set_pixel(RGB_LED, 0, 1*RGB_LED_BRIGHTNESS, 0, 0);
    led_strip_refresh(RGB_LED);

    tinyusb_config_t TINYUSB_CONFIG = TINYUSB_DEFAULT_CONFIG();
    switch (MODE) {
        case PLATFORMS::XINPUT:
            TINYUSB_CONFIG.descriptor.device = &XINPUT::DEVICE_DESC;
            TINYUSB_CONFIG.descriptor.full_speed_config = XINPUT::CONFIG_DESC.data();
            TINYUSB_CONFIG.descriptor.string = const_cast<const char**>(XINPUT::STRING_DESC.data()); //const (const char*)* to (const char*)*
            TINYUSB_CONFIG.descriptor.string_count = XINPUT::STRING_DESC.size();
            break;
    }
    ESP_ERROR_CHECK(tinyusb_driver_install(&TINYUSB_CONFIG));


    while (true){
        if (tud_vendor_mounted()) {
            switch (MODE) {
                case PLATFORMS::XINPUT:
                    led_strip_set_pixel(RGB_LED, 0, 0, 1*RGB_LED_BRIGHTNESS, 0);
                    led_strip_refresh(RGB_LED);
                    if (tud_vendor_write_available() >= sizeof(XINPUT::REPORT)) {
                        if (XINPUT::REPORT.BUTTONS2 == 0x00) {XINPUT::REPORT.BUTTONS2 = 0x10;}
                        else if (XINPUT::REPORT.BUTTONS2 == 0x10) {XINPUT::REPORT.BUTTONS2 = 0x00;}
                        tud_vendor_write(&XINPUT::REPORT, sizeof(XINPUT::REPORT));
                        tud_vendor_flush();
                        vTaskDelay(pdMS_TO_TICKS(500));
                    }
                    else {
                        vTaskDelay(pdMS_TO_TICKS(100));
                    }
                break;
            }
        } 
        else {
            led_strip_set_pixel(RGB_LED, 0, 1*RGB_LED_BRIGHTNESS, 0, 0);
            led_strip_refresh(RGB_LED);
            ESP_LOGI("app_main", "Device Not Mounted!");
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

}
