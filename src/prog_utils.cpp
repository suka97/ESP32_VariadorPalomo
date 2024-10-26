#include "global.h"

bool isTimeBetween(struct tm now, struct tm start, struct tm end) {
    if ( now.tm_hour < start.tm_hour ) return false;
    if ( now.tm_hour > end.tm_hour ) return false;
    if ( now.tm_hour == start.tm_hour && now.tm_min < start.tm_min ) return false;
    if ( now.tm_hour == end.tm_hour && now.tm_min > end.tm_min ) return false;
    return true;
}


String getWpHtml(const String& var) {
    int profile = var.substring(2, 3).toInt();
    if ( profile < 0 || profile >= WP_PROFILES_MAX ) return String();

    String param = var.substring(4); // skip "wpX-"
    if ( param == "enabled" ) return settings.wp_profiles[profile].enabled ? "checked" : "";
    if ( param == "number" ) return settings.wp_profiles[profile].number;
    if ( param == "apikey" ) return settings.wp_profiles[profile].apikey;
    return String();
}


void wp_getSettings(EEPROM_Settings& sett, AsyncWebServerRequest *request) {
    for ( uint8_t i=0 ; i<WP_PROFILES_MAX ; i++ ) {
        sett.wp_profiles[i].enabled = request->hasParam("wp" + String(i) + "-enabled");
        request->getParam("wp" + String(i) + "-number")->value().toCharArray(sett.wp_profiles[i].number, WP_NUMBER_LEN);
        request->getParam("wp" + String(i) + "-apikey")->value().toCharArray(sett.wp_profiles[i].apikey, WP_APIKEY_LEN);

        Serial.println("wp" + String(i) + ":");
        Serial.println("  enabled: " + String(sett.wp_profiles[i].enabled));
        Serial.println("  number: " + String(sett.wp_profiles[i].number));
        Serial.println("  apikey: " + String(sett.wp_profiles[i].apikey));
    }
}


void handleResetButton() {
    uint32_t reset_time = millis();
    uint32_t refresh_time = millis();
    while ( digitalRead(PIN_RESET) == LVL_RESET_PRESSED ) {
        if ( millis()-refresh_time > REFRESH_SCREEN ) {
            uint32_t time_left = (RESET_TIME - (millis()-reset_time)) / 1000;
            Serial.println("Reseting in " + String(time_left) + "s");
            lcd_print("Reseting in", String(time_left) + "s");
            refresh_time = millis();
        }
        if ( millis()-reset_time > RESET_TIME ) {
            Serial.println("Erasing EEPROM and restarting");
            lcd_print("Borrando EEPROM y reiniciando");
            EEPROM_CreateSettings();
            delay(1000);
            ESP.restart();
        }
    }
}