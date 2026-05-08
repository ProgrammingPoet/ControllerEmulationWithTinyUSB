//app_main.cpp
#include <cstdint>
#include <array>
#include <string>
#include <tinyusb.h>
#include <tinyusb_default_config.h>
#include <XInput.h>

extern "C" {
    void app_main() {

        tinyusb_config_t TINYUSB_CONFIG = TINYUSB_DEFAULT_CONFIG();
        TINYUSB_CONFIG.descriptor.device = &XINPUT::DEVICE_DESC;
        TINYUSB_CONFIG.descriptor.full_speed_config = XINPUT::CONFIG_DESC.data();
        TINYUSB_CONFIG.descriptor.string = const_cast<const char**>(XINPUT::STRING_DESC.data()); //const (const char*)* to (const char*)*
        TINYUSB_CONFIG.descriptor.string_count = XINPUT::STRING_DESC.size();

    }
}
