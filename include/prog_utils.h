#ifndef PROG_UTILS_H
#define PROG_UTILS_H

#include "global.h"

bool isTimeBetween(struct tm now, struct tm start, struct tm end);
String getWpHtml(const String& var);
void wp_getSettings(EEPROM_Settings& sett, AsyncWebServerRequest *request);
void handleResetButton();

#endif