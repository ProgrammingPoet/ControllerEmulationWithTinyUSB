//app_main.cpp
#include <cstdint>
#include <array>
#include <string>
#include <atomic>
#include <tinyusb.h>
#include <tinyusb_default_config.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <esp_wifi.h>
#include <lwip/sockets.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include <XInput.h>
#include <LED.h>
XINPUT::REPORT BUFFER_A{};
XINPUT::REPORT BUFFER_B{};
std::atomic<XINPUT::REPORT*> WRITE_BUFFER_PTR;
std::atomic<XINPUT::REPORT*> READ_BUFFER_PTR;
#include <WiFi.h>


enum class PLATFORMS {
    XINPUT
};

PLATFORMS MODE = PLATFORMS::XINPUT;



extern "C" void app_main() {
    ESP_LOGI("app_main", "Entered app_main");

    #if CONFIG_IDF_TARGET_ESP32S3
        ESP_ERROR_CHECK(led_strip_new_rmt_device(&RGB_LED_CONFIG, &RGB_LED_RMT_CONFIG, &RGB_LED));
        led_strip_clear(RGB_LED);
        led_strip_set_pixel(RGB_LED, 0, 0, 0, 0);
        led_strip_refresh(RGB_LED);
    #elif CONFIG_IDF_TARGET_ESP32S2
        gpio_reset_pin(GPIO_NUM_15);
        gpio_set_direction(GPIO_NUM_15, GPIO_MODE_OUTPUT);
        gpio_set_level(GPIO_NUM_15, 0);
    #endif

    #ifdef CONFIG_TINYUSB_VENDOR_COUNT
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
    ESP_LOGI("app_main", "TinyUSB Initialised");
    #endif

    WRITE_BUFFER_PTR.store(&BUFFER_A);
    READ_BUFFER_PTR.store(&BUFFER_B);

    SetupWiFi();

    #ifdef CONFIG_TINYUSB_VENDOR_COUNT
    while (true){
        if (tud_vendor_mounted()) {
            static uint32_t USB_BUFFER_SIZE = 0;
            switch (MODE) {
                case PLATFORMS::XINPUT:
                    if (tud_vendor_write_available() >= USB_BUFFER_SIZE) {
                        if (USB_BUFFER_SIZE < tud_vendor_write_available()) {
                            USB_BUFFER_SIZE = tud_vendor_write_available();
                        }
                        tud_vendor_write(READ_BUFFER_PTR, sizeof(XINPUT::REPORT));
                        tud_vendor_flush();
                    }
                    break;
            }
        } 
        else {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    #endif
}
