#include "BMP180.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  BMP_t BMP;
  Serial.println("Code running");
  Serial.println(BMP.calculateTemperature());
  //word temp=0xABCD;
  //BEtoLEConvert(temp);
  //Serial.println();
  //Serial.println(temp,HEX);
  Serial.println(BMP.calculatePressure());
}


void loop() {
  // put your main code here, to run repeatedly:
     
  
}
