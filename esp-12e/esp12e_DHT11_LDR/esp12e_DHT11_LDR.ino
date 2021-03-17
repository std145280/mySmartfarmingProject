//ESP8266-12E  &&  DHT11  &&  LDR [temperature, humidity, light data] - 3 tokens version
#include "ThingsBoard.h"

#include <ESP8266WiFi.h>
#include "DHT.h"

#define DHTTYPE DHT11

#define WIFI_AP "Forthnet_WIFI"
#define WIFI_PASSWORD "a1234567890a"

//multiple tokens setup for multiple 1-sensor devices
// Analog Device (LDR)
#define TOKEN_1 "Qa82XbQHPyYgzVbbySda"
// DHT11 Temperature Device 
#define TOKEN_2 "bVbmAAWfOYi31eX8w0K0"
// DHT11 Humidity Device 
#define TOKEN_3 "ZmQFrvOXm4tNZOQJWnLr"


//simulated devices
//SIMULATED Analog Device (LDR)
#define TOKEN_4 "OtZklw6GmlIHhCNVZMZg"
//SIMULATED AIR SENSOR 1 (MH Q-2)
#define TOKEN_5 "mcWd68OzWhO8tVFJvlzs"
//SIMULATED AIR SENSOR 2 (MH Q-135)
#define TOKEN_6 "aCWOKlzyH3HRqZAvIbJJ"

//Actuators (represented with leds)
//Water Pump 1
#define TOKEN_7 "H5yjoNcLG5bdk7GhoAFa"
//FAN - air renewal
#define TOKEN_8 "GwY6xIOfraG6PKo4RzK3"
//FAN - hardware cooling
#define TOKEN_9 "eFIyRRHkGvopebd1Q5ig"
//Light - photosynthesis booster 1
#define TOKEN_10 "z9ZdJD9KjSeqbeS91elH"
//Buzzer (real buzzer on breadboard)
#define TOKEN_11 "XjjqfxMeQVLcgrQAsgyr"

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
