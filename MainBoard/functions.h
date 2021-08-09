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
 * Description: Detects the level of water with a magnetic float and a magnetic switch  
 * 
 * Arguments:
 *   none
 *   
 * Return:
 *   bool -- getWaterLevel which is false for water full and true for water Low
 */              
bool getWaterLevel()
 
{
    bool waterLevel = false;
    waterLevel = digitalRead(WATER_LEVEL); 
  if (waterLevel == false) 
  { 
    return (waterLevel);  // "WATER LEVEL - HIGH")
  } 
  else 
  { 
    return (waterLevel); // "WATER LEVEL - LOW"
  } 
}


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
 * Description: Pumps a number of half-cups of water with delays of 10 seconds in between pumps.  
 * 
 * Arguments:
 *   int halfCupOfWater
 *   
 * Return:
 *   none
 */
void pumpWater(uint8_t halfCupOfWater){
    
    for(uint8_t i = 0; i < halfCupOfWater; i++){
    //if(getWaterLevel()==false){}
    digitalWrite(WATER_PUMP_SIGNAL_PIN, HIGH);                      
    delay(5000);                             
    digitalWrite(WATER_PUMP_SIGNAL_PIN, LOW);
    delay(10000);
    }
  }


/*
 * Description: returns temperature and value from DHT sensor
 *
 * Instructions for function:
 * 
 *     float *temp_var*;   
 *     float *humidity_var*;
 *     if(measure_environment( &*temp_var*, &*humidity_var*) == true)
 *        {...}
 *     var temperature and humidity will have measurement values stored in them
 *     temp in deg Celsius, humidity in %
 *
 * Arguments:
 *
 *
 * Return:
 * float temperature, float humidity
 */
 bool measureEnvironment( float *temperature, float *humidity = NULL )
{
  static unsigned long measurement_timestamp = millis( );
  bool canExit = false;
  
  /* Measure once every one second. */
  while( millis( ) - measurement_timestamp < 1000 )
  {}

  //only return a value if the measure function has returned true
  while(!canExit) 
  {
    if(dht_sensor.measure( temperature, humidity ) == true)
    {
      measurement_timestamp = millis( );
      canExit = true;
    }
    else
    {
      //Serial.println("FAIL");
    }
  }
 // return( false );
}



//This section prints output to LCD
void printScreen() {

  String testVar = "Test!";

  //float temperatureScreenC = 0;
  //float temperatureScreenF = 0;

  //measureEnvironment(&temperatureScreenC);
  //temperatureScreenF = ((temperatureScreenC * 1.8) + 32);


  //This prints the header of the screen like "Sensor Status" at the top.
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(screens[currentScreen][0]);



   //Serial.print("IN: ");
   //Serial.println(temperatureGlobalF);
   

  //This is for the default and passive screens, only display, no input.
  //This displays the 3 passive values below the header.
  if (currentScreen == 0){
  lcd.setCursor(0,1);
  lcd.print(screens[currentScreen][1]);
  lcd.print(" ");
  lcd.print(temperatureGlobalF);
  lcd.setCursor(0,2);
  lcd.print(screens[currentScreen][2]);
  lcd.print(" ");
  lcd.print(readSoil());
  lcd.setCursor(0,3);
  lcd.print(screens[currentScreen][3]);
  lcd.print(" ");
  lcd.print(lightAvg);
  }


/*
  //This is for the default and passive screens, only display, no input.
  //This displays screen 2 passive values below the header.
  if (currentScreen == 1){
 
  }

    //This is for the default and passive screens, only display, no input.
  //This displays screen 2 passive values below the header.
  if (currentScreen == 2){

  }
  */
  
  //Allow the up and down button to input values for "parameters" array. In units of 5 deg F for min and max temp screens.
  else if (currentScreen == 1 || currentScreen == 2){
  lcd.setCursor(0,1);
  lcd.print(parameters[currentScreen] * 5);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
  }


  //Allow the up and down button to input values for "parameters" array. In increments of LOW,MED,HIGH for water and light profiles.
  else if (currentScreen == 3 || currentScreen == 4){
  lcd.setCursor(0,1);
  if (parameters[currentScreen] == 0)
    {lcd.print("LOW");
    }
  else if (parameters[currentScreen] == 1)
    {lcd.print("MEDIUM");
    }
  else if (parameters[currentScreen] == 2)
    {lcd.print("HIGH");
    }
  else if (parameters[currentScreen] < 0)
    {lcd.print(parameters[currentScreen]);
    }
  else if (parameters[currentScreen] >2)
    {lcd.print(parameters[currentScreen]);
    }
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
  }

  
  //Allow the up and down button to input values for "parameters" array. Just increments of one half-cups screen and any additional screens.
  else if (currentScreen == 5){
  lcd.setCursor(0,1);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
  }

/*
  else if (currentScreen > 7){
  lcd.setCursor(0,1);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
  }
*/
  
}

