#include <esp_now.h>
#include <WiFi.h>
#include <FirebaseESP32.h>
#include <esp_wifi.h>
//1. Change the following info
#define FIREBASE_HOST "https://braille-2d53c.firebaseio.com/"
#define FIREBASE_AUTH "uUaUJ7acCrajKdp1bRjLN9CKeedor5E3ISlQ8Bkd"
#define WIFI_SSID "anuja1"
#define WIFI_PASSWORD "ar3k57u4"

FirebaseData firebaseData;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    String d;
    bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  // Firebase.setInt(firebaseData, "/RandomValue", myData.b);
  Serial.print("Float: ");
  Serial.println(myData.c);
  Serial.print("String: ");
  Serial.println(myData.d);
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.println();
}

void WiFiReset() {
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void setup() {
  WiFiReset();
  // Initialize Serial Monitor
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.begin(9600);
  //Force espnow to use same channel as network
  uint8_t primaryChan = 0;
  wifi_second_chan_t secondChan = WIFI_SECOND_CHAN_NONE;
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_promiscuous(true));
  esp_wifi_set_channel(primaryChan, secondChan);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 0);
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

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  
  
}
 
void loop() {
  Firebase.setString(firebaseData, "/RandomValue", myData.d);
  Serial.println("data Send successfully");
  delay(4000);
}