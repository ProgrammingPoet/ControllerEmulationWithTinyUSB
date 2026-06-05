//WiFi.h
void UDPServer(void* ARGUMENT) {
    std::array<uint8_t, 20> RX_BUFFER;
    sockaddr_in SERVER_ADDRESS{};
    SERVER_ADDRESS.sin_addr.s_addr = htonl(INADDR_ANY);
    SERVER_ADDRESS.sin_family = AF_INET;
    SERVER_ADDRESS.sin_port = htons(8080);
    int32_t SOCKET_ID = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
    bind(SOCKET_ID, (struct sockaddr *)&SERVER_ADDRESS, sizeof(SERVER_ADDRESS));
    ESP_LOGI("UDPServer", "Socket Bind Completed");

    while (true) {
        sockaddr_in SOURCE_ADDRESS{};
        socklen_t SOURCE_ADDRESS_LEN = sizeof(SOURCE_ADDRESS);
        recvfrom(SOCKET_ID, RX_BUFFER.data(), sizeof(RX_BUFFER), 0, (struct sockaddr *)&SOURCE_ADDRESS, &SOURCE_ADDRESS_LEN);
        
        XINPUT::REPORT* BUFFER = WRITE_BUFFER_PTR.load();
        BUFFER->BUTTONS1 = RX_BUFFER[0];
        BUFFER->BUTTONS2 = RX_BUFFER[1];
        BUFFER->LT = RX_BUFFER[2];
        BUFFER->RT = RX_BUFFER[3];
        BUFFER->LX = RX_BUFFER[4] | (RX_BUFFER[5] << 8);
        BUFFER->LY = RX_BUFFER[6] | (RX_BUFFER[7] << 8);
        BUFFER->RX = RX_BUFFER[8] | (RX_BUFFER[9] << 8);
        BUFFER->RY = RX_BUFFER[10] | (RX_BUFFER[11] << 8);
        
        XINPUT::REPORT* TEMP = READ_BUFFER_PTR.load();
        READ_BUFFER_PTR.store(WRITE_BUFFER_PTR.load());
        WRITE_BUFFER_PTR.store(TEMP);

        while (recvfrom(SOCKET_ID, RX_BUFFER.data(), sizeof(RX_BUFFER), MSG_DONTWAIT, (struct sockaddr *)&SOURCE_ADDRESS, &SOURCE_ADDRESS_LEN) >= 0) {
            continue;
        }
    }
}

void WiFiEventHandler(void* ARGUMENT, esp_event_base_t EVENT_BASE, int32_t EVENT_ID, void* EVENT_DATA) {
    if (EVENT_ID == WIFI_EVENT_AP_STACONNECTED) {
        ESP_LOGI("WiFiEventHandler", "Station Connected");
    } else if (EVENT_ID == WIFI_EVENT_AP_STADISCONNECTED) {
        ESP_LOGI("WiFiEventHandler", "Station Disconnected");
    }
}

void SetupWiFi() {

    esp_err_t NVS_ERROR = nvs_flash_init();
    if (NVS_ERROR == ESP_ERR_NVS_NO_FREE_PAGES || NVS_ERROR == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ESP_ERROR_CHECK(nvs_flash_init());
    }

    std::string_view SSID = "Controller";
    std::string_view PASSWORD = "Controller";

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();
    wifi_init_config_t WIFI_INIT_CONFIG = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&WIFI_INIT_CONFIG));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &WiFiEventHandler, nullptr, nullptr));
    wifi_config_t WIFI_AP_CONFIG{};
    std::copy(SSID.begin(), SSID.end(), WIFI_AP_CONFIG.ap.ssid);
    std::copy(PASSWORD.begin(), PASSWORD.end(), WIFI_AP_CONFIG.ap.password);
    WIFI_AP_CONFIG.ap.ssid_len = SSID.size();
    WIFI_AP_CONFIG.ap.channel = 1;
    WIFI_AP_CONFIG.ap.max_connection = 1;
    WIFI_AP_CONFIG.ap.authmode = WIFI_AUTH_WPA2_PSK;
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &WIFI_AP_CONFIG));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI("SetupWiFi", "WiFi Initialised");

    xTaskCreate(UDPServer, "UDP Server", 4096, nullptr, 5, nullptr);
}