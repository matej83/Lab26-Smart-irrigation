/*
transmitter

*/

#include <esp_now.h>
#include <WiFi.h>

int id = 1; // change to board id

int sensorPin1= 34; //A0 to esp32 GPIO pin 34
int sensorPin2= 35; //A0 to esp32 GPIO pin 33
int sensorPin3= 32; //A0 to esp32 GPIO pin 32
int sensorPin4= 33; //A0 to esp32 GPIO pin 31
int sensorValue1; // value of ADC 0-4095 (0v-3.3v)
int sensorValue2; // value of ADC 0-4095 (0v-3.3v)
int sensorValue3; // value of ADC 0-4095 (0v-3.3v)
int sensorValue4; // value of ADC 0-4095 (0v-3.3v)

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x24, 0x0A, 0xC4, 0x5E, 0xE8, 0x7C};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id; // must be unique for each sender board
    int h1;
    int h2;
    int h3;
    int h4;
} struct_message;


struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
void readsensor(){
  sensorValue1 =analogRead(sensorPin1); //read sensor value (the wetter the soil the lower the value.)
  sensorValue2 =analogRead(sensorPin2); //read sensor value (the wetter the soil the lower the value.)
  sensorValue3 =analogRead(sensorPin3); //read sensor value (the wetter the soil the lower the value.)
  sensorValue4 =analogRead(sensorPin4); //read sensor value (the wetter the soil the lower the value.)
  }
 
void setup() {
  Serial.begin(115200);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
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
 
  readsensor();
  // Set values to send
  myData.id = id;
  myData.h1 = sensorValue1;
  myData.h2 = sensorValue2;
  myData.h3 = sensorValue3;
  myData.h4 = sensorValue4;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10000);
}
