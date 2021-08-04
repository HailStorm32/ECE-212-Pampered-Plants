#include "Wire.h"

const uint8_t I2C_ADDR =  8;
int receivedByte =  10;
long timeOfAwake = 0;
bool success = false;
bool gotData = false;

void setup() {
  Wire.begin(I2C_ADDR);
  Serial.begin(9600);

  pinMode(7, OUTPUT);




  timeOfAwake = millis();
  Wire.onReceive(receiveEvents);
}


void loop() {

  while(!Particle.connected())
  {
    Serial.println("waiting for connected");
  }
  Serial.println(" r");

  if(gotData)
  {
    switch (receivedByte) {
      case 0:
        success = Particle.publish("alert-status", "0");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
      case 1:
        success = Particle.publish("alert-status", "1");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
      case 2:
        success = Particle.publish("alert-status", "2");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
      case 3:
        success = Particle.publish("alert-status", "3");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
      case 4:
        success = Particle.publish("alert-status", "4");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
      case 5:
        success = Particle.publish("alert-status", "5");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
      case 6:
        success = Particle.publish("alert-status", "6");
        if (!success)
        {
          for(uint8_t indx = 0; indx < 5; indx++)
          {
            digitalWrite(7, HIGH);
            delay(1000);
            digitalWrite(7, LOW);
            delay(500);
          }
        }
    }
    gotData = false;
  }//


if((millis() - timeOfAwake) >= 45000)
  {
    SystemSleepConfiguration config;
    config.mode(SystemSleepMode::HIBERNATE).gpio(WKP, RISING);
    System.sleep(config);
  }
}

void receiveEvents(int numBytes)
{

  receivedByte = Wire.read();
  Serial.print(receivedByte);
  timeOfAwake = millis();
  gotData = true;
}
