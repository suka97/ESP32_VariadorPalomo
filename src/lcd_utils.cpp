#include "global.h"

void lcd_print(const String& text, const String& text2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(text);
    lcd.setCursor(0, 1);
    lcd.print(text2);
}


String temp2string(float temp) {
    char buffer[10];
    sprintf(buffer, "%2.1f", temp);
    return String(buffer) + "C";
}


void lcd_screen1() {
    String row1 = "";
    String row2 = "";
    lcd_print(row1, row2);
}


void lcd_screen2() {
    String row1 = WiFi.localIP().toString();
    String row2 = "";
    lcd_print(row1, row2);
}