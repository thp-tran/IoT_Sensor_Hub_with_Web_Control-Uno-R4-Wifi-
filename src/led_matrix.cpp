#include "led_matrix.h"

ArduinoLEDMatrix matrix;

void led_matrix(void* pvParameters){
    matrix.begin();
    while (1)
    {
        /* code */
        SensorData data;
        xSemaphoreTake(semDHT, portMAX_DELAY);
        xQueueReceive(qTempHumi, &data, portMAX_DELAY);
        if(data.humidity > 80){
            matrix.loadFrame(heart);
        } else {
            matrix.loadFrame(happy);
        }
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    
}