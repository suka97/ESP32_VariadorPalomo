#include <global.h>

const uint32_t REFRESH_DELAY = 1000;
const uint32_t RESET_TIME = 10000;

const char* BOARD_ID = "PalomoVF";
const uint8_t VERSION = 5;

const char* AP_SSID = "ESP32_AP";
const char* AP_PWD = "12345678";

EEPROM_Settings settings;
AsyncWebServer server(80);
struct tm timeinfo;

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
uint8_t screen_id = 0;

OneWire oneWire(PIN_DS2820);
DallasTemperature ds2820(&oneWire);
float ds2820_temp = 0;

int vf_profile = -1;
uint32_t vf_pwm = 0;


void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    // IO init
    pinMode(PIN_RESET, INPUT_PULLUP);
    // PWM init
    ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
    ledcAttachPin(PIN_VF_V0, PWM_CHANNEL);

    // LCD init
    lcd.begin(16, 2);
    lcd_print("Suka"); delay(PRINT_DELAY);

    // DS2820 init
    ds2820.begin();
    ds2820_temp = getTemp();
    if ( ds2820_temp == DEVICE_DISCONNECTED_C ) {
        Serial.println("DS2820 not found");
        lcd_print("Error sensor", "temperatura"); delay(PRINT_DELAY);
    }

    // EEPROM Settings init
    EEPROM_Begin();
    EEPROM_ReadSettings();
    Serial.println("Board ID: " + String(settings.board_id) + ", version: " + String(settings.version));
    if( strcmp(settings.board_id, BOARD_ID) != 0 || settings.version != VERSION ) {
        Serial.println("Settings not found or invalid. Creating new settings...");
        lcd_print("Creating", "New settings..."); delay(PRINT_DELAY);
        EEPROM_CreateSettings();
        EEPROM_ReadSettings();
    }

    // Initialize SPIFFS
    if(!SPIFFS.begin(true)){
        Serial.println("An Error has occurred while mounting SPIFFS");
        lcd_print("SPIFFS error"); delay(PRINT_DELAY);
        return;
    }

    // Wifi init
    if( settings.wifi_ap ) {
        Serial.println("Starting AP...");
        lcd_print("Starting", "AP...");
        WiFi.softAP(AP_SSID, AP_PWD);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
        lcd_print("AP IP address:", IP.toString()); delay(PRINT_DELAY);
    } else {
        Serial.println("WIFI_SSID: " + String(settings.wifi_ssid));
        lcd_print("Wifi:", settings.wifi_ssid); delay(PRINT_DELAY);
        if ( !settings.wifi_dhcp ) {
            IPAddress local_ip(settings.wifi_ip);
            IPAddress gateway(settings.wifi_gateway);
            IPAddress subnet(settings.wifi_subnet);
            Serial.println("WIFI_IP: " + local_ip.toString());
            Serial.println("WIFI_GATEWAY: " + gateway.toString());
            Serial.println("WIFI_SUBNET: " + subnet.toString());
            lcd_print("IP Estatica:", local_ip.toString()); delay(PRINT_DELAY);
            if ( !WiFi.config(local_ip, gateway, subnet) ) {
                Serial.println("Failed to configure static IP");
            }
        }
        Serial.println("Connecting to WiFi...");
        lcd_print("Connecting", "to WiFi...");
        bool connected = connectToWifi(settings.wifi_ssid, settings.wifi_pass);
        if (connected) {
            Serial.println("Connected to WiFi with IP address: " + WiFi.localIP().toString());
            lcd_print("Connected", WiFi.localIP().toString()); delay(PRINT_DELAY);
        } else {
            Serial.println("Failed to connect to WiFi.");
        }
    }

    // NTP init
    Serial.println("NTP server: " + String(settings.wifi_ntp));
    lcd_print("NTP server:", settings.wifi_ntp); delay(PRINT_DELAY);
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
        lcd_print("Restarting...");
        delay(1000);
        ESP.restart();
    });
    server.begin();
}



void loop() {
    handleResetButton();
    getLocalTime(&timeinfo);
    ds2820_temp = getTemp();

    int prof_index = getProfileForTime(settings.vf_profiles, timeinfo);
    if ( prof_index != vf_profile ) {
        vf_profile = prof_index;
        Serial.println("Profile Triggered: " + String(vf_profile));
    }

    handleVfProfile();
    lcd_screen1(); 
    delay(REFRESH_DELAY);
}
