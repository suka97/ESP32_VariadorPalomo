#ifndef LCD_UTILS_H
#define LCD_UTILS_H

#include <Arduino.h>

#define PRINT_DELAY 2000

void lcd_print(const String& str, const String& str2="");

#endif