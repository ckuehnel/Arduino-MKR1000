// This file contains the application code

void run_application()
{
  heartbeat();
  WiFi.noLowPowerMode();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    heartbeat();
  }
  
  String sTemp = "22.2";  // place here a sensor value
  String sHum = "55";     // place here a sensor value

  String message = sTemp + " degree celsius\r\n" + sHum + " percent r.H.";
  int len = message.length();
  char charBuf[len+1];
  message.toCharArray(charBuf, len+1);
  pushover(charBuf,0);  

  WiFi.end();
}