#include "global.h"
#include "led_blink.h"
#include "temp_hum_sensor.h"
#include "mainserver.h"
#include "coreiot.h"
void setup() {
  Serial.begin(9600);
  delay(2000);
  Serial.println("Start");
  semDHT = xSemaphoreCreateBinary();
  qTempHumi = xQueueCreate(1, sizeof(SensorData));
  BaseType_t r1 = xTaskCreate(temp_hum_monitor, "TEMP", 512, NULL, 2, NULL);
  //BaseType_t r2 = xTaskCreate(led_blinky, "BLINK", 512, NULL, 2, NULL);
  xTaskCreate(webserver_task, "WebServer", 512, NULL, 1, NULL);
  vTaskStartScheduler();
}


  
void loop(){
}