#include "arduino_secrets.h"

/*
  433 MHz RF Module Receiver Demonstration 1
  RF-Rcv-Demo-1.ino
  Demonstrates 433 MHz RF Receiver Module
  Use with Transmitter Demonstration 1

  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/

// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

int motorOnePinOne = 22;
int motorOnePinTwo = 24;
int motorOnePinEnable = 2;  // PWM pin

int motorTwoPinOne = 26;
int motorTwoPinTwo = 28;
int motorTwoPinEnable = 3;  // PWM pin

byte xInput;
byte yInput;

long rightWheelSpd;
long leftWheelSpd;

bool rightWheelDir;
bool leftWheelDir;


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(motorOnePinOne, OUTPUT);
    pinMode(motorOnePinTwo, OUTPUT);
    pinMode(motorOnePinEnable, OUTPUT);
    pinMode(motorTwoPinOne, OUTPUT);
    pinMode(motorTwoPinTwo, OUTPUT);
    pinMode(motorTwoPinEnable, OUTPUT);
    // Initialize ASK Object
    rf_driver.init();
    // Setup Serial Monitor
    Serial.begin(9600);
}

void loop()
{
    // Set buffer to size of expected message
    uint8_t buf[2];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {
      // Get serial input, parse into two floats
      xInput = buf[0];
      yInput = buf[1];
      // Serial.println("In X:Y = " + (String)xInput + " : " + (String)yInput);
      
      // Transform joystick values into motor signals
      // Put into the range -255 to 255 for easier calcs/understanding
      signed int x = 2*(xInput - 127.5);
      signed int y = 2*(127.5 - yInput);  // Invert y so large number = forward
      // Serial.println("X:Y = " + (String)x + " : " + (String)y);
      x = x/2; // Make turning less touchy
      // add numbers together into "speed" of each motor
      rightWheelSpd = y - x;
      leftWheelSpd = y + x;
      // Serial.println("SPD1 X:Y = " + (String)leftWheelSpd + " : " + (String)rightWheelSpd);
      // Bound these in the +255 to -255 range
      int maxSpd = max(abs(rightWheelSpd), abs(leftWheelSpd));  // abs means this owrks for forwards and backwards
      if (maxSpd > 255)  {
        rightWheelSpd = (rightWheelSpd*255)/maxSpd;
        leftWheelSpd = (leftWheelSpd*255)/maxSpd;
      }
      // Zero out if near zero
      if (abs(rightWheelSpd) <= 15) {rightWheelSpd = 0;}
      if (abs(leftWheelSpd) <= 15) {leftWheelSpd = 0;}
      // Calc direction
      rightWheelDir = (rightWheelSpd > 0) ? true : false;
      leftWheelDir = (leftWheelSpd > 0) ? true : false;

      // Serial.println("Fin Spd LHS:RHS = " + (String)leftWheelSpd + " : " + (String)rightWheelSpd);
      // Serial.println("Fin Dir LHS:RHS = " + (String)leftWheelDir + " : " + (String)rightWheelDir);
      
      Serial.println("RHS: " + (String)(!rightWheelDir) + " : " + (String)(rightWheelDir));
      Serial.println("LHS: " + (String)(leftWheelDir) + " : " + (String)(!leftWheelDir));
      // Write motor PWMs
      analogWrite(motorOnePinEnable, abs(rightWheelSpd));
      digitalWrite(motorOnePinOne, !rightWheelDir);  // Inverted because motors are opposite direction...
      digitalWrite(motorOnePinTwo, rightWheelDir);
      
      analogWrite(motorTwoPinEnable, abs(leftWheelSpd));
      digitalWrite(motorTwoPinOne, leftWheelDir);
      digitalWrite(motorTwoPinTwo, !leftWheelDir);
    }
    delay(50);
}