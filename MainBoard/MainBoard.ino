//---------------Begin Includes------------------------------------//
#include <dht_nonblocking.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include <EEPROM.h>


//---------------Begin Global Variables----------------------------//
const uint8_t SOIL_PIN = A2; // set signal pin to analog (any analog pin works)
const uint8_t SOIL_PWR_PIN = 3; // sensor powered through digital pin to reduce corrosion (any digital pin works)
const uint8_t LDR_1_PIN = A0;
const uint8_t LDR_2_PIN = A1;
const uint8_t DHT_SENSOR_PIN = 2;
const uint8_t PHOTON_WK_PIN = 9;
const uint32_t MONITOR_INTERVAL = 60000; // 5min -- monitor interval in milliseconds
const uint8_t MAX_NUM_LIGHT_MEASUREMENTS = 143; //Should work out to be an average over 12hrs
const uint8_t PHOTON_I2C_ADDR = 8;
const uint8_t WATER_PUMP_SIGNAL_PIN = 1;
const uint8_t WATER_LEVEL = 4; 

struct eepromAddresses
{
  const uint16_t TEMP_MIN_ADDR = 0;
  const uint16_t TEMP_MAX_ADDR = 1;
  const uint16_t SOIL_CAT_ADDR = 2;
  const uint16_t LIGHT_CAT_ADDR = 3;
  const uint16_t WATER_AMOUNT_ADDR = 4;
}UNO_EEPROM;

struct soilSensorBounds
{
  const uint16_t MIN_LOWER = 1; //1-300
  const uint16_t MIN_UPPER = 300; 
  const uint16_t MED_LOWER = 301; //301-600
  const uint16_t MED_UPPER = 600; 
  const uint16_t MAX_LOWER = 601; //601-1000
  const uint16_t MAX_UPPER = 1000; 
}SOIL_PARAM;

struct lightSensorBounds
{
  const uint8_t MIN_LOWER = 1; //1-85
  const uint8_t MIN_UPPER = 85; 
  const uint8_t MED_LOWER = 86; //86-170
  const uint8_t MED_UPPER = 170; 
  const uint8_t MAX_LOWER = 171; //171-255
  const uint8_t MAX_UPPER = 255; 
}LIGHT_PARAM;

/*struct temperatureSensorBounds
{
  uint8_t MIN = 12; //default temp in C
  uint8_t MAX = 26; //default temp in C
}TEMPERATURE_PARAM;*/

unsigned long lastMeasurement = 0;
uint8_t numOfLightMeasurements = 0;
uint16_t sumOfLightReadings = 0;
uint8_t lightAvg = 0;
bool paramsDataUpdated = false;

float temperatureGlobalF = 0;




//------------Begin LCD Variables-----------------//

//Input & Button Logic
const uint8_t NUM_OF_INPUTS = 4;
const uint8_t INPUT_PINS[NUM_OF_INPUTS] = {5,6,7,8};
bool inputState[NUM_OF_INPUTS];
bool lastInputState[NUM_OF_INPUTS] = {LOW,LOW,LOW,LOW};
bool inputFlags[NUM_OF_INPUTS] = {LOW,LOW,LOW,LOW};
long lastDebounceTime[NUM_OF_INPUTS] = {0,0,0,0};
long debounceDelay = 5;


//Determine your screen count and creating arrays for screen and what is to be displayed
const uint8_t NUM_OF_SCREENS = 6;
uint8_t currentScreen = 0;
String screens[NUM_OF_SCREENS][5] = {{"Sensor Status","Temp:","Moisture:","Light:"},{"Min Temperature","deg in F"},{"Max Temperature", "Deg F"},
{"Soil Moist Profile","Selected"},{"Light Profile","Selected"},{"Add half-cups water", "half-cups"}};
int parameters[NUM_OF_SCREENS];




//Screens array and parameters array are NOT linked together.
//screens is a variableName[10][2] array, it is a 2D array that has a table of
// 10 squares and within the squares is 2 values stored.
//Parameters is a different array. It is 1D. Has 10 squares with 1 value each.


//------------End LCD Variables-----------------//

#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include "functions.h"//This is included here seperatly so that the gobals defined above can be used in the functions.h





