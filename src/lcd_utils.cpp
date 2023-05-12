#include "global.h"

void lcd_print(const String& text, const String& text2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(text);
    lcd.setCursor(0, 1);
    lcd.print(text2);
}