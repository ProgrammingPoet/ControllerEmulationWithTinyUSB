#if CONFIG_IDF_TARGET_ESP32S3
    #include <led_strip.h>
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
#elif CONFIG_IDF_TARGET_ESP32S2
    #include <driver/gpio.h>
#endif

void ToggleLED() {
    static bool LED_ON = false;
    if (LED_ON) {
        #if CONFIG_IDF_TARGET_ESP32S3
            led_strip_set_pixel(RGB_LED, 0, 0, RGB_LED_BRIGHTNESS, 0);
            led_strip_refresh(RGB_LED);
        #elif CONFIG_IDF_TARGET_ESP32S2
            gpio_set_level(GPIO_NUM_15, 1);
        #endif
    } else if (!LED_ON) {
        #if CONFIG_IDF_TARGET_ESP32S3
            led_strip_set_pixel(RGB_LED, 0, 0, 0, 0);
            led_strip_refresh(RGB_LED);
        #elif CONFIG_IDF_TARGET_ESP32S2
            gpio_set_level(GPIO_NUM_15, 0);
        #endif
    }
}