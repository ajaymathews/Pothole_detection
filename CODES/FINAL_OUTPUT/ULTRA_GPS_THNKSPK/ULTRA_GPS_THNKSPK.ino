
/* PINS
 *  TRIG-D4
 *  ECHO -D5 
 *  VCC-Vin
 *  GND -GND
 */

#include <ComponentObject.h>
#include <RangeSensor.h>
#include <SparkFun_VL53L1X.h>
#include <vl53l1x_class.h>
#include <vl53l1_error_codes.h>

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

#include <Wire.h>
#include "SparkFun_VL53L1X.h"

int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 757795;
const char * myWriteAPIKey = "P6B8P3UEUXX44PMN";

// Initialize our values
String myStatus = "";

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

TinyGPSPlus gps;
WidgetMap myMap(V0);
SFEVL53L1X distanceSensor;

char auth[] = "a39b9e7a5fed457d929c9699852c1335";
char ssid[] = "ssid";
char pass[] = "MyPassword";
const int trigPin = 2;
const int echoPin = 14;
long duration;
int distance;
WidgetLCD lcd1(V2);
int inde;


void maplot(float Latitude,float Longitude)
{
        myMap.location(inde,Latitude,Longitude,"1");
        delay(1000);
}
void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);
  Blynk.begin(auth, ssid, pass);
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client); 
  Serial.begin(9600);
  myMap.clear();
}

void loop()
{
    gps.encode(Serial.read());
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);

      duration = pulseIn(echoPin, HIGH);

      distance= duration*0.034/2;
 
      lcd1.print(1,1,distance);
      Serial.println(distance);
      delay(100);
      if(distance>15)
      {
      Serial.print("POTHOLE DETECTED");
      Serial.print("Distance(Cm): ");
      Serial.println(distance);
      lcd1.print(0,0,"ULTRA_SONIC:");
      lcd1.print(7,0,"POTHOLE");
      lcd1.print(1,1,"DETECTED");
      
      Serial.print("Latitude= "); 
      float latt =gps.location.lat();   
      Serial.println(latt); 
      Serial.print(" Longitude= "); 
      float lonn =gps.location.lng(); 
      Serial.println(lonn);
      delay(300);
      
       myMap.location(inde,latt,lonn,"1");
      
       ThingSpeak.setField(1, latt);
       ThingSpeak.setField(2, lonn);
       ThingSpeak.setField(5, lonn);
       
       int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
       if(x == 200)
       {
         Serial.println("Channel update successful.");
       }
       else
       {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
        delay(20000); 
        
        Serial.print("\n");
        lcd1.clear();
        myMap.clear();   
      }
        Blynk.run();

      }
