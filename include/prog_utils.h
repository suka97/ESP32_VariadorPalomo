#ifndef PROG_UTILS_H
#define PROG_UTILS_H

#include "global.h"

bool isTimeBetween(struct tm now, struct tm start, struct tm end);
int getProfileForTime(VF_Profile profiles[], struct tm now);
String getVfHtml(const String& var);
void vf_getSettings(EEPROM_Settings& sett, AsyncWebServerRequest *request);

#endif