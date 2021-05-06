#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include<iostream>
#include <Arduino.h>
#include "Wire.h"
#include "RTClib.h"
#include <stdio.h>
#include <string.h>
RTC_DS3231 RTC;

// Config Firebase
#define FIREBASE_HOST "smart-w-ee697.firebaseio.com"
#define FIREBASE_AUTH "54oR8PtPPFJh4vgh7tHMIIQgoiSecYcYWBZY44ZB"

// Config connect WiFi
#define WIFI_SSID "KK"
#define WIFI_PASSWORD "88888888999"

FirebaseData firebaseData2;
FirebaseData firebaseData1;
FirebaseData firebaseData;
int valvepin = D0;
void setup() {
 pinMode(valvepin, OUTPUT);
  digitalWrite(valvepin, LOW);
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin();
  RTC.begin();
  // put your setup code here, to run once:
 
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());



  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.setInt("device/led_control/red", 1);
}

String x;
String z;
int y;
bool j;
void loop() {
  // put your main code here, to run repeatedly:
 Firebase.getInt(firebaseData,"/Total/num");
 int MM=firebaseData.intData();
 
  DateTime now = RTC.now();
  char a[20]="";
  char b[20]="";
  int hm = int(now.minute()-MM);
   int h = int(now.hour());
  int m = int(now.minute());
  sprintf(b,"%02d:%02d",h,hm); 
  sprintf(a,"%02d:%02d",h,m); 
  String b_str = String(b);
  String a_str = String(a);
 
  Firebase.getBool(firebaseData1,"/swsapp/autoWatering/active");
  Firebase.getBool(firebaseData2,"/swsapp/watering");
  Firebase.getBool(firebaseData,"/swsapp/timer/active");
  bool j =firebaseData.boolData();
  Firebase.getString(firebaseData,"/swsapp/autoWatering/time1");
  String x=firebaseData.stringData();
  
 Firebase.getString(firebaseData,"/swsapp/autoWatering/time2");
  String z=firebaseData.stringData();
 Firebase.getString(firebaseData,"/swsapp/timer/startTime");
  String c=firebaseData.stringData();
  
 Firebase.getString(firebaseData,"/swsapp/timer/endTime");
  String d=firebaseData.stringData();
   
  //operate
if(firebaseData2.boolData() == 1) 
    {digitalWrite(valvepin, HIGH);}
    else if(firebaseData1.boolData() == 1)  
     {if(a_str==x||a_str==z)
     {
     digitalWrite(valvepin, HIGH); }}
       else if( j == 1&& a_str==c)
        digitalWrite(valvepin, HIGH);
     else if( b_str==x||b_str==z||a_str==d)
                      {digitalWrite(valvepin, LOW);}
      else if((firebaseData2.boolData() == 0)||(firebaseData1.boolData() == 0)||(firebaseData.boolData() == 0))
        digitalWrite(valvepin, LOW);
      
                   
    
                       
                       
                        
 } 
   /*  else if(firebaseData.boolData() == 0)
                          digitalWrite(valvepin, HIGH);
      else if(firebaseData1.boolData() == 0)
          {digitalWrite(valvepin, HIGH);}
      Firebase.getString(firebaseData1,"/swsapp/timer/startTime");
  String c=firebaseData.stringData();

   Firebase.getString(firebaseData,"/swsapp/timer/endTime");
  String d=firebaseData.stringData();
      Firebase.getBool(firebaseData,"/swsapp/timer/active");
    else if(firebaseData.boolData() == 0)
        digitalWrite(valvepin, HIGH);
        else if(a_str==d){
        digitalWrite(valvepin, HIGH);
       else if(firebaseData.boolData() == 1&& a_str==c)
        digitalWrite(valvepin, LOW);
        if(firebaseData1.boolData() == 1 && a_str==x ){
     digitalWrite(valvepin, LOW);
     Serial.println(firebaseData1.boolData());
     Firebase.setBool(firebaseData1,"/swsapp/watering",T);
    }
     if(firebaseData1.boolData() == 0 || b_str==x)
    {digitalWrite(valvepin, HIGH);
    Firebase.setBool(firebaseData1,"/swsapp/watering",F);
    }
    if (firebaseData2.boolData() == 0)   
     {digitalWrite(valvepin, HIGH);}*/
