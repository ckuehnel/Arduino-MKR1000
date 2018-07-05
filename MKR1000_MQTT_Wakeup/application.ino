// This file contains the application code

void run_application()
{
  heartbeat();
  WiFi.noLowPowerMode();
  WiFi.begin(mySSID, myPW);
  while (WiFi.status() != WL_CONNECTED) 
  {
    heartbeat();
  }
    /* if client was disconnected then try to reconnect again */
  if (!client.connected()) {
    mqttconnect();
  } 
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
 

  WiFi.end();
}