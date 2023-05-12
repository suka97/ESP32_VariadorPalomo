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


String getVfStr() {
    return String("P") + ( (vf_profile == -1) ? "-" : String(vf_profile) );
}


String getPwmStr() {
    uint8_t duty_100 = vf_pwm * 100 / PWM_MAX_DUTY;
    return (duty_100 == 0) ? "OFF " : (String(duty_100) + "%");
}


void lcd_screen1() {
    String row1 = time2string(timeinfo) + "    " + temp2string(ds2820_temp);
    String row2 = getPwmStr() + "          " + getVfStr();
    lcd_print(row1, row2);
}


void lcd_screen2() {
    String row1 = WiFi.localIP().toString();
    String row2 = "";
    lcd_print(row1, row2);
}