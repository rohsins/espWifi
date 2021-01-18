#ifndef _wifi_h_
#define _wifi_h_

#include <iostream>
#include <cstdint>
#include <cstring>
#include "esp_system.h"
#include "esp_wifi.h"

#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_event_loop.h"
#include "esp_log.h"


class Wifi {
    private:
        char ssid[32] = { 0 };
        char password[32] = { 0 };
    public:
        Wifi();
        void setSsid (const char * ssid);
        void setPassword (const char * password);
        void Init();
        void Init(wifi_config_t WifiConfig, wifi_init_config_t InitConfig, esp_err_t (*handler)(void *ctx, system_event_t *event));
        void Connect();
        void Disconnect();
        ~Wifi();
};

#endif