void setup() 
{
  for(int indx = 0; indx < NUM_OF_SCREENS; indx++)
  {
    parameters[indx] = 0;
  }
  
  
  Serial.begin(9600);

  Wire.begin();

// --Water Level Sensor--//
  pinMode(WATER_LEVEL, INPUT_PULLUP);// Attaches the input of the float sensor to the Arduino internal 10k resistor

//--Moisture Sensor--//
  pinMode(SOIL_PWR_PIN, OUTPUT); 
  digitalWrite(SOIL_PWR_PIN, LOW); // set default to no power, reduces corrosion on moisture sensor
  
//--Moisture End--//

//Photon board related pin setup
  pinMode(PHOTON_WK_PIN, OUTPUT);
  digitalWrite(PHOTON_WK_PIN, LOW);
	
//--WATER PUMP--//
pinMode(WATER_PUMP_SIGNAL_PIN, OUTPUT);


//--LCD--//

  //This for loop pulls pin resistor values to HIGH, LOW is when pressed.
  for(int i = 0; i < NUM_OF_INPUTS; i++) {
    pinMode(INPUT_PINS[i], INPUT);
    digitalWrite(INPUT_PINS[i], HIGH); // pull-up 20k
  }

  //Initialize LCD itself
	lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight
  
  uint8_t initUsrTempMin = 0;
  uint8_t initUsrTempMax = 0;

  getUsrSetting(1, &initUsrTempMin, &initUsrTempMax);
  
  parameters[1] = initUsrTempMin/5; //store temerature min data
  parameters[2] = initUsrTempMax/5; //store temerature max data
  parameters[3] = getUsrSetting(2); //store soil moisture profile data
  parameters[4] = getUsrSetting(3); //store light profile data
  parameters[5] = getUsrSetting(4); //store # of half cups per watering data

  
	//Initial setup of default LCD screen bootup
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("Pampered Plants v1.0");
  lcd.setCursor(0,2);
  lcd.print("Read our manual");
  lcd.setCursor(0,3);
  lcd.print("And enjoy!");
  delay(2000);
  lcd.clear();
  
//--LCD End--//
  
}


float temperatureReadingTempC = 0;
//float temperatureT = 0;
float humidity = 0;

