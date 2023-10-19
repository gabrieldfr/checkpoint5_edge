#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Defines for DHT sensor
#define DHT_PIN 4
#define DHT_TYPE DHT11

// Define for the photoresistor pin
#define PHOTORESISTOR_PIN 34

// Defines for LED control
#define LED_PIN 2

// MQTT defines
#define MQTT_BROKER "your_broker_address"
#define MQTT_PORT 1883
#define MQTT_TOPIC_SUBSCRIBE "/TEF/lamp108/cmd"
#define MQTT_TOPIC_PUBLISH "/TEF/lamp108/attrs"
#define MQTT_TOPIC_PUBLISH_TEMPERATURE "/TEF/lamp108/attrs/temperature"
#define MQTT_TOPIC_PUBLISH_HUMIDITY "/TEF/lamp108/attrs/humidity"
#define MQTT_TOPIC_PUBLISH_LUMINOSITY "/TEF/lamp108/attrs/luminosity"
#define MQTT_CLIENT_ID "fiware_108"
#define WIFI_SSID "YourWiFiSSID"
#define WIFI_PASSWORD "YourWiFiPassword"

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHT_PIN, DHT_TYPE);

char ledState = '0';

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  initWiFi();
  initMQTT();
  initDHT();
}

void loop() {
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop();
  publishSensorData();
  checkLEDState();
}

void initWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void initMQTT() {
  client.setServer(MQTT_BROKER, MQTT_PORT);
  client.setCallback(mqttCallback);
}

void initDHT() {
  dht.begin();
}

void reconnectMQTT() {
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (client.connect(MQTT_CLIENT_ID)) {
      Serial.println("Connected to MQTT broker");
      client.subscribe(MQTT_TOPIC_SUBSCRIBE);
    } else {
      Serial.print("MQTT connection failed, rc=");
      Serial.print(client.state());
      Serial.println(" Trying again in 5 seconds...");
      delay(5000);
    }
  }
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (message.equals("lamp108@on|")) {
    digitalWrite(LED_PIN, HIGH);
    ledState = '1';
  } else if (message.equals("lamp108@off|")) {
    digitalWrite(LED_PIN, LOW);
    ledState = '0';
  }
}

void checkLEDState() {
  if (ledState == '1') {
    digitalWrite(LED_PIN, HIGH);
  } else {
    digitalWrite(LED_PIN, LOW);
  }
}

void publishSensorData() {
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (!isnan(humidity) && !isnan(temperature)) {
    char msgBuffer[5];
    dtostrf(humidity, 4, 1, msgBuffer);
    client.publish(MQTT_TOPIC_PUBLISH_HUMIDITY, msgBuffer);

    dtostrf(temperature, 4, 1, msgBuffer);
    client.publish(MQTT_TOPIC_PUBLISH_TEMPERATURE, msgBuffer);
  }

  int sensorValue = analogRead(PHOTORESISTOR_PIN);
  float luminosity = map(sensorValue, 0, 4095, 0, 100);
  char msgBuffer[5];
  dtostrf(luminosity, 4, 1, msgBuffer);
  client.publish(MQTT_TOPIC_PUBLISH_LUMINOSITY, msgBuffer);
}