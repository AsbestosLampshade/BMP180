#include "BMP180.h"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  BMP_t BMP;
  Serial.println("Code running");
  Serial.print("The temperature is:");
  Serial.print(BMP.getTemperature());
  Serial.println();
  Serial.print("The Pressure is:");
  Serial.print(BMP.getPressure());
  Serial.println();
}


void loop() {
  // put your main code here, to run repeatedly:
}
