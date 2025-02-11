#include <global.h>

const uint32_t REFRESH_TIME = 1000;
const uint32_t REFRESH_SCREEN = 100;
const uint32_t RESET_TIME = 10000;
const uint32_t NOWIFI_REBOOT_DELAY = 60000*1;   // 1 min

const char* BOARD_ID = "WifiDoorbell";
const uint8_t VERSION = 3;

const char* AP_SSID = "WifiDoorbell";
const char* AP_PWD = "12345678";

EEPROM_Settings settings;
AsyncWebServer server(80);
struct tm timeinfo;
bool wifi_connected = false;

LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);
uint8_t screen_id = 0;


void setup() {
    Serial.begin(115200);
    Serial.println("Starting...");

    // IO init
    pinMode(PIN_RESET, INPUT_PULLUP);
    pinMode(PIN_IN_DOORBELL, INPUT_PULLUP);
    pinMode(PIN_LED, OUTPUT); digitalWrite(PIN_LED, !LVL_LED_ON);

    // LCD init
    lcd.begin(16, 2);
    lcd_print("Suka"); delay(PRINT_DELAY);

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

    handleResetButton();

    // Wifi init
    bool connection_error = false;
    if( !settings.wifi_ap ) {
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
        digitalWrite(PIN_LED, (connected) ? LVL_LED_ON : !LVL_LED_ON);
        if (connected) {
            wifi_connected = true;
            Serial.println("Connected to WiFi with IP address: " + WiFi.localIP().toString());
            lcd_print("Connected", WiFi.localIP().toString()); delay(5*PRINT_DELAY);
        } else {
            Serial.println("Failed to connect to WiFi");
            lcd_print("Failed to", "connect to WiFi"); delay(PRINT_DELAY*2);
            connection_error = true;
        }
    }
    // Wifi AP
    if ( !wifi_connected ) {
        Serial.println("Starting AP...");
        lcd_print("Starting", "AP...");
        WiFi.softAP(AP_SSID, AP_PWD);
        IPAddress IP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(IP);
        lcd_print("AP IP address:", IP.toString()); delay(PRINT_DELAY);
    } 

    // NTP init
    // if ( wifi_connected ) {
    //     Serial.println("NTP server: " + String(settings.wifi_ntp));
    //     lcd_print("NTP server:", settings.wifi_ntp); delay(PRINT_DELAY);
    //     configTime(-10800, 0, settings.wifi_ntp);
    //     syncLocalTime();
    // }

    // Webserver init
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/index.html", String(), false, htmlProcessor);
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/style.css", "text/css");
    });
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/script.js", "text/javascript");
    });
    server.on("/test", HTTP_GET, [](AsyncWebServerRequest *request){
        Serial.print("[Test request] ");
        String phone = request->getParam("phone")->value();
        String apikey = request->getParam("apikey")->value();
        Serial.println("number: " + phone + ", apikey: " + apikey);

        // esp_err_t res = http_callMeBot_send(phone, apikey, "Test message", false);
        esp_err_t res = http_telegram_send(phone, TELEGRAM_TOKEN, "Test message", true);
        request->send( (res == ESP_OK) ? 200 : 500 );
    });
    server.on("/lib/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lib/jquery.min.js", "text/javascript");
    }); 
    server.on("/lib/bootstrap.min.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/lib/bootstrap.min.css", "text/css");
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

        wp_getSettings(new_sett, request);

        request->send(SPIFFS, "/reset.html", String(), false, htmlProcessor);
        Serial.println("Saving new settings...");
        EEPROM_WriteSettings(new_sett);
        Serial.println("Restarting...");
        lcd_print("Restarting...");
        delay(1000);
        ESP.restart();
    });
    server.begin();

    // Wifi Connection Error
    if ( connection_error ) {
        Serial.println("Rebooting in x minutes...");
        delay(NOWIFI_REBOOT_DELAY);
        ESP.restart();
    }

    // Notify Boot
    if ( wifi_connected ) {
        sendDoorbellNotifications("WifiDoorbell iniciado con IP: " + WiFi.localIP().toString());
    }
}


uint32_t refresh_time = 0;
uint32_t refresh_screen = 0;

void loop() {
    if ( !wifi_connected ) return;

    handleResetButton();

    // if ( millis()-refresh_time > REFRESH_TIME ) {
    //     refresh_time = millis();
    //     getLocalTime(&timeinfo);
    // }

    handleDoorbell();

    // if ( millis()-refresh_screen > REFRESH_SCREEN ) {
    //     refresh_screen = millis();
    //     lcd_screen1();
    // }
}
