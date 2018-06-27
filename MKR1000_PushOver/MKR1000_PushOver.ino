#include "arduino_secrets.h"
/*   Pushover sketch by M.J. Meijer 2014   Send pushover.net messages from the arduino*/#include <SPI.h>#include <WiFi101.h>char* ssid     = SECRET_SSID;char* password = SECRET_PW;// Pushover settingschar pushoversite[] = "api.pushover.net";char apitoken[] = "aeneokkaiz6iywxhpzigzug9khzi33";char userkey [] = "uojfahpyozqtj6gbgccg61e5jngg93";int length;WiFiClient client;void setup() {  Serial.begin(115200);  while(!Serial);  Serial.print(F("Starting WiFi connection..."));  delay(2000);  Serial.println();  Serial.print("Connecting to WiFi - ");  Serial.println(ssid);  WiFi.begin(ssid, password);  while (WiFi.status() != WL_CONNECTED) {    delay(500);    Serial.print(".");  }  Serial.println("");  Serial.println("WiFi connected");  Serial.print("IP address: ");  Serial.println(WiFi.localIP());}void loop(){  String sTemp = "22.2";  // place here a sensor value  String sHum = "55";     // place here a sensor value  String message = sTemp + " degree celsius\r\n" + sHum + " percent r.H.";  //Serial.print(message);  //Serial.print("   ");    int len = message.length();  // Serial.println(len);    char charBuf[len+1];  message.toCharArray(charBuf, len+1);  Serial.println(charBuf);  pushover(charBuf,0);    delay(60000);           // delay 1 minute}byte pushover(char *pushovermessage, int priority){  String message = pushovermessage;  length = 113 + message.length();  if(client.connect(pushoversite,80))  {    client.println("POST /1/messages.json HTTP/1.1");    client.println("Host: api.pushover.net");    client.println("Connection: close\r\nContent-Type: application/x-www-form-urlencoded");    client.print("Content-Length: ");    client.print(length);    client.println("\r\n");;    client.print("token=");    client.print(apitoken);    client.print("&user=");    client.print(userkey);    client.print("&message=");    client.print(message);    client.print("&priority=");    client.print(priority);    client.print("&retry=60");    client.print("&expire=3600");    while(client.connected())      {      while(client.available())      {        char ch = client.read();        Serial.write(ch);      }    }    client.stop();  }  }