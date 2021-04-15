/**
 * ESP-NOW: ESP32 Broadcast with ESP-Now
 * 
 * 
*/
#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>
#include <Wire.h>
#include <BH1750.h>

BH1750 lightMeter;

// ACCESS POINT credentials
const char* ssidAP = "ESP32AP5";
const char* passwordAP = "123456789";

// ESP8266 Mac address (first peer)
uint8_t mac_peer[] = {0xFC, 0xF5, 0xC4, 0x31, 0xA6, 0x28};

// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 5

typedef struct message {
    int id;
    float rss0;
    float rss1;
    float rss2;
    float rss3;
    float rss4;
    float rss5;
    float rss6;
    float rss7;
    float rss8;
    float lux;
    int readingId;
};
 
struct message myMessage;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 2000;        // Interval at which to publish sensor readings

unsigned int readingId = 0;

float rssAP0;
float rssAP1;
float rssAP2;
float rssAP3;
float rssAP4;
float rssAP5;
float rssAP6;
float rssAP7;
float rssAP8;
void handleADC() {
  // scan network
  int i  = 0;
  bool ulang = 1;
  while (ulang == 1) {
    int numberOfNetworks = WiFi.scanNetworks();
    for (int n = 0; n < numberOfNetworks; n++) {
      if (WiFi.SSID(n) == "ESP32AP0") {
        rssAP0 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP0);
      }
      else if (WiFi.SSID(n) == "ESP32AP1") {
        rssAP1 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP1);
      }
      else if (WiFi.SSID(n) == "ESP32AP2") {
        rssAP2 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP2);
      }
      else if (WiFi.SSID(n) == "ESP32AP3") {
        rssAP3 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP3);
      }
      else if (WiFi.SSID(n) == "ESP32AP4") {
        rssAP4 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP4);
      }
      else if (WiFi.SSID(n) == "ESP32AP5") {
        rssAP5 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP5);
      }
      else if (WiFi.SSID(n) == "ESP32AP6") {
        rssAP6 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP6);
      }
      else if (WiFi.SSID(n) == "ESP32AP7") {
        rssAP7 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP7);
      }
      else if (WiFi.SSID(n) == "ESP32AP8") {
        rssAP8 = float(WiFi.RSSI(n));
        i++;
        Serial.println(rssAP8);
      }
    }
    Serial.println(i);
    Serial.println("--------------------------");
    if (i == 8) {
      ulang = 0;
    }
    else if (i != 8) {
      i = 0;
    }

  }
}
 
void onSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last Packet Send Status: ");
  if (status == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

void setup() {
  Serial.begin(115200);
  
  Wire.begin();
  lightMeter.begin();
  
  WiFi.mode(WIFI_AP_STA);
  
 
  // Set device as an access point
  WiFi.softAP(ssidAP, passwordAP);  
  
  // Get Mac Add
  Serial.print("Mac Address: ");
  Serial.print(WiFi.macAddress());
  Serial.println("ESP32 ESP-Now Broadcast");
  
    // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(onSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, mac_peer, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
void loop() {
  handleADC();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
  // Save the last time a new reading was published
  previousMillis = currentMillis;
  //Set values to send
  myMessage.id = BOARD_ID;
  myMessage.rss1 = rssAP1;
  myMessage.rss2 = rssAP2;
  myMessage.rss3 = rssAP3;
  myMessage.rss4 = rssAP4;
  myMessage.rss5 = rssAP5;
  myMessage.rss6 = rssAP6;
  myMessage.rss7 = rssAP7;
  myMessage.rss8 = rssAP8;
  myMessage.rss0 = rssAP0;
  myMessage.lux  = lightMeter.readLightLevel();
  myMessage.readingId = readingId++;
  Serial.println("Send a new message");
  esp_now_send(NULL, (uint8_t *) &myMessage, sizeof(myMessage));
}
}
