#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Config Firebase
#define FIREBASE_HOST "smart-w-ee697.firebaseio.com"
#define FIREBASE_AUTH "dHwzE0AvNPmm8X2eOHvsy6fcTFUyGIwpDxiahPSo"

// Config connect WiFi
#define WIFI_SSID "LastlB"
#define WIFI_PASSWORD "lonieusO12"

 int value=0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(WiFi.localIP());
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  

  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.setInt("device/moisture/status", 0);
}
 
void loop() {
  int value = analogRead(A0);
  Serial.println(value);
  Firebase.setInt("device/moisture/status", value);
  delay(200);
}

  
