//app_main.cpp
#include <tinyusb.h>
#include <tinyusb_default_config.h>
#include <XInput.h>

extern "C" {
    void app_main() {


        tinyusb_config_t TINYUSB_CONFIG = TINYUSB_DEFAULT_CONFIG();
        TINYUSB_CONFIG.descriptor.device = &XINPUT::DEVICE_DESC;
        TINYUSB_CONFIG.descriptor.full_speed_config = nullptr;
        TINYUSB_CONFIG.descriptor.string = nullptr;
        TINYUSB_CONFIG.descriptor.string_count = 0;
    }
}
