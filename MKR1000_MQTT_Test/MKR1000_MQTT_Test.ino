#include "arduino_secrets.h"
/*
  MKR1000_MQTT_Test
  (c) Claus Kuehnel 2018-05-27 info@ckuehnel.ch


*/

#include <WiFi101.h>
#include <PubSubClient.h>
#include "DHT.h"

// WiFi
const char* mySSID = SECRET_SSID;
const char* myPW = SECRET_PW;

// MQTT (CloudMQTT)
const char* BROKER = "m20.cloudmqtt.com";
uint16_t    BRPORT = 12394;
const char* BRUSER = "raxxxid";
const char* BRPWD  = "ah52xxxd8JS";
const char* CLIENTID = "MKR1000";

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
#define uS_TO_S_FACTOR 1000000

//int DEEPSLEEP_SECONDS = 1800; // 30 min
int DEEPSLEEP_SECONDS = 180; // 3 min

/* create an instance of PubSubClient client */
WiFiClient MKR1000Client;
PubSubClient client(MKR1000Client);

uint64_t chipid;

const int dhtpin = 22;


// Initialize DHT sensor.
DHT dht(dhtpin, DHTTYPE);

// Temporary variables
static char celsiusTemp[7];
static char humidityTemp[7];
char msg[20];

/* topics */
#define TEMP_TOPIC    "MKR1000/temp"
#define HUMI_TOPIC    "MKR1000/humi"

// Client variables 
char linebuf[80];
int charcount=0;

char deviceid[21];

void setup() 
{
  //dht.begin();
  
  Serial.begin(115200);
  while(!Serial);               // wait for monitor

  pinMode(LED_BUILTIN, OUTPUT); // on-board LED
  digitalWrite(LED_BUILTIN, HIGH);  

  connectWiFi();
  configureMQTT();
}

void loop() 
{
  char body[1024];
  digitalWrite(LED_BUILTIN, HIGH); //switched on

  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  }

  sensorsData(body);
  delay(50);
  digitalWrite(LED_BUILTIN, LOW); //switched off
  //WiFi.disconnect();
  //Serial.println("Going to Deep Sleep..."); esp_deep_sleep_start();    // uncomment for deep sleep
  delay(10000);               // used for test
}

void sensorsData(char* body)
{
  //This section reads all sensors
  
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  //float humidity = dht.readHumidity();
  float humidity = random(30,80);
  if (!isnan(humidity)) 
  {
    snprintf (msg, 20, "%5.1f", humidity);
    /* publish the message */
    client.publish(HUMI_TOPIC, msg);
  }
  
  // Read temperature as Celsius (the default)
  //float temperature = dht.readTemperature();
  float temperature = random(15,35);
  if (!isnan(temperature)) 
  {
    snprintf (msg, 20, "%5.1f", temperature);
    /* publish the message */
    client.publish(TEMP_TOPIC, msg);
  }
 
  Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" *C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %rF");
}

