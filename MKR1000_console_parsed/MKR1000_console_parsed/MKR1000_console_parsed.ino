#include "arduino_secrets.h"
/**
  Arduino MKR1000 Weather
  
  based on
  ESP8266 -  Weather forecast from https://openweathermap.org/
  v. 1.0
  Copyright (C) 2018 Robert Ulbricht
  https://www.arduinoslovakia.eu
  
  minor modifications 2018-06-26 by Claus Kuehnel info@ckuehnel.ch
  
  IDE: 1.8.5 or higher

  Board: Arduino MKR1000

  Libraries:
  Arduino JSON library: https://github.com/bblanchon/ArduinoJson
  Version: 5.13.0 or higher

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// WiFi - Version: 1.2.6
#include <SPI.h>
#include <WiFi101.h>
#include <ArduinoJson.h>

char* ssid     = SECRET_SSID;
char* password = SECRET_PW;
char* appid    = SECRET_APPID;

#define DEBUG 0  // Select debug mode by 1

//http://api.openweathermap.org/data/2.5/weather?q=Altendorf,CH&units=metric&appid=87286fed489ba6a8afcd81443138d510
const char* host = "api.openweathermap.org";
const char* url = "/data/2.5/weather";
const char* openweathermapq = "Altendorf, CH"; // chosse your location - here "Altendorf, CH"
const char* openweathermapunits = "metric"; // Celsius

String param = String(url) +
               "?q=" + openweathermapq +
               "&units=" + openweathermapunits +
               "&appid=" + appid;

String line;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Get weather data from Openweathermap.org...");
  Serial.print("Connecting to WiFi - ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("requesting URL: ");
  Serial.print(host);
  Serial.println(param);

  client.print(String("GET ") + param + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: MKR1000\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");

  line = client.readStringUntil('\n');
  if (line != "HTTP/1.1 200 OK\r") {
    Serial.print("Unexpected response: ");
    Serial.println(line);
    return;
  }

  if (client.find("\r\n\r\n")) {
    DynamicJsonBuffer jsonBuffer(4096);
    JsonObject& root = jsonBuffer.parseObject(client);
    if (DEBUG) 
    {
      root.prettyPrintTo(Serial);
      Serial.println();
    }

    // parsed output
    const char* name = root["name"];
    Serial.print("City: ");Serial.println(name);

    const double temp = root["main"]["temp"];
    char buffer[64];
    sprintf(buffer, "Temperature: %.01f °C", temp);
    Serial.println(buffer);

    const int humidity = root["main"]["humidity"];
    sprintf(buffer, "Humidity: %d %% r.H.", humidity);
    Serial.println(buffer);

    const int pressure = root["main"]["pressure"];
    sprintf(buffer, "Pressure: %d hPa", pressure);
    Serial.println(buffer);

    const double wind = root["wind"]["speed"];
    sprintf(buffer, "Wind speed: %.02f m/s", wind);
    Serial.println(buffer);

    const char* weather = root["weather"][0]["main"];
    const char* description = root["weather"][0]["description"];
    sprintf(buffer, "Weather: %s (%s)", weather, description);
    Serial.println(buffer);
  }
}

void loop() {
}
