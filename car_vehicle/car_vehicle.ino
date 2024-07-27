#include "arduino_secrets.h"

// Servo - Version: 1.1.7
#include <Servo.h>

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

long WheelSpd;
bool WheelDir;




Servo turningServo;  // create servo object to control a servo

int servoPos = 0;    // variable to store the servo position

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
    
    turningServo.attach(9);  // attaches the servo on pin 9 to the servo object
}

void loop()
{
    // Set buffer to size of expected message
    uint8_t buf[2];
    uint8_t buflen = sizeof(buf);
    // Check if received packet is correct size
    if (rf_driver.recv(buf, &buflen))
    {
      // Get serial input, parse into two floats (in range 0, 255)
      xInput = buf[0];
      yInput = buf[1];
      // Serial.println("In X:Y = " + (String)xInput + " : " + (String)yInput);
      
      // Set wheel directions
      signed int y = map(yInput, 0, 255, -255, 255);
      // Get wheel speed and zero out if low
      WheelSpd = abs(y);
      if (abs(WheelSpd) <= 15) {WheelSpd = 0;}
      WheelDir = ( y > 0 ) ? true : false;
      
      servoPos = map(xInput, 0, 255, 45, 180);  // 0,180 for max server angles
      
      Serial.println("SPD: " + (String)(WheelSpd) + ", FWD: " + (String)(WheelDir) + ", DIR: " + (String)(servoPos));
      
      turningServo.write(servoPos); 
      
      // Write motor PWMs
      analogWrite(motorOnePinEnable, WheelSpd);
      digitalWrite(motorOnePinOne, !WheelDir);  // Inverted because motors are opposite direction...
      digitalWrite(motorOnePinTwo, WheelDir);
      
      analogWrite(motorTwoPinEnable, WheelSpd);
      digitalWrite(motorTwoPinOne,!WheelDir);
      digitalWrite(motorTwoPinTwo, WheelDir);
    }
    delay(50);
}