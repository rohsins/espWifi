#include "wifi.h"

#define DefaultSSID (char *) "WCS"
#define SSIDLENGTH 3
#define DefaultPASSWORD (char *) "RTSR&DWCS"
#define PASSWORDLENGTH 9

EventGroupHandle_t wifi_event_group;
constexpr char* TAGWIFI = "WifiTAG";
const int CONNECTED_BIT = BIT0;

esp_err_t wifi_event_handler(void *ctx, system_event_t *event) {
    switch (event->event_id) {
        case SYSTEM_EVENT_STA_START:
            esp_wifi_connect();
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            xEventGroupSetBits(wifi_event_group, CONNECTED_BIT);
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            esp_wifi_connect();
            xEventGroupClearBits(wifi_event_group, CONNECTED_BIT);
            break;
        default:
            break;
    }
    return ESP_OK;
}

Wifi::Wifi() {
    tcpip_adapter_init();
    wifi_event_group = xEventGroupCreate();
}

void Wifi::Init() {
    static wifi_config_t wifi_config;
    static wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    memcpy(wifi_config.sta.ssid, DefaultSSID, SSIDLENGTH);
    memcpy(wifi_config.sta.password, DefaultPASSWORD, PASSWORDLENGTH);

    esp_event_loop_init(wifi_event_handler, NULL);
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
    esp_wifi_start();
    ESP_LOGI(TAGWIFI, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

void Wifi::Init(wifi_config_t WifiConfig, wifi_init_config_t InitConfig, esp_err_t (*handler)(void *ctx, system_event_t *event)) {
    esp_event_loop_init(handler, NULL);
    esp_wifi_init(&InitConfig);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &WifiConfig);
    esp_wifi_start();
    ESP_LOGI(TAGWIFI, "Waiting for wifi");
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}
