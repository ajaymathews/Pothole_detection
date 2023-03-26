
/* PINS
 *  SCL-D1
 *  SDA -D2 
 *  VCC-3.3V
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
char ssid[] = "MySSID";
char pass[] = "MyPassword";

float Reading;
WidgetLCD lcd1(V2);
float Latitude;
float Longitude;
int inde;


void maplot(float Latitude,float Longitude)
{
        myMap.location(inde,Latitude,Longitude,"1");
        delay(1000);
}
void setup()
{
  Blynk.begin(auth, ssid, pass);
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client); 
  Serial.begin(9600);
  Wire.begin(4,5);
  //ESP.wdtDisable();
  
  myMap.clear();
  Serial.println("VL53L1X Qwiic Test");

  if (distanceSensor.begin() == true)
  {
    Serial.println("Sensor online!");
  }

    lcd1.clear();
    lcd1.print(0,0,"LIDAR");
    delay(100);
}

void loop()
{     
      //delay(1000);
      gps.encode(Serial.read());
      distanceSensor.startRanging(); //Write configuration bytes to initiate measurement
      int distance = distanceSensor.getDistance(); //Get the result of the measurement from the sensor
      distanceSensor.stopRanging();
      Serial.print("distance");
      Serial.println(distance);

      if(distance>300)
    {
      Serial.println("POTHOLE DETECTED");
      lcd1.print(0,0,"LIDAR:");
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
       ThingSpeak.setField(4,distance);
       
       int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
       if(x == 200)
       {
         Serial.println("Channel update successful.");
       }
       else
       {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
        //Blynk.run();
        delay(20000); 
        
        Serial.print("\n");
        lcd1.clear();
        myMap.clear();     

      }    
      // myMap.clear();
        Blynk.run(); 
      } 
