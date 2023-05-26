#include "global.h"

float getTemp() {
    ds2820.requestTemperatures();
    return ds2820.getTempCByIndex(0);
}


// 0 - 100
float getManualAdc() {
    uint16_t adc = analogRead(PIN_ADC_MANUAL);
    #ifdef ADC_MANUAL_BWD
        adc = LVL_ADC_MANUAL_MAX - adc;
    #endif
    return adc * 100.0 / LVL_ADC_MANUAL_MAX;
}


void setVfSpeed(float speed) {
    if ( speed > 100.0 ) speed = 100.0;
    vf_pwm = speed / 100.0 * PWM_MAX_DUTY;
    uint32_t duty = vf_pwm;
    #ifdef LVL_VF_V0_INV
        duty = PWM_MAX_DUTY - vf_pwm;
    #endif
    ledcWrite(PWM_CHANNEL, duty); 
}