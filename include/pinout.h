#ifndef PINOUT_H
#define PINOUT_H

// #define BOARD_MHETLIVE_ESP32MINIKIT

#ifdef BOARD_MHETLIVE_ESP32MINIKIT

    #define PIN_LCD_RS 21
    #define PIN_LCD_EN 22
    #define PIN_LCD_D4 18
    #define PIN_LCD_D5 19
    #define PIN_LCD_D6 23
    #define PIN_LCD_D7 5

    #define PIN_DS2820 26

    #define PIN_RESET 15
    #define LVL_RESET_PRESSED LOW

    #define PIN_VF_EN 0
    #define PIN_VF_V0 2

    #define PWM_CHANNEL 0
    #define PWM_FREQ 5000
    #define PWM_RESOLUTION 8
    #define PWM_MAX_DUTY 255

#else
// ESP32 DEV MODULE V1.1

    #define PIN_LCD_RS 23
    #define PIN_LCD_EN 22
    #define PIN_LCD_D4 5
    #define PIN_LCD_D5 18
    #define PIN_LCD_D6 19
    #define PIN_LCD_D7 21

    #define PIN_DS2820 15

    #define PIN_RESET 0
    #define LVL_RESET_PRESSED LOW

    #define PIN_VF_EN 16
    #define PIN_VF_V0 2

    #define PWM_CHANNEL 0
    #define PWM_FREQ 5000
    #define PWM_RESOLUTION 8
    #define PWM_MAX_DUTY 255

#endif

#endif