#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include "eeprom_map.h"

#define EEPROM_SIZE 512

void EEPROM_Clear();
void EEPROM_WriteValue(long value, int i);
void EEPROM_ReadSettings();
void EEPROM_WriteSettings(EEPROM_Settings settings);
void EEPROM_Begin();
void EEPROM_CreateSettings();

#endif