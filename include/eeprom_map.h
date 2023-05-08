#ifndef EEPROM_MAP_H
#define EEPROM_MAP_H

#define BOARD_ID_MAX_LENGTH 10
#define SSID_MAX_LENGTH 32
#define PASSWORD_MAX_LENGTH 32
#define NTP_MAX_LENGTH 32
#define IP_MAX_CHAR_LENGTH 16

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
};

#endif