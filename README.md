# 🌤️ IoT Sensor Hub with Web Control – Arduino UNO R4 WiFi

![Arduino](https://img.shields.io/badge/Arduino-UNO%20R4%20WiFi-blue)
![PlatformIO](https://img.shields.io/badge/Build-PlatformIO-orange)
![CoreIoT](https://img.shields.io/badge/Cloud-CoreIoT.io-success)
![License](https://img.shields.io/badge/License-MIT-green)

---

## 🧭 Overview

This project implements an **IoT Sensor Hub** on the **Arduino UNO R4 WiFi** (Renesas RA4M1 + ESP32-S3) platform.  
It measures **temperature and humidity** using a DHT sensor, displays readings on an **LCD and LED matrix**,  
and allows remote monitoring & control through a **local web dashboard** and **CoreIoT cloud**.

---

## ⚙️ Features

### 🌡️ Environmental Monitoring
- Reads temperature & humidity via **DHT22**.
- Displays data on **I2C LCD (20x4)** and optional **LED matrix**.

### 💻 Local Web Dashboard
- Hosted directly on UNO R4 WiFi.
- Access in AP mode at **`http://192.168.4.1`**
- Features:
  - Real-time temperature & humidity display (auto-refresh every 2s)
  - Wi-Fi configuration page (switch to STA mode)
  - LED ON/OFF controls

### ☁️ Cloud Connection – CoreIoT
- Sends telemetry every 10s to **CoreIoT MQTT Broker**:  
  `app.coreiot.io:1883`
- MQTT Topics:
  - Publish telemetry → `v1/devices/me/telemetry`
  - Subscribe RPC → `v1/devices/me/rpc/request/+`
- Remote LED control through RPC command:
  ```json
  { "method": "setStateLED", "params": "ON" }
