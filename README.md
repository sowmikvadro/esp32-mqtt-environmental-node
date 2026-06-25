# ESP32 Resilient Environmental Node

## Overview

This project implements a resilient ESP32 environmental monitoring node that simulates temperature and humidity sensor readings and publishes them to a public MQTT broker using JSON payloads.

The firmware is designed with a non-blocking reconnection strategy that automatically recovers from WiFi or MQTT connection failures without blocking the main execution loop.

The project was developed using the Arduino Framework (C++) and demonstrated using the Wokwi ESP32 Simulator.

---

# Features

* Simulated temperature sensor
* Simulated humidity sensor
* MQTT communication using a public EMQX broker
* JSON payload generation using ArduinoJson
* MQTT monitoring using MQTTX
* Non-blocking WiFi reconnection
* Non-blocking MQTT reconnection
* Sensor data publishing every 5 seconds
* MQTT publish status verification
* Automatic recovery from communication failures

---

# Technologies Used

* ESP32
* Arduino Framework (C++)
* PubSubClient
* ArduinoJson
* MQTTX
* Wokwi Simulator

---

# MQTT Configuration

| Parameter | Value                   |
| --------- | ----------------------- |
| Broker    | broker.emqx.io          |
| Port      | 1883                    |
| Topic     | environment/sowmik/data |
| Client ID | SowmikESP32_EnvNode_V1  |

---

# Example JSON Payload

```json
{
  "temperature": 31.5,
  "humidity": 67.2,
  "uptime_ms": 25000
}
```

---

# Firmware Architecture

## Startup Phase

```text
Boot
 ↓
Initialize Serial
 ↓
Connect WiFi
 ↓
Configure MQTT Client
 ↓
Connect MQTT Broker
 ↓
Enter Main Loop
```

## Runtime Phase

```text
Main Loop
    ↓
Check WiFi Connection
    ↓
WiFi Connected?
 ├─ No
 │   ↓
 │ Reconnect WiFi
 │   ↓
 │ Return
 │
 └─ Yes
      ↓
Check MQTT Connection
      ↓
MQTT Connected?
 ├─ No
 │   ↓
 │ Reconnect MQTT
 │   ↓
 │ Return
 │
 └─ Yes
      ↓
Generate Sensor Values
      ↓
Create JSON Payload
      ↓
Publish MQTT Message
      ↓
Wait for Next Publish Cycle
```

---

# Reconnection State Machine

```text
WIFI_DISCONNECTED
        ↓
Attempt WiFi Reconnection
        ↓
WIFI_CONNECTED
        ↓
MQTT_DISCONNECTED
        ↓
Attempt MQTT Reconnection
        ↓
MQTT_CONNECTED
        ↓
Publish Sensor Data
```

The firmware continuously monitors WiFi and MQTT connectivity.

If either connection is lost, the firmware attempts recovery every 5 seconds using a millis()-based timer.

This approach avoids blocking the application and keeps the main loop responsive.

---

# Non-Blocking Reconnection Strategy

The reconnection mechanism is implemented using periodic timer checks based on `millis()`.

Example logic:

```cpp
if (millis() - lastAttempt >= 5000)
{
    reconnect();
}
```

Benefits:

* No delay() used during runtime reconnection
* Main loop remains responsive
* Continuous system monitoring
* Automatic recovery from connection failures

---

# Build Instructions

## Arduino IDE

1. Install Arduino IDE
2. Install ESP32 Board Package
3. Install PubSubClient Library
4. Install ArduinoJson Library
5. Open the project source file
6. Select ESP32 Dev Module
7. Compile and Upload

## Wokwi Simulator

1. Open the Wokwi project
2. Start simulation
3. Open MQTTX
4. Connect MQTTX to:

   * Broker: broker.emqx.io
   * Port: 1883
5. Subscribe to:

```text
environment/sowmik/data
```

6. Observe incoming JSON messages

---

# Wokwi Simulation

Project Link:

https://wokwi.com/projects/467735100166977537

---

# Screenshots

## Serial Monitor

![Serial Monitor](screenshots/serial-monitor.png)

## MQTTX Output

![MQTTX Output](screenshots/mqttx-output.png)

---

# Simulated Network Failure Test

To verify firmware resilience, MQTT connectivity failure was simulated.

Observed behavior:

1. MQTT connection loss was detected.
2. The firmware remained operational.
3. Automatic reconnection attempts were performed every 5 seconds.
4. Once connectivity was restored, MQTT reconnected automatically.
5. Sensor data publishing resumed without requiring a restart.

Example serial output:

```text
Attempting MQTT Reconnect...
MQTT Failed. State = -2

Attempting MQTT Reconnect...
MQTT Failed. State = -2

MQTT Reconnected!

Published JSON:
{"temperature":31.2,"humidity":65.7,"uptime_ms":25000}
```

This demonstrates successful recovery from communication failures while maintaining normal firmware operation.

---

# Demonstration

The demonstration video includes:

* ESP32 startup sequence
* WiFi connection
* MQTT broker connection
* Sensor data generation
* JSON payload publishing
* MQTTX message reception
* Simulated network disconnect
* Automatic reconnection and recovery

---

# Author

Sowmik Kumar Vadro
Computer Science and Engineering (CSE)
AUST
Bangladesh
