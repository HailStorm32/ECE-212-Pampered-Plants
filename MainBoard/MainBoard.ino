//---------------Begin Includes------------------------------------//
#include <dht_nonblocking.h>


//---------------Begin Global Variables-----------------------------//

//Moisture sensor below
const uint8_t SOIL_PIN = A2; // set signal pin to analog (any analog pin works)
const uint8_t SOIL_PWR_PIN = 3; // sensor powered through digital pin to reduce corrosion (any digital pin works)
const uint8_t LDR_1_PIN = A0;
const uint8_t LDR_2_PIN = A1;
const uint8_t DHT_SENSOR_PIN = 2;



#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
#include "functions.h"//This is included here seperatly so that the gobals defined above can be used in the functions.h




//LCD code below
#include "LiquidCrystal_I2C.h"
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
