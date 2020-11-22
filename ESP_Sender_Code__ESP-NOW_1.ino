/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp-now-wi-fi-web-server/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <espnow.h>
#include <ESP8266WiFi.h>

// Replace with your network credentials (STATION)
const char* ssidStation = "ESP32-Server";
const char* passwordStation = "123456789";

// ACCESS POINT credentials
const char* ssidAP = "ESP32-Access-Point-1";
const char* passwordAP = "123456789";


// Set your Board ID (ESP32 Sender #1 = BOARD_ID 1, ESP32 Sender #2 = BOARD_ID 2, etc)
#define BOARD_ID 1

//MAC Address of the receiver 
uint8_t broadcastAddress[] = {0xFC, 0xF5, 0xC4, 0x94, 0x2E, 0xCE};

//Wi-Fi channel (must match the gateway wi-fi channel as an access point)
#define CHAN_AP 2

//Structure example to send data
//Must match the receiver structure
typedef struct struct_message {
    int id;
    float rss1;
    float rss2;
    float rss3;
    float rss4;
    float rss5;
    float rss6;
    float rss7;
    float rss8;
    float rss9;
    int readingId;
} struct_message;

//Create a struct_message called myData
struct_message myData;

unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 500;        // Interval at which to publish sensor readings

unsigned int readingId = 0;

float rssAP1;
float rssAP2;
float rssAP3;
float rssAP4;
float rssAP5;
float rssAP6;
float rssAP7;
float rssAP8;
float rssAP9;
void handleADC(){
    // scan network
  int numberOfNetworks = WiFi.scanNetworks(); 
  for (int i = 0; i < numberOfNetworks; i++) {
    if (WiFi.SSID(i) == "ESP32-Access-Point-1"){
      rssAP1 = float(WiFi.RSSI(i));
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-2"){
      rssAP2 = float(WiFi.RSSI(i));
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-3"){
      rssAP3 = float(WiFi.RSSI(i));
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-4"){
      rssAP4 = float(WiFi.RSSI(i));      
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-5"){
      rssAP5 = float(WiFi.RSSI(i));      
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-6"){
      rssAP6 = float(WiFi.RSSI(i));      
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-7"){
      rssAP7 = float(WiFi.RSSI(i));      
    }
    else if (WiFi.SSID(i) == "ESP32-Access-Point-8"){
      rssAP8 = float(WiFi.RSSI(i));      
    }
    else if (WiFi.SSID(i) == "ESP32-Server"){
      rssAP9 = float(WiFi.RSSI(i));      
    }
  }
  }

// callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
  }
}

 
void setup() {
  //Init Serial Monitor
  Serial.begin(115200);
 
  // Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(ssidStation, passwordStation);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Setting as a Wi-Fi Station..");
  }
  Serial.print("Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Set device as an access point
  WiFi.softAP(ssidAP, passwordAP);  

  //Init ESP-NOW
  if (esp_now_init()==0) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  
  //Add peer        
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
}
 
void loop() {
  handleADC();
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // Save the last time a new reading was published
    previousMillis = currentMillis;
    //Set values to send
    myData.id = BOARD_ID;
    myData.rss1 = rssAP1;
    myData.rss2 = rssAP2;
    myData.rss3 = rssAP3;
    myData.rss4 = rssAP4;
    myData.rss5 = rssAP5;
    myData.rss6 = rssAP6;
    myData.rss7 = rssAP7;
    myData.rss8 = rssAP8;
    myData.rss9 = rssAP9;
    myData.readingId = readingId++;
     
    //Send message via ESP-NOW
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
  }
}
