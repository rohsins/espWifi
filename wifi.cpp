#include "wifi.h"

EventGroupHandle_t wifi_event_group;
constexpr char* TAGWIFI = "Wifi Library";
const int CONNECTED_BIT = BIT0;
static volatile bool ready = false;
static volatile bool connecting = false;

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
    strcpy(ssid, "WCS");
    strcpy(password, "RTSR&DWCS");
}

void Wifi::setSsid (const char * ssid)
{
    strcpy(this->ssid, ssid);
}

void Wifi::setPassword (const char * password)
{
    strcpy(this->password, password);
}

void setPassword (const char * password);

void Wifi::Init() {
    static wifi_config_t wifi_config;
    static wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

    // memcpy(wifi_config.sta.ssid, DefaultSSID, SSIDLENGTH);
    // memcpy(wifi_config.sta.password, DefaultPASSWORD, PASSWORDLENGTH);

    memcpy(wifi_config.sta.ssid, this->ssid, strlen(this->ssid));
    memcpy(wifi_config.sta.password, this->password, strlen(this->password));

    esp_event_loop_init(wifi_event_handler, NULL);
    esp_wifi_init(&cfg);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config);
}

void Wifi::Init(wifi_config_t WifiConfig, wifi_init_config_t InitConfig, esp_err_t (*handler)(void *ctx, system_event_t *event)) {
    esp_event_loop_init(handler, NULL);
    esp_wifi_init(&InitConfig);
    esp_wifi_set_storage(WIFI_STORAGE_RAM);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &WifiConfig);
}

void Wifi::Connect() {
    ESP_LOGI(TAGWIFI, "Connecting Wifi ...");
    esp_wifi_start();
    xEventGroupWaitBits(wifi_event_group, CONNECTED_BIT, false, true, portMAX_DELAY);
}

void Wifi::Disconnect() {
    ESP_LOGI(TAGWIFI, "Disconnecting Wifi ...");
    esp_wifi_stop();
}

Wifi::~Wifi() {
    // esp_wifi_stop();
}
