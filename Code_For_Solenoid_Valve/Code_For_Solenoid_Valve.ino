#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Config Firebase
#define FIREBASE_HOST "smart-w-ee697.firebaseio.com"
#define FIREBASE_AUTH "54oR8PtPPFJh4vgh7tHMIIQgoiSecYcYWBZY44ZB"

// Config connect WiFi
#define WIFI_SSID "Korn"
#define WIFI_PASSWORD "88888888999"

int valvepin = D0; 
void setup() {
  pinMode(valvepin, OUTPUT);
  digitalWrite(valvepin, HIGH);
  // put your setup code here, to run once:
  Serial.begin(9600);
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
  
}
 int a;
 
void loop() {
  {
  a=Firebase.getInt("Total/num");
  if(Firebase.failed()){
    Serial.println(Firebase.error());
   }
  else{
  Serial.println(a);
  
  }
 }
 digitalWrite(valvepin, HIGH);
 delay(1000);
 if(a<=30){
 digitalWrite(valvepin, LOW);
  delay(5000);
  digitalWrite(valvepin, HIGH);
  }
  else if (a<=50&&a>30)
  {digitalWrite(valvepin, LOW);
  delay(500);
  digitalWrite(valvepin, HIGH);
    }
}
