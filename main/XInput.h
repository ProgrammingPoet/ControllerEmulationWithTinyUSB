namespace XINPUT {
    tusb_desc_device_t const DEVICE_DESC = {
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
}

