#ifndef __LED_MATRIX__
#define __LED_MATRIX__
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "global.h"
#include "Arduino_LED_Matrix.h"

const uint32_t happy[] = {
    0x19819,
    0x80000001,
    0x81f8000
};
const uint32_t heart[] = {
    0x3184a444,
    0x44042081,
    0x100a0040
};

void led_matrix(void* pvParameters);
#endif