//sda -d1
//sck-d2

#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#define BLYNK_PRINT Serial


#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>

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
char auth[] = "a39b9e7a5fed457d929c9699852c1335";
char ssid[] = "MySSID";
char pass[] = "MyPassword";

float Reading;
WidgetLCD lcd1(V2);
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

    if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
  myMap.clear();
  if(!accel.begin())
  {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while(1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

    lcd1.clear();
    lcd1.print(0,0,"STATUS");
    delay(100);

  
}

void loop()
{

  while (Serial.available() > 0)
  {
    gps.encode(Serial.read());
    if (gps.location.isUpdated())
    {
    sensors_event_t event; 
    accel.getEvent(&event); 
    Reading=((event.acceleration.z)/9.8);
    if(Reading>-0.9)
    {
      Serial.print("POTHOLE DETECTED");
      Serial.println("Elevation");
      Serial.print(Reading);
      lcd1.print(0,0,"STATUS:");
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
      
       ThingSpeak.setField(1, Reading);
       ThingSpeak.setField(3, latt);
       ThingSpeak.setField(2, lonn);
       
       int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
       if(x == 200)
       {
         Serial.println("Channel update successful.");
        }
        else
        {
          Serial.println("Problem updating channel. HTTP error code " + String(x));
        }
        delay(20000); // Wait 20 seconds to update the channel again
 
        Serial.print("\n");
        lcd1.clear();
        myMap.clear();
        delay(2000);  
        Blynk.run();

    }
    } 
  }
}
