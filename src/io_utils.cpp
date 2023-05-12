#include "global.h"

float getTemp() {
    ds2820.requestTemperatures();
    return ds2820.getTempCByIndex(0);
}