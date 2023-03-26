/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "b2bd616492d34b38b14fe3d03656b79f";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ssiD";
char pass[] = "123456789";
float Elevation;
float Reading;
WidgetLCD lcd1(V2);

void setup()
{
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
  
}

void loop()
{
  lcd1.clear();
  lcd1.print(0,0,"AccReading");
  delay(100);

  sensors_event_t event; 
  accel.getEvent(&event); 
  Reading=((event.acceleration.z)/9.8);
  Serial.print("elevation");
  Serial.print(Reading);
  if(Reading>-0.4)
  {
   Serial.print("POTHOLE DETECTED");
   lcd1.print(1,1,"POTHOLE DETECTED");
   delay(100);
  }
  Serial.print("\n");

  Blynk.run();  
}
