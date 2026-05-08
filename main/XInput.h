const char LANG_ID[] = { 0x09, 0x04 };

namespace XINPUT {
    const tusb_desc_device_t DEVICE_DESC = {
    .bLength            = sizeof(tusb_desc_device_t),
    .bDescriptorType    = TUSB_DESC_DEVICE,          
    .bcdUSB             = 0x0200,                    
    .bDeviceClass       = 0xFF,                      
    .bDeviceSubClass    = 0xFF,
    .bDeviceProtocol    = 0xFF,
    .bMaxPacketSize0    = CFG_TUD_ENDPOINT0_SIZE,    
    .idVendor           = 0x045E,                    
    .idProduct          = 0x028E,                    
    .bcdDevice          = 0x0114,                   
    .iManufacturer      = 0x01,                       
    .iProduct           = 0x02,                     
    .iSerialNumber      = 0x03,                      
    .bNumConfigurations = 0x01                    
    };
    const std::array<uint8_t, 49> CONFIG_DESC = {
            // --- Configuration Descriptor Header ---
        0x09,           // bLength
        0x02,           // bDescriptorType (Configuration)
        0x31, 0x00,     // wTotalLength (49 bytes: 0x31) -> REPLACED 0x99
        0x01,           // bNumInterfaces (We only have 1 interface now) -> REPLACED 0x04
        0x01,           // bConfigurationValue
        0x00,           // iConfiguration (String Index)
        0xA0,           // bmAttributes (Bus-powered, Remote-Wakeup)
        0xFA,           // bMaxPower (500mA)

        // --- Interface 0: Controller Data ---
        0x09,           // bLength
        0x04,           // bDescriptorType (Interface)
        0x00,           // bInterfaceNumber
        0x00,           // bAlternateSetting
        0x02,           // bNumEndpoints
        0xFF,           // bInterfaceClass (Vendor Specific)
        0x5D,           // bInterfaceSubClass (XInput)
        0x01,           // bInterfaceProtocol (XInput)
        0x00,           // iInterface

        // --- XInput Specific "Unknown" Header (Descriptor 0x21) ---
        0x11, 0x21, 0x00, 0x01, 0x01, 0x25, 0x81, 0x14, 0x00, 
        0x00, 0x00, 0x00, 0x13, 0x01, 0x08, 0x00, 0x00,

        // --- Endpoint 1: IN (Device to PC) ---
        0x07, 0x05, 0x81, 0x03, 0x20, 0x00, 0x04,

        // --- Endpoint 1: OUT (PC to Device - Rumble) ---
        0x07, 0x05, 0x01, 0x03, 0x20, 0x00, 0x08
    };
    const std::array<const char*, 5> STRING_DESC = {
        &(LANG_ID[0]),
        static_cast<const char*>("Microsoft Corporation"),
        static_cast<const char*>("Controller"),
        static_cast<const char*>("04B229A"),
        static_cast<const char*>("Xbox Security Method 3, Version 1.00, \xC2\xA9 2005 Microsoft Corporation. All rights reserved.")
    };
}

