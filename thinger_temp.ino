//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Sending and Receiving Thinger IO
//----------------------------------------Include Library
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>
#include "DHT.h"
//----------------------------------------

//----------------------------------------Thinger IO configuration
#define USERNAME "matejknific"
#define DEVICE_ID "esp"
#define DEVICE_CREDENTIAL "***"
//----------------------------------------

#define ON_Board_LED 2  //--> Defining an On Board LED, used for indicators when the process of connecting to a wifi router. GPIO2 = D4.

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL); //--> Initialize Thinger IO (ThingerESP8266)

//----------------------------------------SSID and Password of your WiFi Router/Hotspot.
const char* ssid = "SSid"; //--> Your wifi name or SSID.
const char* password = "****"; //--> Your wifi password.
//----------------------------------------

//----------------------------------------DHT11 Sensor Configuration
#define DHTPIN D1 //--> Digital pin connected to the DHT sensor. D1 = GPIO5.
#define DHTTYPE DHT11 //--> DHT11

DHT dht11(DHTPIN, DHTTYPE); //--> DHT11 Sensor Initialization
//----------------------------------------

float temperature,humidity; //--> Variables for temperature and humidity data

#define LEDGreenPin D5 //--> Green LED pin. D5 = GPIO14.
#define LEDBluePin D6 //--> Blue LED pin. D6 = GPIO12

void setup() {
  // put your setup code here, to run once:

  pinMode(ON_Board_LED,OUTPUT); //--> On Board LED port Direction output
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off Led On Board

  pinMode(LEDGreenPin, OUTPUT);
  pinMode(LEDBluePin, OUTPUT);

  WiFi.begin(ssid, password); //--> Connect to your WiFi router

  //----------------------------------------Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    //----------------------------------------Make the On Board Flashing LED on the process of connecting to the wifi router.
    digitalWrite(ON_Board_LED, LOW);
    delay(250);
    digitalWrite(ON_Board_LED, HIGH);
    delay(250);
    //----------------------------------------
  }
  //----------------------------------------
  
  digitalWrite(ON_Board_LED, HIGH); //--> Turn off the On Board LED when it is connected to the wifi router.

  thing.add_wifi(ssid, password); //--> Initialize wifi

  dht11.begin(); //--> Starts reading DHT11 Sensor
  
  //----------------------------------------Sends DHT11 Sensor data (Temperature and Humidity) to Thinger IO
  // Symbol or operator ">>" means to transmit data
  thing["dht11"] >> [](pson& out){
      out["temperature"] = temperature;
      out["humidity"] = humidity;
  };
  //----------------------------------------

  //----------------------------------------Receive data from Thinger IO to turn on or off the LED
  // Symbol or Operator "<<" means to receive data
  thing["LEDGreen"] << digitalPin(LEDGreenPin);
  thing["LEDBlue"] << digitalPin(LEDBluePin);
  //----------------------------------------
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // call always the thing handle in the loop and avoid any delay here
  thing.handle();
  
  //----------------------------------------To get temperature and humidity data from the DHT11 sensor
  temperature = dht11.readTemperature();
  humidity = dht11.readHumidity();
  //----------------------------------------
}
