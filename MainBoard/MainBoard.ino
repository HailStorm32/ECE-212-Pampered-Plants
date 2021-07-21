//---------------Begin Includes------------------------------------//
#include <dht_nonblocking.h>
#include "LiquidCrystal_I2C.h"

//---------------Begin Global Variables----------------------------//




//--Moisture Sensor--//
const uint8_t SOIL_PIN = A2; // set signal pin to analog (any analog pin works)
const uint8_t SOIL_PWR_PIN = 3; // sensor powered through digital pin to reduce corrosion (any digital pin works)
const uint8_t LDR_1_PIN = A0;
const uint8_t LDR_2_PIN = A1;
const uint8_t DHT_SENSOR_PIN = 2;

String testVar = "Test!";

#define DHT_SENSOR_TYPE DHT_TYPE_11
DHT_nonblocking dht_sensor( DHT_SENSOR_PIN, DHT_SENSOR_TYPE );
#include "functions.h"//This is included here seperatly so that the gobals defined above can be used in the functions.h

//--Moisture End--//



//--LCD--//

LiquidCrystal_I2C lcd(0x27, 20, 4);

//Dummy variable I needed to declare for Alert Function at bottom.
//Please change this when you get a chance.
int alertCounter = 1;


//Input & Button Logic
const int numOfInputs = 4;
const int inputPins[numOfInputs] = {5,6,7,8};
int inputState[numOfInputs];
int lastInputState[numOfInputs] = {LOW,LOW,LOW,LOW};
bool inputFlags[numOfInputs] = {LOW,LOW,LOW,LOW};
long lastDebounceTime[numOfInputs] = {0,0,0,0};
long debounceDelay = 5;


//Determine your screen count and creating arrays for screen and what is to be displayed
const int numOfScreens = 10;
int currentScreen = 0;
String screens[numOfScreens][5] = {{"Sensor Status 1/3","Temp:","Moisture:","Water level:"},{"Sensor Status 2/3", "Humidity:","Light:","Water resv lvl:"},{"Sensor Status 3/3","passive disp1:","passive disp2:","passive disp3:"},{"Min Temperature","deg in F"},{"Max Temperature", "Deg F"},
{"Water Profile","Selected"},{"Light Profile","Selected"},{"Accel Time", "Secs"},{"Restart Time","Mins"},{"Analog Out. Curr.","mA"}};
int parameters[numOfScreens];


//Screens array and parameters array are NOT linked together.
//screens is a variableName[10][2] array, it is a 2D array that has a table of
// 10 squares and within the squares is 2 values stored.
//Parameters is a different array. It is 1D. Has 10 squares with 1 value each.

//--LCD End--//




void setup() 
{
  Serial.begin(9600);

//--Moisture Sensor--//
  pinMode(SOIL_PWR_PIN, OUTPUT); 
  digitalWrite(SOIL_PWR_PIN, LOW); // set default to no power, reduces corrosion on moisture sensor
  
//--Moisture End--//



//--LCD--//

    //This for loop pulls pin resistor values to HIGH, LOW is when pressed.
  for(int i = 0; i < numOfInputs; i++) {
    pinMode(inputPins[i], INPUT);
    digitalWrite(inputPins[i], HIGH); // pull-up 20k
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


//**insert alert code here**//

  //--LCD--//
  setInputFlags();
  resolveInputFlags();
}



//This is for the button pressing and debounce. Measures time and
//makes sure it is only using 1 button press to send
void setInputFlags() {
  for(int i = 0; i < numOfInputs; i++) {
    int reading = digitalRead(inputPins[i]);
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


//Resolve kicks off the printScreen function after it senses a button press
void resolveInputFlags() {
  for(int i = 0; i < numOfInputs; i++) {
    if(inputFlags[i] == HIGH) {
      inputAction(i);
      inputFlags[i] = LOW;
      printScreen();
    }
  }
}


//This section keeps track of number of screens, increments them, and will increment
//a counter for up and down arrow as well. Disabled for passive screens of course.
void inputAction(int input) {
  if(input == 0) {
    if (currentScreen == 0) {
      currentScreen = numOfScreens-1;
    }else{
      currentScreen--;
    }
    
  }else if(input == 1) {
    if (currentScreen == numOfScreens-1) {
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


//This section increments user input section when you push up or down arrow
void parameterChange(int key) {
  if(key == 0) {
    parameters[currentScreen]++;
  }else if(key == 1) {
    parameters[currentScreen]--;
  }
}


//This section prints output to LCD
void printScreen() {

  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print(screens[currentScreen][0]);


  //This is for the default and passive screens, only display, no input.
  if (currentScreen == 0 || currentScreen == 1 || currentScreen == 2){
  lcd.setCursor(0,1);
  lcd.print(screens[currentScreen][1]);
  lcd.print(" ");
  lcd.print(testVar);
  lcd.setCursor(0,2);
  lcd.print(screens[currentScreen][2]);
  lcd.print(" ");
  lcd.print(testVar);
  lcd.setCursor(0,3);
  lcd.print(screens[currentScreen][3]);
  lcd.print(" ");
  lcd.print(testVar);

  }

  //Allow the up and down button to input values for "parameters" array.
  else if (currentScreen > 2){
  lcd.setCursor(0,1);
  lcd.print(parameters[currentScreen]);
  lcd.print(" ");
  lcd.print(screens[currentScreen][1]);
  }


  

  
  
}


//This section is for Alerts. It will print seperately of button logic.
void printAlert() {

  if (alertCounter = 1){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alert! check device!");
  lcd.setCursor(0,1);
  lcd.print("Temp too high!");
  lcd.setCursor(0,2);
  lcd.print("temp is: ");
  lcd.setCursor(11,2);
  lcd.print(testVar);
  }
  
  else if (alertCounter = 2){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Alert! check device!");
  lcd.setCursor(0,1);
  lcd.print("Temp too low!");
  lcd.setCursor(0,2);
  lcd.print("temp is: ");
  lcd.setCursor(11,2);
  lcd.print(testVar);
  }

  delay(10000);
  lcd.clear();
  printScreen();

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



//--LCD End--//

  
