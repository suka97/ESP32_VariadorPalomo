#include <global.h>

const char* BOARD_ID = "PalomoVF";
const uint8_t VERSION = 5;

const char* AP_SSID = "ESP32_AP";
const char* AP_PWD = "12345678";

EEPROM_Settings settings;
AsyncWebServer server(80);
struct tm timeinfo;


void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // EEPROM Settings init
    EEPROM_Begin();
    EEPROM_ReadSettings();
    Serial.println("Board ID: " + String(settings.board_id) + ", version: " + String(settings.version));
    if( strcmp(settings.board_id, BOARD_ID) != 0 || settings.version != VERSION ) {
        Serial.println("Settings not found or invalid. Creating new settings...");
        EEPROM_CreateSettings();
        EEPROM_ReadSettings();
    }

    // Wifi init
    if( settings.wifi_ap ) {
        Serial.println("Starting AP...");
        WiFi.softAP(AP_SSID, AP_PWD);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
    } else {
        Serial.println("WIFI_SSID: " + String(settings.wifi_ssid));
        if ( !settings.wifi_dhcp ) {
            IPAddress local_ip(settings.wifi_ip);
            IPAddress gateway(settings.wifi_gateway);
            IPAddress subnet(settings.wifi_subnet);
            Serial.println("WIFI_IP: " + local_ip.toString());
            Serial.println("WIFI_GATEWAY: " + gateway.toString());
            Serial.println("WIFI_SUBNET: " + subnet.toString());
            if ( !WiFi.config(local_ip, gateway, subnet) ) {
                Serial.println("Failed to configure static IP");
            }
        }
        Serial.println("Connecting to WiFi...");
        bool connected = connectToWifi(settings.wifi_ssid, settings.wifi_pass);
        if (connected) {
            Serial.println("Connected to WiFi with IP address: " + WiFi.localIP().toString());
        } else {
            Serial.println("Failed to connect to WiFi.");
        }
    }

    // NTP init
    Serial.println("NTP server: " + String(settings.wifi_ntp));
    configTime(-10800, 0, settings.wifi_ntp);
    syncLocalTime();

    // Webserver init
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, htmlProcessor);
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
        EEPROM_Settings new_sett;
            strcpy(new_sett.board_id, settings.board_id);
            new_sett.version = settings.version;

        request->getParam("wifi_ssid")->value().toCharArray(new_sett.wifi_ssid, SSID_MAX_LENGTH);
        request->getParam("wifi_pass")->value().toCharArray(new_sett.wifi_pass, PASSWORD_MAX_LENGTH);
        new_sett.wifi_ap = request->hasParam("wifi_ap");
        new_sett.wifi_dhcp = request->hasParam("wifi_dhcp");
        fillIP(new_sett.wifi_ip, request->getParam("wifi_ip")->value());
        fillIP(new_sett.wifi_gateway, request->getParam("wifi_gateway")->value());
        fillIP(new_sett.wifi_subnet, request->getParam("wifi_subnet")->value());
        fillIP(new_sett.wifi_dns, request->getParam("wifi_dns")->value());
        request->getParam("wifi_ntp")->value().toCharArray(new_sett.wifi_ntp, NTP_MAX_LENGTH);

        Serial.println("New settings:");
        Serial.println("wifi_ssid: " + String(new_sett.wifi_ssid));
        Serial.println("wifi_pass: " + String(new_sett.wifi_pass));
        Serial.println("wifi_ap: " + String(new_sett.wifi_ap));
        Serial.println("wifi_dhcp: " + String(new_sett.wifi_dhcp));
        Serial.println("wifi_ip: " + IPAddress(new_sett.wifi_ip).toString());
        Serial.println("wifi_gateway: " + IPAddress(new_sett.wifi_gateway).toString());
        Serial.println("wifi_subnet: " + IPAddress(new_sett.wifi_subnet).toString());
        Serial.println("wifi_dns: " + IPAddress(new_sett.wifi_dns).toString());
        Serial.println("wifi_ntp: " + String(new_sett.wifi_ntp));

        vf_getSettings(new_sett, request);

        request->send(SPIFFS, "/reset.html", String(), false, htmlProcessor);
        Serial.println("Saving new settings...");
        EEPROM_WriteSettings(new_sett);
        Serial.println("Restarting...");
        delay(1000);
        ESP.restart();
    });
    server.begin();
}



void loop() {
    getLocalTime(&timeinfo);

    // int prof_index = getProfileForTime(settings.vf_profiles, timeinfo);
    // if ( prof_index == -1 ) {
    //     Serial.println("No profile found for current time");
    //     delay(1000);
    //     return;
    // }

    // Serial.println("Profile found: " + String(prof_index));
    // delay(1000);
}
