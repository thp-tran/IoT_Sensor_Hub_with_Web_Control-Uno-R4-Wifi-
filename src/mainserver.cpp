#include "mainserver.h"
#include "global.h"
#include <WiFiS3.h>
#include "coreiot.h"

const char *ap_ssid = "UNO_R4_AP";
const char *ap_pass = "12345678";
WiFiServer server(80);
bool need_revert_AP = false;
bool isAPMode = true;
bool connecting = false;
unsigned long connect_start_ms = 0;
String wifi_ssid = "";
String wifi_password = "";

// HTML Main Page
String mainPage()
{
  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
      <meta charset="utf-8">
      <title>UNO R4 WiFi Dashboard</title>
      <style>
          body {
              background-color: #f5f7fa;
              color: #222;
              font-family: "Segoe UI", Arial, sans-serif;
              text-align: center;
              margin: 0;
              padding: 0;
          }
          header {
              background: linear-gradient(90deg, #0078d7, #00a8ff);
              color: white;
              padding: 20px 0;
              font-size: 22px;
              font-weight: bold;
              box-shadow: 0 2px 5px rgba(0,0,0,0.2);
          }
          .card {
              display: inline-block;
              background: white;
              border-radius: 12px;
              box-shadow: 0 4px 10px rgba(0,0,0,0.1);
              margin: 20px;
              padding: 20px;
              width: 280px;
              transition: 0.3s;
          }
          .card:hover { transform: translateY(-4px); }
          .sensor-value {
              font-size: 40px;
              font-weight: bold;
              color: #0078d7;
          }
          .button {
              padding: 12px 25px;
              margin: 8px;
              border: none;
              border-radius: 8px;
              font-size: 15px;
              cursor: pointer;
              color: white;
              transition: 0.2s;
          }
          .on { background-color: #28a745; }
          .off { background-color: #dc3545; }
          .on:hover { background-color: #218838; }
          .off:hover { background-color: #c82333; }
          footer {
              margin-top: 30px;
              font-size: 13px;
              color: #666;
          }
          a {
              color: #0078d7;
              text-decoration: none;
          }
          a:hover {
              text-decoration: underline;
          }
      </style>
  </head>
  <body>
      <header>🌤️ UNO R4 WiFi DHT Dashboard</header>

      <div class="card">
          <h2>🌡️ Temperature</h2>
          <div class="sensor-value" id="temp">--</div>
          <p>°C</p>
      </div>

      <div class="card">
          <h2>💧 Humidity</h2>
          <div class="sensor-value" id="humi">--</div>
          <p>%</p>
      </div>

      <div class="card">
          <h2>💡 LED Controls</h2>
          <button class="button on" onclick="fetch('/led1?state=on')">LED 1 ON</button>
          <button class="button off" onclick="fetch('/led1?state=off')">LED 1 OFF</button><br>
          <button class="button on" onclick="fetch('/led2?state=on')">LED 2 ON</button>
          <button class="button off" onclick="fetch('/led2?state=off')">LED 2 OFF</button>
      </div>

      <p><a href="/settings">⚙️ Wi-Fi Settings</a></p>

      <footer>UNO R4 WiFi © 2025 | CoreIoT Demo</footer>

      <script>
        async function updateSensor() {
          try {
            const res = await fetch('/sensors');
            const data = await res.json();
            document.getElementById('temp').innerText = data.temperature.toFixed(1);
            document.getElementById('humi').innerText = data.humidity.toFixed(1);
          } catch (e) { console.log('Fetch error:', e); }
        }
        setInterval(updateSensor, 2000);
        updateSensor();
      </script>
  </body>
  </html>
  )rawliteral";
  return page;
}

// HTML WiFi Settings Page
String settingsPage()
{
  String page = R"rawliteral(
  <!DOCTYPE html>
  <html>
  <head>
      <meta charset="utf-8">
      <title>WiFi Settings</title>
      <style>
          body {
              background-color: #f5f7fa;
              color: #222;
              font-family: "Segoe UI", Arial, sans-serif;
              text-align: center;
              margin: 0;
              padding: 0;
          }
          header {
              background: linear-gradient(90deg, #0078d7, #00a8ff);
              color: white;
              padding: 20px 0;
              font-size: 22px;
              font-weight: bold;
              box-shadow: 0 2px 5px rgba(0,0,0,0.2);
          }
          .container {
              margin-top: 50px;
              background: white;
              display: inline-block;
              border-radius: 12px;
              box-shadow: 0 4px 10px rgba(0,0,0,0.1);
              padding: 30px;
          }
          input, button {
              padding: 12px;
              margin: 10px;
              width: 240px;
              border: 1px solid #ccc;
              border-radius: 8px;
              font-size: 15px;
          }
          button {
              background-color: #0078d7;
              color: white;
              border: none;
              cursor: pointer;
              transition: 0.2s;
          }
          button:hover {
              background-color: #005fa3;
          }
          a {
              color: #0078d7;
              text-decoration: none;
              display: block;
              margin-top: 15px;
          }
      </style>
  </head>
  <body>
      <header>⚙️ Wi-Fi Configuration</header>

      <div class="container">
          <form action="/connect" method="GET">
              <input type="text" name="ssid" placeholder="WiFi SSID" required><br>
              <input type="password" name="pass" placeholder="Password"><br>
              <button type="submit">Connect</button>
          </form>
          <a href="/">← Back to Dashboard</a>
      </div>
  </body>
  </html>
  )rawliteral";
  return page;
}

// Parse GET parameter value from URL
String getParamValue(const String &req, const String &key)
{
  int idx = req.indexOf(key + "=");
  if (idx == -1)
    return "";
  int start = idx + key.length() + 1;
  int end = req.indexOf('&', start);
  if (end == -1)
    end = req.indexOf(' ', start);
  return req.substring(start, end);
}

void handleConnect(const String &req)
{
  wifi_ssid = getParamValue(req, "ssid");
  wifi_password = getParamValue(req, "pass");
  Serial.print("WiFi SSID: ");
  Serial.println(wifi_ssid);
  Serial.print("WiFi PASS: ");
  Serial.println(wifi_password);

  connecting = true;
  connect_start_ms = millis();
}

void startAP()
{
  delay(2000);
  Serial.println("Starting AP mode...");
  WiFi.end();
  WiFi.beginAP(ap_ssid, ap_pass);
  delay(2000);
  Serial.print("AP IP: ");
  Serial.println(WiFi.localIP());
  isAPMode = true;
}

void revert_to_AP()
{
  Serial.println("🔄 Reverting to AP mode...");
  WiFi.disconnect();
  delay(300);
  WiFi.end();
  delay(300);

  Serial.println("Starting Access Point...");
  WiFi.beginAP(ap_ssid, ap_pass);
  delay(2000);
  Serial.print("AP IP: ");
  Serial.println(WiFi.localIP());
  isAPMode = true;

  server.begin();
  Serial.println("✅ Web server restarted at 192.168.4.1");
}

void startSTA(const char *ssid, const char *pass)
{
  Serial.println("Switching to STA mode...");
  WiFi.end();
  WiFi.begin(ssid, pass);
  unsigned long start = millis();

  // Đợi tối đa 10 giây để kết nối Wi-Fi
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConnected to STA network!");
    Serial.print("STA IP: ");
    Serial.println(WiFi.localIP());
    isAPMode = false;

    // ✅ Khởi động task CoreIoT sau khi STA kết nối thành công
    xTaskCreate(coreiot_task, "BLINK", 512, NULL, 2, NULL);
  }
  else
  {
    Serial.println("\nSTA connection failed, reverting to AP mode");
    need_revert_AP = true;
  }
}

void webserver_task(void *pvParameters)
{
  startAP();
  server.begin();
  Serial.println("HTTP server started");

  SensorData latestData = {0, 0};

  while (1)
  {
    if (need_revert_AP)
    {
      revert_to_AP();
      need_revert_AP = false;
      vTaskDelay(3000);
    }
    WiFiClient client = server.available();
    if (client)
    {
      Serial.println("Client access");
      String req = client.readStringUntil('\r');
      client.flush();

      // Handle /connect (switch to STA)
      if (req.indexOf("GET /connect") != -1)
      {
        handleConnect(req);
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/plain");
        client.println("Connection: close");
        client.println();
        client.println("Connecting to WiFi...");
        client.stop();
        startSTA(wifi_ssid.c_str(), wifi_password.c_str());
        continue;
      }

      // Settings page
      if (req.indexOf("GET /settings") != -1)
      {
        Serial.println("Serving settings page...");
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: text/html");
        client.println("Connection: close");
        client.println();
        client.print(settingsPage());
        delay(10);
        client.stop();
        continue;
      }

      // Sensors JSON
      if (req.indexOf("GET /sensors") != -1)
      {
        if (xQueuePeek(qTempHumi, &latestData, 0) != pdTRUE)
        {
          Serial.println("⚠️ No sensor data yet");
          latestData.temperature = -1;
          latestData.humidity = -1;
        }
        String json = "{\"temperature\":";
        json += String(latestData.temperature, 1);
        json += ",\"humidity\":";
        json += String(latestData.humidity, 1);
        json += "}";
        client.println("HTTP/1.1 200 OK");
        client.println("Content-Type: application/json");
        client.println("Connection: close");
        client.println();
        client.print(json);
        client.stop();
        continue;
      }

      // LED controls
      if (req.indexOf("/led1?state=on") != -1)
        digitalWrite(13, HIGH);
      else if (req.indexOf("/led1?state=off") != -1)
        digitalWrite(13, LOW);
      else if (req.indexOf("/led2?state=on") != -1)
        digitalWrite(4, HIGH);
      else if (req.indexOf("/led2?state=off") != -1)
        digitalWrite(4, LOW);

      // Default: main page
      client.println("HTTP/1.1 200 OK");
      client.println("Content-Type: text/html");
      client.println("Connection: close");
      client.println();
      client.print(mainPage());
      delay(10);
      client.stop();
    }
    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
