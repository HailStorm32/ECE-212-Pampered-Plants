#include "functions.h"

void setup() 
{
  Serial.begin(9600);
  
  // setup for readSoil function
  int moisture = 0; // initialize output variable
  const int soilPin = A2; // set signal pin to analog (any analog pin works)
  const int soilPower = 7; // sensor powered through digital pin to reduce corrosion (any digital pin works)
  pinMode(soilPower, OUTPUT); // set digital pin as output
  digitalWrite(soilPower, LOW); // set default to no power, reduces corrosion on sensor
  // end setup for readSoil function
}



void loop() {


}
