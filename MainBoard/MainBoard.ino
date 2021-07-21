//---------------Begin Includes------------------------------------//
#include <dht_nonblocking.h>
#include "LiquidCrystal_I2C.h"

//---------------Begin Global Variables-----------------------------//

const uint8_t SOIL_PIN = A2; // set signal pin to analog (any analog pin works)
const uint8_t SOIL_PWR_PIN = 3; // sensor powered through digital pin to reduce corrosion (any digital pin works)
const uint8_t LDR_1_PIN = A0;
const uint8_t LDR_2_PIN = A1;
const uint8_t DHT_SENSOR_PIN = 2;
const uint32_t MONITOR_INTERVAL = 300000; // 5min -- monitor interval in milliseconds
const uint8_t MAX_NUM_LIGHT_MEASUREMENTS = 143; //Should work out to be an average over 12hrs

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



#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );


#include "functions.h"//This is included here seperatly so that the gobals defined above can be used in the functions.h




//LCD code below
LiquidCrystal_I2C lcd(0x27, 16, 4);

const int buttonPinLeft = 5;
const int buttonPinDown = 6;
const int buttonPinUp = 7;
const int buttonPinRight = 8;

int buttonStateLeft = 0; 
int buttonStateDown = 0;
int buttonStateUp = 0;
int buttonStateRight = 0;


const int numOfScreens = 4;
int currentScreen = 0;
String screens[numOfScreens][2] = {{"Value_1","Unit_1"}, {"Value_2", "Unit_2"}, {"Value_3","Unit_3"}, {"Value_4","Unit_4"}};
int parameters[numOfScreens] = {0, 10, 50, 100};



void setup() 
{
  Serial.begin(9600);

  //Next 2 lines is moisture sensor
  pinMode(SOIL_PWR_PIN, OUTPUT); 
  digitalWrite(SOIL_PWR_PIN, LOW); // set default to no power, reduces corrosion on moisture sensor

  //This is LCD code below
    lcd.init();   // initializing the LCD
  lcd.backlight(); // Enable or Turn On the backlight

  pinMode(buttonPinLeft, INPUT_PULLUP);
  pinMode(buttonPinDown, INPUT_PULLUP);
  pinMode(buttonPinUp, INPUT_PULLUP);
  pinMode(buttonPinRight, INPUT_PULLUP);

  lcd.clear();
  lcd.setCursor(4,1);
  lcd.print("THIS IS THE");
  lcd.setCursor(3,2);
  lcd.print("MENU TUTORIAL");
  delay(5000);
  lcd.clear();
}



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
      
    }
    
    
    
    lastMeasurement = millis();
  }
  //End sensor monitoring code
 
  //This is LCD code below
  buttonStateLeft = digitalRead(buttonPinLeft);
  buttonStateDown = digitalRead(buttonPinDown);
  buttonStateUp = digitalRead(buttonPinUp);
  buttonStateRight = digitalRead(buttonPinRight);
  
  if(buttonStateLeft == LOW){
    lcd.clear();
    if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
    } else {
      currentScreen--;
    }
}
  
  else if (buttonStateDown == LOW){
    lcd.clear();
    parameters[currentScreen]--;
}
    
  else if (buttonStateUp == LOW){
    lcd.clear();
    parameters[currentScreen]++;
}

  else if (buttonStateRight == LOW){
    lcd.clear();
    if (currentScreen == numOfScreens-1) {
      currentScreen = 0;
    } else {
      currentScreen++;
    }
}
  
  printScreen();
  delay(200);
}


void printScreen() {
  if (currentScreen == 3) {
    lcd.setCursor(0,0);
    lcd.print(screens[0][0]);
    lcd.print(": ");
    lcd.print(parameters[0]);
    lcd.print(" ");
    lcd.print(screens[0][1]);

    lcd.setCursor(0,1);
    lcd.print(screens[1][0]);
    lcd.print(": ");
    lcd.print(parameters[1]);
    lcd.print(" ");
    lcd.print(screens[1][1]);

    lcd.setCursor(0,2);
    lcd.print(screens[2][0]);
    lcd.print(": ");
    lcd.print(parameters[2]);
    lcd.print(" ");
    lcd.print(screens[2][1]);

    lcd.setCursor(0,3);
    lcd.print(screens[3][0]);
    lcd.print(": ");
    lcd.print(parameters[3]);
    lcd.print(" ");
    lcd.print(screens[3][1]);
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("MENU TOTORIAL");
    lcd.setCursor(0,2);
    lcd.print(screens[currentScreen][0]);
    lcd.setCursor(0,3);
    lcd.print(parameters[currentScreen]);
    lcd.print(" ");
    lcd.print(screens[currentScreen][1]);
  }
}
