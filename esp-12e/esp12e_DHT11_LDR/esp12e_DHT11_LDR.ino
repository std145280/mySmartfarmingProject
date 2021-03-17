//ESP8266-12E  &&  DHT11  &&  LDR [temperature, humidity, light data] - 3 tokens version
#include "ThingsBoard.h"

#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTTYPE DHT11

#define WIFI_AP "Forthnet_WIFI"
#define WIFI_PASSWORD "a1234567890a"

//multiple tokens setup for multiple 1-sensor devices
// Analog Device (LDR)
#define TOKEN_1 "WDjwIpAgdgINoAVRbGMH"
// DHT11 Temperature Device 
#define TOKEN_2 "vNw2XqCMIqe8yJHQVakf"
// DHT11 Humidity Device 
#define TOKEN_3 "ZggbFLIi3snSb58ku6o8"

#define THINGSBOARD_SERVER "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD 115200

// Initialize multiple ThingsBoard clients
WiFiClient espClient1;
WiFiClient espClient2;
WiFiClient espClient3;

// Initialize Multiple ThingsBoard instances
ThingsBoard tb1(espClient1);
ThingsBoard tb2(espClient2);
ThingsBoard tb3(espClient3);

// the Wifi radio's status
int status = WL_IDLE_STATUS;

//pin
const int analogInPin = A0;
int sensorValue = 0;

// DHT Sensor
uint8_t DHTPin = D3;

// Initialize DHT sensor.
DHT dht(DHTPin, DHTTYPE);

//DHT sensor reading
float temperature_DHT11;
float humidity_DHT11;

void setup()
{
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();

  // init dht
  pinMode(DHTPin, INPUT);
  dht.begin();
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
    // Connect to the ThingsBoard with token 1 device
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
    // Connect to the ThingsBoard with token 2 device
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
    // Connect to the ThingsBoard with token 3 device
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

  delay(500);

  //data ldr
  sensorValue = analogRead(analogInPin);

  //DHT11 Data
  temperature_DHT11 = dht.readTemperature();
  humidity_DHT11 = dht.readHumidity();

  Serial.println("Sending data...");
  Serial.println("LDR sensor: " + String(sensorValue));
  Serial.println("DHT-11 temperature: " + String(temperature_DHT11));
  Serial.println("DHT-11 humidity: " + String(humidity_DHT11));

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  delay(100);
  tb1.sendTelemetryInt("LDR data", sensorValue);
  delay(100);
  tb2.sendTelemetryInt("DHT-11 temperature", temperature_DHT11);
  delay(100);
  tb3.sendTelemetryFloat("DHT-11 humidity", humidity_DHT11);

  delay(100);
  tb1.loop();
  delay(100);
  tb2.loop();
  delay(100);
  tb3.loop();
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