//This is for the button pressing and debounce. Measures time and
//makes sure it is only using 1 button press to send
void setInputFlags() {
  for(uint8_t i = 0; i < NUM_OF_INPUTS; i++) {
    uint8_t reading = digitalRead(INPUT_PINS[i]);
    if (reading != lastInputState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != inputState[i]) {
        inputState[i] = reading;
        if (inputState[i] == HIGH) {
          inputFlags[i] = HIGH;
        }
      }
    }
    lastInputState[i] = reading;
  }
}

//This section increments user input section when you push up or down arrow
void parameterChange(uint8_t key) {
  if(key == 0) {
    parameters[currentScreen]++;
    paramsDataUpdated = true;
  }else if(key == 1) {
    parameters[currentScreen]--;
    paramsDataUpdated = true;
  }
}

//This section keeps track of number of screens, increments them, and will increment
//a counter for up and down arrow as well. Disabled for passive screens of course.
void inputAction(uint8_t input) {
  if(input == 0) {
    if (currentScreen == 0) {
      currentScreen = NUM_OF_SCREENS-1;
    }else{
      currentScreen--;
    }
    
  }else if(input == 1) {
    if (currentScreen == NUM_OF_SCREENS-1) {
      currentScreen = 0;
    }else{
      currentScreen++;
    }
  }
  else if(input == 2) {

    //If you dont want screen to have input at all
    if (currentScreen != 0 || currentScreen != 1 || currentScreen != 2) {
    parameterChange(0);
    }
  }

  else if(input == 3) {
    if (currentScreen != 0 || currentScreen != 1 || currentScreen != 2) {
    parameterChange(1);
   }
    else {}
  }
}

//Resolve kicks off the printScreen function after it senses a button press
void resolveInputFlags() {
  for(int i = 0; i < NUM_OF_INPUTS; i++) {
    if(inputFlags[i] == HIGH) {
      inputAction(i);
      inputFlags[i] = LOW;
      printScreen();
    }
  }
}

/*
 * Description: Take user setting data and store it in the
 *  Uno's EEPROM
 *  
 * Arguments:
 *  (INPUT) dataType -- byte that corasponds to the data you want store
 *      0 - temperature min
 *      1 - temerature max
 *      2 - soil moisture category
 *      3 - light 12 hr avg category
 *      4 - amount of half cups per watering
 *  (INPUT) data -- data you wish to store
 *  
 * Returns:
 *  none
 */
 void storeUsrSettings(uint8_t dataType, uint8_t data)
 {
  switch(dataType)
  {
    case 0:
      EEPROM.put(UNO_EEPROM.TEMP_MIN_ADDR, data);
      break;
    case 1:
      EEPROM.put(UNO_EEPROM.TEMP_MAX_ADDR, data);
      break;
    case 2:
      EEPROM.put(UNO_EEPROM.SOIL_CAT_ADDR, data);
      break;
    case 3:
      EEPROM.put(UNO_EEPROM.LIGHT_CAT_ADDR, data);
      break;
    case 4:
      EEPROM.put(UNO_EEPROM.WATER_AMOUNT_ADDR, data);
      break;
  }
 }





