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