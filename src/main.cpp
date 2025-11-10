#include "global.h"
#include "led_blink.h"
#include "temp_hum_sensor.h"
#include "mainserver.h"
#include "coreiot.h"
#include "led_matrix.h"
void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  delay(2000);
  Serial.println("Start");
  qTempHumi = xQueueCreate(1, sizeof(SensorData));
  xTaskCreate(temp_hum_monitor, "TEMP", 256, NULL, 3, NULL);
  // xTaskCreate(led_blinky, "BLINK", 512, NULL, 2, NULL);
  xTaskCreate(led_matrix, "LED_Matrix", 256, NULL, 2, NULL);
  xTaskCreate(webserver_task, "WebServer", 512, NULL, 2, NULL);
  vTaskStartScheduler();
}


  
void loop(){
}
// #include <Arduino.h>
// #include <WiFiS3.h>
// #include <WiFiServer.h>

// WiFiServer server1(80);

// // Default Access Point configuration
// const char* ap_ssid1 = "Arduino_R4_AP";
// const char* ap_pass1 = "12345678";

// // Temporary variables for user input
// String inputSSID = "";
// String inputPASS = "";

// // ==========================
// // Start Access Point mode
// // ==========================
// void startAPMode() {
//   WiFi.disconnect();
//   delay(500);
//   if (WiFi.beginAP(ap_ssid1, ap_pass1) == WL_AP_LISTENING) {
//     Serial.println(" Access Point mode started successfully!");
//     Serial.print("SSID: "); Serial.println(ap_ssid1);
//     Serial.print("Password: "); Serial.println(ap_pass1);
//     Serial.print("IP address: "); Serial.println(WiFi.localIP());
//     server1.begin();
//   } else {
//     Serial.println(" Failed to start Access Point mode!");
//   }
// }

// // ==========================
// // Try connecting in STA mode
// // ==========================
// bool tryConnectSTA(const char* ssid, const char* pass) {
//   Serial.print(" Trying to connect to WiFi: ");
//   Serial.println(ssid);

//   WiFi.disconnect();
//   delay(500);
//   WiFi.begin(ssid, pass);

//   unsigned long start = millis();
//   const unsigned long timeout = 10000; // 10 seconds

//   while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
//     Serial.print(".");
//     delay(500);
//   }

//   if (WiFi.status() == WL_CONNECTED) {
//     Serial.println("\n Connected successfully!");
//     Serial.print("STA IP: "); Serial.println(WiFi.localIP());
//     return true;
//   } else {
//     Serial.println("\n Connection failed, returning to AP mode...");
//     return false;
//   }
// }

// // ==========================
// // Send basic HTML page
// // ==========================
// void sendWebPage(WiFiClient& client) {
//   client.println("HTTP/1.1 200 OK");
//   client.println("Content-Type: text/html");
//   client.println("Connection: close");
//   client.println();
//   client.println("<!DOCTYPE html><html><head><title>WiFi Config</title></head><body>");
//   client.println("<h2>Arduino R4 WiFi - Network Configuration</h2>");
//   client.println("<form action=\"/connect\" method=\"GET\">");
//   client.println("SSID:<br><input type='text' name='ssid'><br>");
//   client.println("Password:<br><input type='password' name='pass'><br><br>");
//   client.println("<input type='submit' value='Connect WiFi'>");
//   client.println("</form>");
//   client.println("</body></html>");
// }

// // ==========================
// // Handle incoming HTTP requests
// // ==========================
// void handleClient() {
//   WiFiClient client = server1.available();
//   if (!client) return;

//   String req = client.readStringUntil('\r');
//   client.flush();

//   if (req.startsWith("GET /connect")) {
//     int ssidIndex = req.indexOf("ssid=");
//     int passIndex = req.indexOf("&pass=");
//     if (ssidIndex > 0 && passIndex > 0) {
//       inputSSID = req.substring(ssidIndex + 5, passIndex);
//       inputPASS = req.substring(passIndex + 6, req.indexOf(" ", passIndex));
//       inputSSID.replace("%20", " ");
//       inputPASS.replace("%20", " ");

//       Serial.println(" User input:");
//       Serial.print("SSID: "); Serial.println(inputSSID);
//       Serial.print("PASS: "); Serial.println(inputPASS);

//       client.println("HTTP/1.1 200 OK");
//       client.println("Content-Type: text/html");
//       client.println("Connection: close");
//       client.println();
//       client.println("<html><body>");
//       client.println("<h3>Trying to connect...</h3>");
//       client.println("<p>Check Serial Monitor for status.</p>");
//       client.println("</body></html>");
//       client.stop();

//       if (!tryConnectSTA(inputSSID.c_str(), inputPASS.c_str())) {
//         startAPMode();  // Back to AP if connection fails
//       }
//       return;
//     }
//   }

//   sendWebPage(client);
//   delay(1);
//   client.stop();
// }

// // ==========================
// // Setup
// // ==========================
// void setup() {
//   Serial.begin(9600);
//   delay(2000);
//   startAPMode();
// }

// // ==========================
// // Main loop
// // ==========================
// void loop() {
//   handleClient();
// }
