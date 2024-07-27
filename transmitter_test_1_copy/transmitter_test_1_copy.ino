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

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    // Initialize ASK Object
    rf_driver.init();
}
 
void loop()
{
    const char *msg = "Hi";
    digitalWrite(LED_BUILTIN, HIGH);  // Light is on while transmitting
    rf_driver.send((uint8_t *)msg, strlen(msg));
    rf_driver.waitPacketSent();
    digitalWrite(LED_BUILTIN, LOW);
    delay(2000);
}