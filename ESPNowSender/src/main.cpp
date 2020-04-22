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

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0xB4, 0xA4, 0x70};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;
  float c;
  String d;
  bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void WiFiReset() {
  WiFi.persistent(false);
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}

void setup() {
  WiFiReset();
  // Init Serial Monitor
  WiFi.mode(WIFI_STA);
  delay(1000);
  Serial.begin(9600);
  // Set device as a Wi-Fi Station
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
  Firebase.reconnectWiFi(true);
  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // Serial.println(num);
  delay(3000);
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1,20);
  // Set values to send
  if (Firebase.getFloat(firebaseData, "/testNo")) {

    if (firebaseData.dataType() == "float") {
      Serial.println(firebaseData.floatData());
      myData.c = firebaseData.floatData();
    }

  } else {
    Serial.println(firebaseData.errorReason());
  }
  myData.d = "Hello";
  myData.e = false;

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}