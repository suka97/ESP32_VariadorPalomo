#include "global.h"

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
    if ( var == "wifi_ssid" ) return settings.wifi_ssid;
    if ( var == "wifi_pass" ) return settings.wifi_pass;
    if ( var == "wifi_ap" ) return settings.wifi_ap ? "checked" : "";
    if ( var == "wifi_dhcp" ) return settings.wifi_dhcp ? "checked" : "";
    if ( var == "wifi_ip" ) return IPAddress(settings.wifi_ip).toString();
    if ( var == "wifi_gateway" ) return IPAddress(settings.wifi_gateway).toString();
    if ( var == "wifi_subnet" ) return IPAddress(settings.wifi_subnet).toString();
    if ( var == "wifi_dns" ) return IPAddress(settings.wifi_dns).toString();
    if ( var == "wifi_ntp" ) return settings.wifi_ntp;
    return String();
}


// void handle_setup() {
//   String s = "<html><body>";
//   s += "<form method='post'>";
//   s += "SSID: <input type='text' name='ssid'><br>";
//   s += "Password: <input type='password' name='password'><br>";
//   s += "<input type='submit' value='Submit'>";
//   s += "</form></body></html>";

//   server.send(200, "text/html", s);
//   if (server.method() == HTTP_POST) {
//     String ssid = server.arg("ssid");
//     String password = server.arg("password");

//     // Write SSID and password to EEPROM
//     EEPROM.begin(EEPROM_SIZE);
//     EEPROM.put(0, ssid);
//     EEPROM.put(SSID_MAX_LENGTH, password);
//     EEPROM.commit();
//     EEPROM.end();

//     server.sendHeader("Location", "/");
//     server.send(302);
//     delay(1000);
//     ESP.restart();
//   }
// }