/*
 * Description: Retrieve the specified value from EEPROM
 *  
 * Arguments:
 *  (INPUT) data2Retrieve -- byte that corasponds to the data you want retrieve
 *      1 - temerature min and max
 *      2 - soil moisture category
 *      3 - light 12 hr avg category
 *      4 - amount of half cups per watering   
 *  (OUTPUT) usrTempMin -- pointer to variable that will contain the user set min temp
 *  (OUTPUT) usrTempMax -- pointer to variable that will contain the user set max temp
 *  
 * Return:
 *  uint8_t -- byte corresponding to a setting category
 *    0 - no category to return
 *    1 - minimum category
 *    2 - medium category
 *    3 - maximum category
 */
  uint8_t getUsrSetting(uint8_t data2Retrieve, uint8_t* usrTempMin = NULL, uint8_t* usrTempMax = NULL)
  {
    uint8_t data = 0;
    
    switch(data2Retrieve)
    {
      case 1:
        EEPROM.get(UNO_EEPROM.TEMP_MIN_ADDR, *usrTempMin);
        EEPROM.get(UNO_EEPROM.TEMP_MAX_ADDR, *usrTempMax);
        break;
      case 2:
        EEPROM.get(UNO_EEPROM.SOIL_CAT_ADDR, data);
        return data;
        break;
      case 3:
        EEPROM.get(UNO_EEPROM.LIGHT_CAT_ADDR, data);
        return data;
        break;
      case 4:
        EEPROM.get(UNO_EEPROM.WATER_AMOUNT_ADDR, data);
        return data;
        break;
    }
    return 0;
 }



 /*
  * Description: Responsible for sending an alert to the LCD screen
  *   and triggering the Phonton board to send a text
  *   
  * Arguments:
  *  (INPUT) sendText -- wether or not to send an alert text
  *  (INPUT) alertType -- designate what type of alert to display
  *     0 - error
  *     1 - water tank low
  *     2 - temperature too low
  *     3 - temerature too high
  *     4 - average light for 12hrs out of bounds
  *     5 - dirt too wet
  *     6 - dirt too dry
  *   
  * Returns:
  *   none
  */
  void sendAlert(bool sendText, uint8_t alertType)
  {
    float temperatureC = 0;
    float temperatureF = 0;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Code Processing");
    lcd.setCursor(0,1);
    
    if(sendText)
    {
      //Wakeup the photon
      digitalWrite(PHOTON_WK_PIN, HIGH);
      delay(200);
      digitalWrite(PHOTON_WK_PIN, LOW);

      delay(20000);//Give the photon time to wake up

      //Send alert type to Photon
      Wire.beginTransmission(PHOTON_I2C_ADDR);
      Wire.write(alertType);
      Wire.endTransmission();
    }

    //Print message to screen
    switch(alertType)
    {
      case 0:
        Serial.println("Alert! Case: Error");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Error! Code failed!");
        lcd.setCursor(0,1);
        break;
       case 1:
        Serial.println("Alert! Case: Tank Low");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("      Alert!      ");
        lcd.setCursor(0,1);
        lcd.print("Water Tank Low!");
        lcd.setCursor(0,2);
        lcd.print("Please refill");
        break;  
       case 2:
        Serial.println("Alert! Case: Temp Too Low");
        measureEnvironment(&temperatureC);
        temperatureF = ((temperatureC * 1.8) + 32);
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("      Alert!      ");
        lcd.setCursor(0,1);
        lcd.print("Temp is too low!");
        lcd.setCursor(0,2);
        lcd.print("temp is:   F");
        lcd.setCursor(9,2);
        lcd.print((uint8_t)temperatureF);
        break;
       case 3:
        Serial.println("Alert! Case: Temp Too High");
        measureEnvironment(&temperatureC);
        temperatureF = ((temperatureC * 1.8) + 32);
        
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("      Alert!      ");
        lcd.setCursor(0,1);
        lcd.print("Temp is too high!");
        lcd.setCursor(0,2);
        lcd.print("temp is:   F");
        lcd.setCursor(9,2);
        lcd.print((uint8_t)temperatureF);
        break; 
       case 4:
        Serial.println("Alert! Case: Avg Light Bounds Error");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("      Alert!      ");
        lcd.setCursor(0,1);
        lcd.print("Avg light for 12hrs");
        lcd.setCursor(0,2);
        lcd.print("is out of bounds!");
        lcd.setCursor(0,3);
        lcd.print("Set bounds is:");

        switch(getUsrSetting(3))
        {
          case 0:
            lcd.setCursor(15,3);
            lcd.print("low");
            break;
          case 1:
            lcd.setCursor(15,3);
            lcd.print("med");
            break;
          case 2:
            lcd.setCursor(15,3);
            lcd.print("high");
            break;
        }
        break;   
       case 5:
        Serial.println("Alert! Case: Soil Too Wet");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("      Alert!      ");
        lcd.setCursor(0,1);
        lcd.print("Soil is too wet!");
        lcd.setCursor(0,2);
        lcd.print(" ");
        lcd.setCursor(0,3);
        lcd.print("Set bounds is:");

        switch(getUsrSetting(2))
        {
          case 0:
            lcd.setCursor(15,3);
            lcd.print("low");
            break;
          case 1:
            lcd.setCursor(15,3);
            lcd.print("med");
            break;
          case 2:
            lcd.setCursor(15,3);
            lcd.print("high");
            break;
        }
        break;
       case 6:
        Serial.println("Alert! Case: Soil Too Dry");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("      Alert!      ");
        lcd.setCursor(0,1);
        lcd.print("Soil is too dry!");
        lcd.setCursor(0,2);
        lcd.print(" ");
        lcd.setCursor(0,3);
        lcd.print("Set bounds is:");

        switch(getUsrSetting(2))
        {
          case 0:
            lcd.setCursor(15,3);
            lcd.print("low");
            break;
          case 1:
            lcd.setCursor(15,3);
            lcd.print("med");
            break;
          case 2:
            lcd.setCursor(15,3);
            lcd.print("high");
            break;
        }
        break;                  
    }
    
    delay(10000);
    lcd.clear();
    printScreen();
  }
