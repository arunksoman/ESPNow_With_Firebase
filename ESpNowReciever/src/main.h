#include <Arduino.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

//1. Change the following info
#define FIREBASE_HOST "https://braille-2d53c.firebaseio.com/"
#define FIREBASE_AUTH "uUaUJ7acCrajKdp1bRjLN9CKeedor5E3ISlQ8Bkd"
#define WIFI_SSID "anuja1"
#define WIFI_PASSWORD "ar3k57u4"

FirebaseData firebaseData;

float num;
void setup(){
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  // Firebase.reconnectWiFi(true);
  //4. Enable auto reconnect the WiFi when connection lost
}
void loop(){
  //randomNumber = random(300);
  //Serial.print("random Number: ");
  // Serial.println(randomNumber);
  //delay(1000);
  // Firebase.setInt(firebaseData, "/testNo", 100);
  if (Firebase.getFloat(firebaseData, "/testNo")) {

    if (firebaseData.dataType() == "float") {
      Serial.println(firebaseData.floatData());
    }

  } else {
    Serial.println(firebaseData.errorReason());
  }
  // Serial.println(num);
  delay(3000);
}