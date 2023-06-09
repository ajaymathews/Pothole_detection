
#define BLYNK_PRINT Serial
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] ="90a310c54349404da713f1e62adbdcf5";
// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ssid";
char pass[] = "123456789";
WidgetMap myMap(V0);
void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);

  // If you want to remove all points:
  //myMap.clear();

  int index = 1;
  float lat = 51.5074;
  float lon = 0.1278;
  myMap.location(index, lat, lon, "value");
}

void loop()
{
  Blynk.run();
}
