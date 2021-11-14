/*
receiver
*/

#include <esp_now.h>
#include <WiFi.h>

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
  int id;
  int h1;
  int h2;
  int h3;
  int h4;
//int y;
}struct_message;

// Create a struct_message called myData
struct_message myData;

// Create a structure to hold the readings from each board
struct_message board1;
struct_message board2;
struct_message board3;

// Create an array with all the structures
struct_message boardsStruct[3] = {board1, board2, board3};

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
  // Update the structures with the new incoming data
  boardsStruct[myData.id-1].h1 = myData.h1;
  boardsStruct[myData.id-1].h2 = myData.h2;
  boardsStruct[myData.id-1].h3 = myData.h3;
  boardsStruct[myData.id-1].h4 = myData.h4;
  Serial.printf("h1 value: %d \n", boardsStruct[myData.id-1].h1);
  Serial.printf("h2 value: %d \n", boardsStruct[myData.id-1].h2);
  Serial.printf("h3 value: %d \n", boardsStruct[myData.id-1].h3);
  Serial.printf("h4 value: %d \n", boardsStruct[myData.id-1].h4);
  Serial.println();
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // Acess the variables for each board
  int board1H1 = boardsStruct[0].h1;
  int board1H2 = boardsStruct[0].h2;
  int board1H3 = boardsStruct[0].h3;
  int board1H4 = boardsStruct[0].h4;
  

  int board2X = boardsStruct[1].h1;
  int board2H2 = boardsStruct[1].h2;
  int board2H3 = boardsStruct[1].h3;
  int board2H4 = boardsStruct[1].h4;
  
 // int board3X = boardsStruct[2].x;
 // int board3Y = boardsStruct[2].y;
  
  

  delay(10000);  
}
