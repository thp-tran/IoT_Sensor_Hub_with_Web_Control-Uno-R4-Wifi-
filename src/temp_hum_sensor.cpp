#include "temp_hum_sensor.h"
#include "global.h"
#include <DHTStable.h>
#include <LiquidCrystal_I2C.h>

const int DHTPIN = 2;


//LiquidCrystal_I2C lcd(0x27, 20, 4);
//DHTStable DHT;

void temp_hum_monitor(void *pvParameters)
{
    // lcd.init();
    // lcd.backlight();
    SensorData data;
    while (1)
    {   
        // DHT.read11(DHTPIN);
        // data.humidity = DHT.getHumidity();
        // data.temperature = DHT.getTemperature();
        data.humidity = 75;
        data.temperature = 27;
        xQueueOverwrite(qTempHumi, &data);
        
        // lcd.clear();
        // lcd.setCursor(0, 0);
        // lcd.print("Temp: ");
        // lcd.setCursor(7, 0);
        // lcd.print(data.temperature);
        // lcd.print(" C");
        // lcd.setCursor(0, 1);
        // lcd.print("Hum: ");
        // lcd.setCursor(7, 1);
        // lcd.print(data.humidity);
        // lcd.print(" %");
        Serial.print("Temp: ");
        Serial.println(data.temperature);
        Serial.print("Hum: ");
        Serial.println(data.temperature);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
}
