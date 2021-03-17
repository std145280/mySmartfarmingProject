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

WiFiClient espClient4;
WiFiClient espClient5;
WiFiClient espClient6;

// Initialize Multiple ThingsBoard instances
ThingsBoard tb1(espClient1);
ThingsBoard tb2(espClient2);
ThingsBoard tb3(espClient3);

ThingsBoard tb4(espClient4);
ThingsBoard tb5(espClient5);
ThingsBoard tb6(espClient6);

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


//randomizer limits and values initialization
int lower = 1, upper = 5;
int randomNumber;
int randomLoop = 10;

float LDRSim;
float Air1;
float Air2;

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
  //random number generation
  randomNumber = (rand() % (upper - lower + 1)) + lower;

  if (randomLoop < 100) {
    randomLoop = (randomLoop + randomNumber * 2);
  } else {
    randomLoop = (randomLoop - randomNumber * 2);
  }


  digitalWrite(D8, HIGH);
  delay(90000);
  digitalWrite(D6, LOW);
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

  delay(100);
  if (!tb4.connected())
  {
    // Connect to the ThingsBoard with token 4 device
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
    // Connect to the ThingsBoard with token 5 device
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

  delay(100);
  if (!tb6.connected())
  {
    // Connect to the ThingsBoard with token 6 device
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN_6);
    if (!tb6.connect(THINGSBOARD_SERVER, TOKEN_6))
    {
      Serial.println("Failed to connect with token TOKEN_6");
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



  Serial.println("Random Number:" + randomNumber);

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  delay(100);
  tb1.sendTelemetryInt("LDR data", sensorValue);
  delay(100);
  tb2.sendTelemetryInt("DHT-11 temperature", temperature_DHT11);
  delay(100);
  tb3.sendTelemetryFloat("DHT-11 humidity", humidity_DHT11);

  //Simulated values telemetry
  delay(100);
  LDRSim = sensorValue + randomNumber;
  tb4.sendTelemetryInt("LDR data", LDRSim);
  Serial.println("LDR Simulated: " + String(LDRSim));
  randomNumber = (rand() % (upper - lower + 1)) + lower;
  Air1 = randomLoop;
  Air2 = randomLoop + randomNumber; //randomied second time


  tb5.sendTelemetryInt("MH Q-2", Air1 );
  delay(100);

  tb6.sendTelemetryInt("MH Q-135", Air2);

  Serial.println("MH Q-2: " + String(Air1));
  Serial.println("MH Q-135: " +  String(Air2));

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
  delay(100);
  tb6.loop();
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
