//
// Created by Riccardo Marino on 6/1/20.
//

#include "Globals.hpp"

/*** Common vars ****/
char receivedChars[maxReceivedCharsLen];
char currentCmd[maxStrLength];


/**** Global conf values ****/
const byte criticalHighTemp = 90;


/********************* PINS *********************/

#ifdef USING_ATMEGA_ARDUINO_NANO

byte ledPin = 13;

byte button1Pin = 50;
byte button2Pin = 51;
byte button3Pin = 52;

/**** FRUITING CHAMBER (FC) ****/
byte fcHeatFanDirection1Pin = 6;
byte fcHeatFanDirection2Pin = 7;
byte fcTempPin = 12;
byte fcTempOnLedPin = 8;
byte fcHeatLampPin = 10;
byte fcHumidifierPin = 11;
byte fcFAEDirection1Pin = 2;
byte fcFAEDirection2Pin = 4;
byte fcHeatFanSpeedPin = 5;
byte fcFAEFanPin = 3;
byte fcBuzzerPin = 9;


#endif //USING_ATMEGA_ARDUINO_NANO
