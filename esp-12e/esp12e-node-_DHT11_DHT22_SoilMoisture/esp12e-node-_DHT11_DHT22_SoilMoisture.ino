//NodeMCU &&  DHT22  &&  Soil moisture [temperature (x2), humidity(x2), moisture data]
#include "ThingsBoard.h"

#include <ESP8266WiFi.h>
#include "DHT.h"

//DHT11, DHT22

#define WIFI_AP "Forthnet_WIFI"
#define WIFI_PASSWORD "a1234567890a"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token

// Analog Device (Soil Moisture Sensor)
#define TOKEN_1 "Vex1ioMaJzM0T3os8Kmf"
// DHT11 Temperature Device
#define TOKEN_2 "Z7z3PEM7g0SbeACbKLFm"
// DHT11 Humidity Device
#define TOKEN_3 "bttkIkP3qeJPrWnAVIEj"
// DHT22 Temperature Device
#define TOKEN_4 "kyM5bGFGkj0LC8UOGXps"
// DHT22 Humidity Device
#define TOKEN_5 "fx84uJ9SyHa9rYrepQff"

//simulated devices
// SIMULATED DHT22 Temperature Device
#define TOKEN_6 "eFMqmVqvjxCemD0St7po"
// SIMULATED DHT22 Humidity Device
#define TOKEN_7 "ORY57urBkKoOCAIW1NVR"
// SIMULATED Analog Device (Soil Moisture Sensor 2)
#define TOKEN_8 "9WU1DxIdYQjMcfQArRAt"

//Actuators (represented with leds on breadboard)
//Water Pump 2
#define TOKEN_9 "G8v7pM1yZ22c3fj7L8oR"
//Light - photosynthesis booster 2
#define TOKEN_10 "wMLG9sQzziq6XxQ3fLVQ"

#define THINGSBOARD_SERVER "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD 115200

// Initialize multiple ThingsBoard clients
WiFiClient espClient1;
WiFiClient espClient2;
WiFiClient espClient3;
WiFiClient espClient4;
WiFiClient espClient5;

// Initialize multiple ThingsBoard instances
ThingsBoard tb1(espClient1);
ThingsBoard tb2(espClient2);
ThingsBoard tb3(espClient3);
ThingsBoard tb4(espClient4);
ThingsBoard tb5(espClient5);

// the Wifi radio's status
int status = WL_IDLE_STATUS;

//pin
const int analogInPin = A0;
int sensorValue = 0;

// DHT Sensors
DHT sensor1(D3, DHT11);
DHT sensor2(D1, DHT22);

//DHT sensor reading
//DHT11
float temperature_DHT11;
float humidity_DHT11;
//DHT22
float temperature_DHT22;
float humidity_DHT22;

void setup()
{
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();

  // init dht
  sensor1.begin();
  sensor2.begin();
}

void loop()
{
  delay(2500);

  if (WiFi.status() != WL_CONNECTED)
  {
    reconnect();
  }

  delay(100);
  if (!tb1.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN_1);
    if (!tb1.connect(THINGSBOARD_SERVER, TOKEN_1))
    {
      Serial.println("Failed to connect with token TOKEN_1");
      return;
    }
  }

  delay(100);
  if (!tb2.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN_2);
    if (!tb2.connect(THINGSBOARD_SERVER, TOKEN_2))
    {
      Serial.println("Failed to connect with token TOKEN_2");
      return;
    }
  }

  delay(100);
  if (!tb3.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN_3);
    if (!tb3.connect(THINGSBOARD_SERVER, TOKEN_3))
    {
      Serial.println("Failed to connect with token TOKEN_3");
      return;
    }
  }

  delay(100);
  if (!tb4.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN_4);
    if (!tb4.connect(THINGSBOARD_SERVER, TOKEN_4))
    {
      Serial.println("Failed to connect with token TOKEN_4");
      return;
    }
  }

  delay(100);
  if (!tb5.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN_5);
    if (!tb5.connect(THINGSBOARD_SERVER, TOKEN_5))
    {
      Serial.println("Failed to connect with token TOKEN_5");
      return;
    }
  }

  delay(500);

  //data ldr
  sensorValue = analogRead(analogInPin);

  //DHT11 Data
  temperature_DHT11 = sensor1.readTemperature();
  humidity_DHT11 = sensor1.readHumidity();

  temperature_DHT22 = sensor2.readTemperature();
  humidity_DHT22 = sensor2.readHumidity();

  Serial.println("Sending data...");
  Serial.println("Soil moisture sensor: " + String(sensorValue));
  Serial.println("DHT-11 temperature: " + String(temperature_DHT11));
  Serial.println("DHT-11 humidity: " + String(humidity_DHT11));
  Serial.println("DHT-22 temperature: " + String(temperature_DHT22));
  Serial.println("DHT-22 humidity: " + String(humidity_DHT22));

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  delay(100);
  tb1.sendTelemetryInt("Soil moisture sensor", sensorValue);
  delay(100);
  tb2.sendTelemetryInt("DHT-11 temperature", temperature_DHT11);
  delay(100);
  tb3.sendTelemetryFloat("DHT-11 humidity", humidity_DHT11);
  delay(100);
  tb4.sendTelemetryInt("DHT-22 temperature", temperature_DHT22);
  delay(100);
  tb5.sendTelemetryFloat("DHT-22 humidity", humidity_DHT22);

  delay(100);
  tb1.loop();
  delay(100);
  tb2.loop();
  delay(100);
  tb3.loop();
  delay(100);
  tb4.loop();
  delay(100);
  tb5.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect()
{
  // Loop until we're reconnected
  status = WiFi.status();
  if (status != WL_CONNECTED)
  {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
