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
  
  digitalWrite(SOIL_PWR_PIN, HIGH); // apply power
  delay(10);
  moisture = analogRead(SOIL_PIN); //read the signal value from sensor
  digitalWrite(SOIL_PWR_PIN, LOW); // turn sensor power off
  return moisture; //send value
}


/*
 * Description: returns temperature value from DHT sensor
 *
 * Arguments:
 *
 *
 * Return:
 * int (units are in degrees Celsius)
 */
static bool measure_environment( float *temperature, float *humidity )
{
  static unsigned long measurement_timestamp = millis( );
  /* Measure once every four seconds. */
  if( millis( ) - measurement_timestamp > 3000ul )
  {
    if( dht_sensor.measure( temperature, humidity ) == true )
    {
      measurement_timestamp = millis( );
      return( true );
    }
  }
  return( false );
}

void readTemp()
{
  float temperature;
  float humidity;
  /* Measure temperature and humidity.  If the functions returns
     true, then a measurement is available. */
  if( measure_environment( &temperature, &humidity ) == true )
  {
    return temperature;
  }
}





/*
 * Description: Temporary function, does nothing atm. Holder until the
 *  settings funtion is written
 *  
 *  
 *  
 */
 int getUsrSetting()
 {
  
 }