//This is the loop. This is the heart that let's everything in motion.
//It is always looping. We love loop.
void loop() {

   //Serial.print("Soil: ");
  // Serial.println(readSoil());

   //measureEnvironment(&temperatureReadingTempC);
   dht_sensor.measure( &temperatureReadingTempC, &humidity );
   temperatureGlobalF = ((temperatureReadingTempC * 1.8) + 32);

   //Serial.print("Temp: ");
   //Serial.println(temperatureGlobalF);

      //-------BEGIN DEBUG CODE ---------
   /* uint8_t debugUsrTempMin = 255;
    uint8_t debugUsrTempMax = 255;

    getUsrSetting(1, &debugUsrTempMin, &debugUsrTempMax);

    Serial.print("User Set Min Temp: ");
    Serial.println(debugUsrTempMin);
    Serial.print("User Set Max Temp: ");
    Serial.println(debugUsrTempMax);

    Serial.print("User Set Soil profile: ");
    Serial.println(getUsrSetting(2));

    Serial.print("User Set light profile: ");
    Serial.println(getUsrSetting(3));

    Serial.print("User Set halfcup profile: ");
    Serial.println(getUsrSetting(4));

    Serial.println(" ");
    Serial.println(" ");*/
    //-------END DEBUG CODE ---------

  
  //Begin storing user settings code
  if(paramsDataUpdated)
  {
    /*for(uint8_t indx = 0; indx < 5; indx++)
    {
      storeUsrSettings(indx, parameters[indx+1]);
    }*/
    storeUsrSettings(0, parameters[1]*5);//store temerature min data
    storeUsrSettings(1, parameters[2]*5);//store temerature min data
    storeUsrSettings(2, parameters[3]);//store soil moisture profile data
    storeUsrSettings(3, parameters[4]);//store light profile data
    storeUsrSettings(4, parameters[5]);//store # of half cups per watering data
    
    //1 -- temperature min
    //2 -- temperature max
    //3 -- soil moisture
    //4 -- light
    //5 -- half cups

    paramsDataUpdated = false;
  }


  
  //Begin sensor monitoring code

  //Only measure sensors at set intervals
  if((millis() - lastMeasurement) >= MONITOR_INTERVAL)
  {
    Serial.println("IN Monitor");
/*    //-------BEGIN DEBUG CODE ---------
    uint8_t debugUsrTempMin = 255;
    uint8_t debugUsrTempMax = 255;

    getUsrSetting(1, &debugUsrTempMin, &debugUsrTempMax);

    Serial.print("User Set Min Temp: ");
    Serial.println(debugUsrTempMin);
    Serial.print("User Set Max Temp: ");
    Serial.println(debugUsrTempMin);

    Serial.print("User Set Soil profile: ");
    Serial.println(getUsrSetting(2));

    Serial.print("User Set light profile: ");
    Serial.println(getUsrSetting(3));

    Serial.print("User Set halfcup profile: ");
    Serial.println(getUsrSetting(4));*/
    //-------END DEBUG CODE ---------

    
    uint8_t usrTempMin = 53;
    uint8_t usrTempMax = 85;
    
    //Get sensor readings
    uint16_t moistureReading = readSoil();
    uint8_t lightReading = getLightReading();
    float temperatureReadingC = 0;
    float temperatureReadingF = 0;
    bool tankLevel = 1; //Holder until water level function is written

    measureEnvironment(&temperatureReadingC);
    temperatureReadingF = ((temperatureReadingC * 1.8) + 32);
    
   Serial.println("Checking tank");
    //Check water tank level
    if(tankLevel == 0)
    {
      sendAlert(true, 1);
    }

    Serial.println("Checking moisture");
    //Check moisture sensor
    switch(getUsrSetting(2))
    {
      case 0:
        if(moistureReading < SOIL_PARAM.MIN_LOWER)
        {
          pumpWater(getUsrSetting(4));
        }
        else if(moistureReading > SOIL_PARAM.MIN_UPPER)
        {
          sendAlert(true, 5);
        }
        break;
      case 1:
        if(moistureReading < SOIL_PARAM.MED_LOWER)
        {
          pumpWater(getUsrSetting(4));
        }
        else if(moistureReading > SOIL_PARAM.MED_UPPER)
        {
          sendAlert(true, 5);
        }
        break;
      case 2:
        if(moistureReading < SOIL_PARAM.MAX_LOWER)
        {
          pumpWater(getUsrSetting(4));
        }
        else if(moistureReading > SOIL_PARAM.MAX_UPPER)
        {
          sendAlert(true, 5);
        }
        break;
    }

    Serial.println("Checking light");
    //Check light sensor
    
    //If we havn't gatherd all the readings, gather another reading
    if(numOfLightMeasurements < MAX_NUM_LIGHT_MEASUREMENTS)
    {
      sumOfLightReadings += getLightReading();
      numOfLightMeasurements++;
    }
    else
    {
      lightAvg = (sumOfLightReadings/MAX_NUM_LIGHT_MEASUREMENTS);

      switch(getUsrSetting(3))
      {
        case 0:
          if(lightAvg < LIGHT_PARAM.MIN_LOWER || lightAvg > LIGHT_PARAM.MIN_UPPER)
          {
            sendAlert(false, 4);
          }
          break;
        case 1:
          if(lightAvg < LIGHT_PARAM.MED_LOWER || lightAvg > LIGHT_PARAM.MED_UPPER)
          {
            sendAlert(false, 4);
          }
          break;
        case 2:
          if(lightAvg < LIGHT_PARAM.MAX_LOWER || lightAvg > LIGHT_PARAM.MAX_UPPER)
          {
            sendAlert(false, 4);
          }
          break;
      }

      //Reset our tracking variables
      numOfLightMeasurements = 0;
      sumOfLightReadings = 0;
    }

    Serial.println("Checking temp");
    //Check temperature
    getUsrSetting(1, &usrTempMin, &usrTempMax);

    if(temperatureReadingF < usrTempMin)
    {
      sendAlert(true, 2);
    }
    else if(temperatureReadingF > usrTempMax)
    {
      sendAlert(true, 3);
    }
    
    
    lastMeasurement = millis();
  }
  //End sensor monitoring code

  //--LCD--//
  //Run through LCD loop to display values on screen and gather user input
  setInputFlags(); 
  resolveInputFlags();

  //End LCD code

}


//To-do
//Make increments 5F for temp
//Come up with more advanced things to display, ex: average light last 12 hours, when last watered
//Integrade a timeout so screen 0 will be shown after an amount of time
//Convert profiles from a 0,1,2 into LOW,MED,HIGH for print output ONLY
//Integrate sensor function calls into main print output for LCD
//User Alerts needs real data and needs a trigger so we can test it
//Add second alerts page verifying communication (text sent)?
//RF transmit
//EEPROM storage


//Alert function will be called from monitoring code (sensor),
// In alert function it will write screen and wake up photon board and send text
// The after a delay, screen 1 will be redrawn.
// Alert functionality will not be in normal button screen logic.
