#ifndef EEPROM_MAP_H
#define EEPROM_MAP_H

#define BOARD_ID_MAX_LENGTH 20
#define SSID_MAX_LENGTH 32
#define PASSWORD_MAX_LENGTH 32
#define NTP_MAX_LENGTH 32
#define IP_MAX_CHAR_LENGTH 16
#define WP_PROFILES_MAX 4

#define WP_NUMBER_LEN 20
#define WP_APIKEY_LEN 10

struct WP_Profile
{
    bool enabled;
    char number[WP_NUMBER_LEN];
    char apikey[WP_APIKEY_LEN];
};


struct EEPROM_Settings
{
    char board_id[BOARD_ID_MAX_LENGTH];
    uint8_t version;

    char wifi_ssid[SSID_MAX_LENGTH];
    char wifi_pass[PASSWORD_MAX_LENGTH];
    bool wifi_ap;
    bool wifi_dhcp;
    uint8_t wifi_ip[4];
    uint8_t wifi_gateway[4];
    uint8_t wifi_subnet[4];
    uint8_t wifi_dns[4];
    char wifi_ntp[NTP_MAX_LENGTH];

    WP_Profile wp_profiles[WP_PROFILES_MAX];
};

#endif