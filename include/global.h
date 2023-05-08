#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <SPIFFS.h>
#include "time.h"

#include "wifi_utils.h"
#include "eeprom_utils.h"
#include "eeprom_map.h"

#define EEPROM_SIZE 512

extern const char* BOARD_ID;
extern const uint8_t VERSION;

extern const char* AP_SSID;
extern const char* AP_PWD;

extern EEPROM_Settings settings;
extern AsyncWebServer server;