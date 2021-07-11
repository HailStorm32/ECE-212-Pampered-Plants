//---------------Begin Global Variables-----------------------------//
const uint8_t SOIL_PIN = A2; // set signal pin to analog (any analog pin works)
const uint8_t SOIL_PWR_PIN = 7; // sensor powered through digital pin to reduce corrosion (any digital pin works)
const uint8_t LDR_1_PIN = A0;
const uint8_t LDR_2_PIN = A1;


//---------------Begin Includes------------------------------------//
#include "functions.h"
  
void setup() 
{
  Serial.begin(9600);


  pinMode(SOIL_PWR_PIN, OUTPUT); 
  digitalWrite(SOIL_PWR_PIN, LOW); // set default to no power, reduces corrosion on moisture sensor
}


void loop() {


}
