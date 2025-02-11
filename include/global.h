#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <SPIFFS.h>
#include "time.h"
#include <LiquidCrystal.h>
#include <HTTPClient.h>

#include "pinout.h"
#include "wifi_utils.h"
#include "eeprom_utils.h"
#include "eeprom_map.h"
#include "prog_utils.h"
#include "lcd_utils.h"
#include "io_utils.h"
#include "http_utils.h"

#define EEPROM_SIZE 512
#define TELEGRAM_TOKEN      "8076586992:AAFrUk5gwQyQ5KRRNgkKC4MVktQBDD2Ai1Q"

extern const uint32_t REFRESH_TIME;
extern const uint32_t REFRESH_SCREEN;
extern const uint32_t RESET_TIME;

extern const char* BOARD_ID;
extern const uint8_t VERSION;

extern const char* AP_SSID;
extern const char* AP_PWD;

extern EEPROM_Settings settings;
extern AsyncWebServer server;
extern struct tm timeinfo;

extern LiquidCrystal lcd;

#endif