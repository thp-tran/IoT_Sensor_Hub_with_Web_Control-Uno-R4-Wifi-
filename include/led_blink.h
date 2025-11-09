#ifndef __LED_BLINK_H__
#define __LED_BLINK_H__
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "global.h"

#define LED_GPIO 13
void led_blinky(void* pvParameters);
#endif