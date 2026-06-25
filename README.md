# ESP32 Resilient Environmental Node

## Overview

This project implements a resilient ESP32 environmental monitoring node that simulates temperature and humidity sensor readings and publishes them to a public MQTT broker using JSON payloads.

The firmware is designed with a non-blocking reconnection strategy that automatically recovers from WiFi or MQTT connection failures without blocking the main execution loop.

---

## Features

* Simulated temperature sensor
* Simulated humidity sensor
* MQTT communication using EMQX public broker
* JSON payload generation using ArduinoJson
* MQTT monitoring using MQTTX
* Non-blocking WiFi reconnection
* Non-blocking MQTT reconnection
* Sensor data publishing every 5 seconds

---

## Technologies Used

* ESP32
* Arduino Framework (C++)
* PubSubClient
* ArduinoJson
* MQTTX
* Wokwi Simulator

---

## MQTT Configuration

Broker: broker.emqx.io

Port: 1883

Topic:

environment/sowmik/data

---

## Example JSON Payload

```json
{
  "temperature": 31.5,
  "humidity": 67.2,
  "uptime_ms": 25000
}
```

---

## Firmware Architecture

### Startup Phase

```text
Boot
 ↓
Connect WiFi
 ↓
Connect MQTT Broker
 ↓
Enter Main Loop
```

### Runtime Phase

```text
Loop
 ↓
Check WiFi
 ├── Disconnected
 │      ↓
 │ Reconnect WiFi
 │      ↓
 │ Return
 │
 └── Connected
         ↓
    Check MQTT
         ↓
    MQTT Connected?
         ↓
      No
         ↓
    Reconnect MQTT
         ↓
       Return
         ↓
      Yes
         ↓
 Read Sensor Values
         ↓
 Create JSON Payload
         ↓
 Publish MQTT Message
```

---

## Reconnection State Machine

```text
WIFI_DISCONNECTED
        ↓
Reconnect WiFi
        ↓
WIFI_CONNECTED
        ↓
MQTT_DISCONNECTED
        ↓
Reconnect MQTT
        ↓
MQTT_CONNECTED
        ↓
Publish Sensor Data
```

The reconnection logic uses millis()-based timers to perform periodic retry attempts without blocking the firmware execution.

---

## Build Instructions

### Arduino IDE

1. Install ESP32 Board Package
2. Install PubSubClient Library
3. Install ArduinoJson Library
4. Open the sketch
5. Select ESP32 Dev Module
6. Compile and Upload

### Wokwi

1. Open the Wokwi project
2. Start simulation
3. Connect MQTTX to broker.emqx.io
4. Subscribe to:

environment/sowmik/data

5. Observe incoming JSON messages

---

## Demonstration

The demonstration includes:

* ESP32 startup
* WiFi connection
* MQTT broker connection
* JSON payload publishing
* MQTTX receiving sensor data
* Automatic recovery from a simulated network failure
