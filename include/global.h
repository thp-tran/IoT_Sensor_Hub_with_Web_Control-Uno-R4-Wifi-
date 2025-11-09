#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

typedef struct {
  float temperature;
  float humidity;
} SensorData;

extern QueueHandle_t qTempHumi;
extern SemaphoreHandle_t semDHT;

#endif