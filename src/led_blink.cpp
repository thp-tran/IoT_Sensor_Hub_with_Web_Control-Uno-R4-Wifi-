#include "led_blink.h"
int led = 13;

void led_blinky(void *pvParameters)
{
    SensorData recv0;

    while (1)
    {
        if (xQueuePeek(qTempHumi, &recv0, 0) == pdTRUE)
        {
            float hum = recv0.humidity;
            int blinkDelay = 0;

            if (hum < 50)
            {
                digitalWrite(LED_GPIO, LOW);
                blinkDelay = 1000;
            }
            else if (hum < 60)
                blinkDelay = 1000;
            else if (hum < 70)
                blinkDelay = 500;
            else if (hum < 80)
                blinkDelay = 200;
            else
                blinkDelay = 100;

            if (hum >= 50)
            {
                digitalWrite(LED_GPIO, HIGH);
                vTaskDelay(pdMS_TO_TICKS(blinkDelay));
                digitalWrite(LED_GPIO, LOW);
                vTaskDelay(pdMS_TO_TICKS(blinkDelay));
            }
            else
            {
                vTaskDelay(pdMS_TO_TICKS(blinkDelay));
            }
        }
        else
        {
            // Nếu chưa có dữ liệu thì chờ nhẹ
            vTaskDelay(pdMS_TO_TICKS(200));
        }
    }
}
