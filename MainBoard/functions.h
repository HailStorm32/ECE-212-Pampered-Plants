/*
 * Please use the following comment block above your fuction:
 */

/*
 * Description: Should provide a breif description of
 *   what the fuction does
 *  
 * Arguments:
 *   (INPUT/OUTPUT) argumentName -- short one line description of argument
 *   
 *  Return:
 *    returnType(void, int, uint8_t...) -- short one line description of what value(s) it returns
 */

 //EXAMPLE

/*
 * Description: sends a pre packaged message to the given address using the
  *  address graph (if it has been set up) to send out messages
  *  
  * Arguments:
  *   (INPUT) radioP -- pointer to radio instance
  *   (INPUT) packedMessage -- prepacked message to send
  *   (INPUT) receivingAddr -- address to send message to
  *   
  * Return:
  *  true -- was able to send to a hub
  *  false -- wasnt able to send to any hub
  */



//============================= Functions Begin Below This Line ===================================================


/*
 * Description: Takes values from two photo resistors, 
 *  averages them, and maps them to values 0-255  
 * 
 * Arguments:
 *   none
 *   
 * Return:
 *   uint8_t -- averaged light intensity from 0-255, with 255 being brightest
 */
uint8_t getLightReading()
{
  const uint8_t LDR_1_PIN = A0;
  const uint8_t LDR_2_PIN = A1;
      
  uint8_t ldrValue1 = 0;
  uint8_t ldrValue2 = 0;

  //Get the values from the sensors and map them to 0-255
  ldrValue1 = map(analogRead(LDR_1_PIN), 0, 1023, 0, 255);
  ldrValue2 = map(analogRead(LDR_2_PIN), 0, 1023, 0, 255);

  return ((ldrValue1+ldrValue2)/2); //Return the average of the two sensors
}


/*
 * Description: Pulls reading from moisture sensor
 *
 * Arguments:
 *  none
 *
 * Return:
 *  int -- minimum is 0, max unknown at this time
 *
 * Note:
 *  Initial calibration performed with 5V vcc, expected values will decrease as supply voltage is lowered
 *  Dry: 0-300
 *  Middle dryness: 400-600
 *  Saturated (moist): 800-1000
 */

int readSoil()
{
  int moisture = 0; // initialize output variable
  int soilPin = A2; // set signal pin to analog
  int soilPower = 7; // sensor powered through digital pin to reduce corrosion
  pinMode(soilPower, OUTPUT); // set digital pin as output
  
  digitalWrite(soilPower, HIGH); // apply power
  delay(10);
  moisture = analogRead(soilPin); //read the signal value from sensor
  digitalWrite(soilPower, LOW); // turn sensor power off
  return moisture; //send value
}
