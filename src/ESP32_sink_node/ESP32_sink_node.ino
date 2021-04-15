#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

// ACCESS POINT credentials
const char* ssidAP = "ESP32AP0";
const char* passwordAP = "123456789";
 
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
  unsigned int readingId;
};
 
struct message myMessage;
 
void onDataReceiver(const uint8_t * mac, const uint8_t *incomingData, int len) {
  // We don't use mac to verify the sender
  // Let us transform the incomingData into our message structure
 memcpy(&myMessage, incomingData, sizeof(myMessage));
  Serial.print(myMessage.id, len);
  Serial.print(",");
  Serial.print(myMessage.rss1);
  Serial.print(",");
  Serial.print(myMessage.rss2);
  Serial.print(",");
  Serial.print(myMessage.rss3);
  Serial.print(",");
  Serial.print(myMessage.rss4);
  Serial.print(",");
  Serial.print(myMessage.rss5);
  Serial.print(",");
  Serial.print(myMessage.rss6);
  Serial.print(",");
  Serial.print(myMessage.rss7);
  Serial.print(",");
  Serial.print(myMessage.rss8);
  Serial.print(",");
  Serial.print(myMessage.rss0);
  Serial.print(",");
  Serial.print(myMessage.lux);
  Serial.print(",");
  Serial.print(myMessage.readingId);
  Serial.println();
}
 
void setup() {
 Serial.begin(115200);
 WiFi.mode(WIFI_AP_STA);

 // Set device as an access point
 WiFi.softAP(ssidAP, passwordAP);   
 
 // Get Mac Add
 Serial.print("Mac Address: ");
 Serial.print(WiFi.macAddress());
 Serial.println("ESP32 ESP-Now Broadcast");
 
 // Initializing the ESP-NOW
 if (esp_now_init() != 0) {
   Serial.println("Problem during ESP-NOW init");
   return;
 }
 esp_now_register_recv_cb(onDataReceiver);
}
 
void loop() {
 // put your main code here, to run repeatedly:
}
