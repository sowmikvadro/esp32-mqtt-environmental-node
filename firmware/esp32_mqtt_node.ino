#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

// Professional MQTT Topic
const char* mqtt_topic = "environment/sowmik/data";

// Unique MQTT Client ID
const char* clientID = "SowmikESP32_EnvNode_V1";

WiFiClient espClient;
PubSubClient client(espClient);

// Simulated Sensor Functions
float getTemperature() {
  return random(250, 350) / 10.0;   // 25.0°C - 34.9°C
}

float getHumidity() {
  return random(500, 800) / 10.0;   // 50.0% - 79.9%
}

void reconnectWiFi() {

  static unsigned long lastAttempt = 0;

  if (millis() - lastAttempt < 5000) {
    return;
  }

  lastAttempt = millis();

  Serial.println("Attempting WiFi Reconnect...");

  WiFi.disconnect();
  WiFi.begin(ssid, password);
}

void reconnectMQTT() {

  static unsigned long lastAttempt = 0;

  if (millis() - lastAttempt < 5000) {
    return;
  }

  lastAttempt = millis();

  Serial.println("Attempting MQTT Reconnect...");

  if (client.connect(clientID)) {

    Serial.println("MQTT Reconnected!");

  } else {

    Serial.print("MQTT Failed. State = ");
    Serial.println(client.state());

  }
}

void setup() {

  Serial.begin(115200);

  randomSeed(micros());

  Serial.println("Connecting WiFi...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");

  client.setServer(mqtt_server, mqtt_port);
  client.setKeepAlive(60);

  Serial.println("Connecting MQTT Broker...");

  if (client.connect(clientID)) {

    Serial.println("MQTT Connected!");

    client.publish(
      mqtt_topic,
      "ESP32 Environmental Node Started"
    );

    Serial.println("Startup Message Published!");

  } else {

    Serial.print("MQTT Failed. State = ");
    Serial.println(client.state());

  }
}

void loop() {

  // WiFi State Check
  if (WiFi.status() != WL_CONNECTED) {

    reconnectWiFi();
    return;
  }

  // MQTT State Check
  if (!client.connected()) {

    reconnectMQTT();
    return;
  }

  // Maintain MQTT Connection
  client.loop();

  static unsigned long lastPublish = 0;

  if (millis() - lastPublish >= 5000) {

    lastPublish = millis();

    float temperature = getTemperature();
    float humidity = getHumidity();

    JsonDocument doc;

    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["uptime_ms"] = millis();

    char payload[128];

    serializeJson(doc, payload);

    bool publishResult = client.publish(
      mqtt_topic,
      payload
    );

    Serial.print("Publish Result: ");
    Serial.println(publishResult);

    Serial.print("Published JSON: ");
    Serial.println(payload);

    Serial.println("-------------------------");
  }
}
