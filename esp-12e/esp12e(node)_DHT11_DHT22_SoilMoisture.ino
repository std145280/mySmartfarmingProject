//NodeMCU &&  DHT22  &&  Soil moisture [temperature (x2), humidity(x2), moisture data]
#include "ThingsBoard.h"

#include <ESP8266WiFi.h>
#include "DHT.h"

//DHT11, DHT22


#define WIFI_AP "Forthn********"
#define WIFI_PASSWORD "***********"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN "My_Token"
#define THINGSBOARD_SERVER "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD 115200

// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;

//pin
const int analogInPin = A0;
int sensorValue = 0;

// DHT Sensor
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

  if (!tb.connected())
  {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN))
    {
      Serial.println("Failed to connect");
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
  tb.sendTelemetryInt("Soil moisture sensor", sensorValue);
  tb.sendTelemetryInt("DHT-11 temperature", temperature_DHT11);
  tb.sendTelemetryFloat("DHT-11 humidity", humidity_DHT11);
  tb.sendTelemetryInt("DHT-22 temperature", temperature_DHT22);
  tb.sendTelemetryFloat("DHT-22 humidity", humidity_DHT22);

  tb.loop();
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