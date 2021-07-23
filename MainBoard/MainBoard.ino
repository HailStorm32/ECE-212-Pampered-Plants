//---------------Begin Includes------------------------------------//
#include <dht_nonblocking.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"


//---------------Begin Global Variables----------------------------//
const uint8_t SOIL_PIN = A2; // set signal pin to analog (any analog pin works)
const uint8_t SOIL_PWR_PIN = 3; // sensor powered through digital pin to reduce corrosion (any digital pin works)
const uint8_t LDR_1_PIN = A0;
const uint8_t LDR_2_PIN = A1;
const uint8_t DHT_SENSOR_PIN = 2;
const uint8_t PHOTON_WK_PIN = 9;
const uint32_t MONITOR_INTERVAL = 300000; // 5min -- monitor interval in milliseconds
const uint8_t MAX_NUM_LIGHT_MEASUREMENTS = 143; //Should work out to be an average over 12hrs
const uint8_t PHOTON_I2C_ADDR = 8;
const uint8_t WATER_PUMP_SIGNAL_PIN = 1;

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
  const uint16_t MIN_LOWER = 1; //1-85
  const uint16_t MIN_UPPER = 85; 
  const uint16_t MED_LOWER = 86; //86-170
  const uint16_t MED_UPPER = 170; 
  const uint16_t MAX_LOWER = 171; //171-255
  const uint16_t MAX_UPPER = 255; 
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



//------------Begin LCD Variables-----------------//

//Input & Button Logic
const uint8_t NUM_OF_INPUTS = 4;
const uint8_t INPUT_PINS[NUM_OF_INPUTS] = {5,6,7,8};
int inputState[NUM_OF_INPUTS];
bool lastInputState[NUM_OF_INPUTS] = {LOW,LOW,LOW,LOW};
bool inputFlags[NUM_OF_INPUTS] = {LOW,LOW,LOW,LOW};
long lastDebounceTime[NUM_OF_INPUTS] = {0,0,0,0};
long debounceDelay = 5;


//Determine your screen count and creating arrays for screen and what is to be displayed
const uint8_t NUM_OF_SCREENS = 10;
uint8_t currentScreen = 0;
String screens[NUM_OF_SCREENS][5] = {{"Sensor Status 1/3","Temp:","Moisture:","Water level:"},{"Sensor Status 2/3", "Humidity:","Light:","Water resv lvl:"},{"Sensor Status 3/3","passive disp1:","passive disp2:","passive disp3:"},{"Min Temperature","deg in F"},{"Max Temperature", "Deg F"},
{"Water Profile","Selected"},{"Light Profile","Selected"},{"Accel Time", "Secs"},{"Restart Time","Mins"},{"Analog Out. Curr.","mA"}};
int parameters[NUM_OF_SCREENS];


//Screens array and parameters array are NOT linked together.
//screens is a variableName[10][2] array, it is a 2D array that has a table of
// 10 squares and within the squares is 2 values stored.
//Parameters is a different array. It is 1D. Has 10 squares with 1 value each.
String testVar = "Test!";

//------------End LCD Variables-----------------//

#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
LiquidCrystal_I2C lcd(0x27, 20, 4);

#include "functions.h"//This is included here seperatly so that the gobals defined above can be used in the functions.h





void setup() 
{
  Serial.begin(9600);

  Wire.begin();

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




//This is the loop. This is the heart that let's everything in motion.
//It is always looping. We love loop.
void loop() {
  //Begin sensor monitoring code

  //Only measure sensors at set intervals
  if((millis() - lastMeasurement) >= MONITOR_INTERVAL)
  {
    uint8_t usrTempMin = 12;
    uint8_t usrTempMax = 26;
    //uint8_t usrRange = 0;
    
    //Get sensor readings
    uint16_t moistureReading = readSoil();
    uint8_t lightReading = getLightReading();
    uint8_t temperatureReading = 0;//Holder until temerature function is written
    bool tankLevel = 1; //Holder until water level function is written

   
    //Check water tank level
    if(tankLevel == 0)
    {
      //Send alert
    }
    
    //Check moisture sensor
    switch(getUsrSetting())
    {
      case 1:
        if(moistureReading < SOIL_PARAM.MIN_LOWER)
        {
          //Water plant
        }
        else if(moistureReading > SOIL_PARAM.MIN_UPPER)
        {
          //Send alert
        }
        break;
      case 2:
        if(moistureReading < SOIL_PARAM.MED_LOWER)
        {
          //Water plant
        }
        else if(moistureReading > SOIL_PARAM.MED_UPPER)
        {
          //Send alert
        }
        break;
      case 3:
        if(moistureReading < SOIL_PARAM.MAX_LOWER)
        {
          //Water plant
        }
        else if(moistureReading > SOIL_PARAM.MAX_UPPER)
        {
          //Send alert
        }
        break;
    }

    
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

      switch(getUsrSetting())
      {
        case 1:
          if(lightAvg < LIGHT_PARAM.MIN_LOWER || lightAvg > LIGHT_PARAM.MIN_UPPER)
          {
            //Send alert
          }
          break;
        case 2:
          if(lightAvg < LIGHT_PARAM.MED_LOWER || lightAvg > LIGHT_PARAM.MED_UPPER)
          {
            //Send alert
          }
          break;
        case 3:
          if(lightAvg < LIGHT_PARAM.MAX_LOWER || lightAvg > LIGHT_PARAM.MAX_UPPER)
          {
            //Send alert
          }
          break;
      }

      //Reset our tracking variables
      numOfLightMeasurements = 0;
      sumOfLightReadings = 0;
    }

    //Check temperature
    getUsrSetting(&usrTempMin, &usrTempMax);

    if(temperatureReading < usrTempMin || temperatureReading > usrTempMax)
    {
      //Send alert
    }
    
    
    lastMeasurement = millis();
  }
  //End sensor monitoring code

  //--LCD--//
  setInputFlags();
  resolveInputFlags();
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
