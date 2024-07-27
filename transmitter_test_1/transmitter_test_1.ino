#include "arduino_secrets.h"

/*
  433 MHz RF Module Transmitter Demonstration 1
  RF-Xmit-Demo-1.ino
  Demonstrates 433 MHz RF Transmitter Module
  Use with Receiver Demonstration 1
 
  DroneBot Workshop 2018
  https://dronebotworkshop.com
*/
 
// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library 
#include <SPI.h> 
 
// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

// No pin specified as RH defaults to pin12
// int LedPinInternal=13;

int potPinOne = A8;
int potPinTwo = A9;
int potValOne;
int potValTwo;
int pwmValOne;
int pwmValTwo;
int ledVal;
unsigned long timeOne, timeTwo;


void setup()
{
    pinMode(potPinOne, INPUT);
    pinMode(potPinTwo, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    // Initialize ASK Object
    rf_driver.init();
    
    Serial.begin(9600);
}
 
void loop()
{
    timeOne = millis();
    // Read in potentiometer vals
    potValOne = analogRead(potPinOne);
    potValTwo = analogRead(potPinTwo);
    // Map to byte range and cast
    pwmValOne = (byte) map(potValOne, 0, 1023, 0, 255);
    pwmValTwo = (byte) map(potValTwo, 0, 1023, 0, 255);
    // Serial.println("PWM 1:1  =  " + (String)pwmValOne + " : " + (String)pwmValTwo);
    
    // ledVal = map(potValOne, 0, 1023, 0, 255);
    byte pwmVals[2] = {pwmValOne, pwmValTwo};

    digitalWrite(LED_BUILTIN, HIGH);  // Light is on while transmitting
    rf_driver.send((uint8_t *)pwmVals, 2);
    rf_driver.waitPacketSent();
    digitalWrite(LED_BUILTIN, LOW);
    timeTwo = millis();
    Serial.println((String)(timeTwo - timeOne));
    delay(50);
    
}
