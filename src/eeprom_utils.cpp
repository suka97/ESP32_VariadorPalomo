#include "global.h"

void EEPROM_Clear() {
    for (unsigned int i = 0 ; i < EEPROM.length() ; i++)
        EEPROM.write(i, 0);
    EEPROM.commit();
}


void EEPROM_Write(char *buffer, uint8_t size) {
    for ( uint8_t i=0 ; i<size ; i++ )
        EEPROM.put ( i, buffer[i] ); 
    EEPROM.commit();
}

void EEPROM_WriteSettings(EEPROM_Settings settings) {
    EEPROM.put(0, settings);
    EEPROM.commit();
}

void EEPROM_ReadSettings() {
    EEPROM.get(0, settings);
}

void EEPROM_Begin() {
    EEPROM.begin(EEPROM_SIZE);
}

void EEPROM_CreateSettings() {
    strcpy(settings.board_id, BOARD_ID);
    settings.version = VERSION;
    strcpy(settings.wifi_ssid, "");
    strcpy(settings.wifi_pass, "");
    settings.wifi_ap = true;
    settings.wifi_dhcp = true;
    fillIP(settings.wifi_ip, 192, 168, 1, 1);
    fillIP(settings.wifi_gateway, 0, 0, 0, 0);
    fillIP(settings.wifi_subnet, 255, 255, 255, 0);
    fillIP(settings.wifi_dns, 8, 8, 8, 8);
    strcpy(settings.wifi_ntp, "time.google.com");
    EEPROM_WriteSettings(settings);
}