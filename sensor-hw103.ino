#include <WiFi.h>
int sensorPin= 4;

int value; // value of adc 0-4095 (0v-3.3v)



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readsensor();
  //value = constrain(value,1500,4095);    //%
  //value = map(value,1500,4095,100,0);    //%
  Serial.println(value);
  delay(500);
}
void readsensor(){
  value=analogRead(sensorPin); //the wetter the soil the lower the value 
  }
