  /* LDR - Light Dependent Resistor or Photoresistor sensor
   *  
   */
  int ldrPin1 = A0; // LDR1 on Analog Pin 0

  int ldrPin2 = A1; // LDR2 on Analog Pin 1

  uint8_t ldrValue1 = 0; // Value read from the LDR1 converts analog signal to digital scaled signal of 0 - 255 which is brightest light to darkest light

  uint8_t ldrValue2 = 0; // Value read from the LDR2 converts analog signal to digital scaled signal of 0 - 255 which is brightest light to darkest light


void setup() 
{
  Serial.begin(9600);
}

void loop()
{
  ldrValue1 = analogRead(ldrPin1); // read the value from the LDR 
  
  Serial.println( "LDR value1: " ); // prints the words LDR value: and then goes to next line
  
  Serial.println( ldrValue1 ); // 
  
  delay(1000); // delays 

  ldrValue2 = analogRead(ldrPin2); // read the value from the LDR 
  
  Serial.println( "LDR value2: " ); // prints the words LDR value: and then goes to next line
  
  Serial.println( ldrValue2 ); // prints the 0-5VDC input as a digital 0 - 1023 reading of the sensor
  
  delay(1000); // delays 

}
