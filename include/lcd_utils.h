#ifndef LCD_UTILS_H
#define LCD_UTILS_H

#include <Arduino.h>

#define PRINT_DELAY 500

void lcd_print(const String& str, const String& str2="");
void lcd_screen1();
void lcd_screen2();

#endif