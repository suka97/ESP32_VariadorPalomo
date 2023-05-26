#include "global.h"

struct tm string2time(String time) {
    struct tm timeinfo;
    timeinfo.tm_hour = time.substring(0, 2).toInt();
    timeinfo.tm_min = time.substring(3, 5).toInt();
    return timeinfo;
}


String time2string(struct tm timeinfo) {
    char buffer[10];
    sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    return String(buffer);
}


void syncLocalTime() {
    while (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        lcd_print("Failed to", "obtain time");
        delay(1000);
    }
    Serial.println(time2string(timeinfo));
    lcd_print("Time:", time2string(timeinfo)); 
}


void fillIP(uint8_t *buffer, const String &ip) {
    IPAddress ip_addr;
    ip_addr.fromString(ip);
    buffer[0] = ip_addr[0];
    buffer[1] = ip_addr[1];
    buffer[2] = ip_addr[2];
    buffer[3] = ip_addr[3];
}


void fillIP(uint8_t *buffer, uint8_t ip1, uint8_t ip2, uint8_t ip3, uint8_t ip4) {
    buffer[0] = ip1;
    buffer[1] = ip2;
    buffer[2] = ip3;
    buffer[3] = ip4;
}


bool connectToWifi(const char* ssid, const char* password) {
    WiFi.begin(ssid, password);
    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 30) {
        retries++;
        Serial.print(".");
        delay(500);
    }
    return WiFi.status() == WL_CONNECTED;
}


String htmlProcessor(const String& var){
    if ( var == "curr_temp" ) return String(ds2820_temp);
    if ( var == "curr_mode" ) return (digitalRead(PIN_KEY_AUTO)==LVL_KEY_AUTO_PRESSED) ? String("AUTO") : String("MANUAL");
    if ( var == "curr_vel" ) return String(vf_pwm * 100 / PWM_MAX_DUTY);

    if ( var == "wifi_ssid" ) return settings.wifi_ssid;
    if ( var == "wifi_pass" ) return settings.wifi_pass;
    if ( var == "wifi_ap" ) return settings.wifi_ap ? "checked" : "";
    if ( var == "wifi_dhcp" ) return settings.wifi_dhcp ? "checked" : "";
    if ( var == "wifi_ip" ) return IPAddress(settings.wifi_ip).toString();
    if ( var == "wifi_gateway" ) return IPAddress(settings.wifi_gateway).toString();
    if ( var == "wifi_subnet" ) return IPAddress(settings.wifi_subnet).toString();
    if ( var == "wifi_dns" ) return IPAddress(settings.wifi_dns).toString();
    if ( var == "wifi_ntp" ) return settings.wifi_ntp;
    if ( var.startsWith("vf") ) return getVfHtml(var);
    return String();
}