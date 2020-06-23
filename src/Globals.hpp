//
// Created by Riccardo Marino on 5/31/20.
//

#define USING_ATMEGA_ARDUINO_NANO

#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "Arduino.h"

const bool SHROOMIES_DEBUG = false;

/*** Common vars ****/
const byte maxStrLength = 10;
const byte maxCmdArgs = 3;
const byte maxReceivedCharsLen = maxStrLength * maxCmdArgs + maxCmdArgs;
extern char receivedChars [];
extern char currentCmd [];

/**** Global conf values ****/
extern const byte fcNormalFanSpeed;
extern const byte fcCoolingDownFanSpeed;
extern const byte criticalHighTemp;

/********************* PINS *********************/

/**** FRUITING CHAMBER (FC) ****/
extern byte fcTempPin;
extern byte fcTempOnLedPin;
extern byte fcHeatLampPin;
extern byte fcHeatFanSpeedPin;
extern byte fcFAEFanPin;
extern byte fcHumidifierPin;
extern byte fcHeatFanDirection1Pin;
extern byte fcHeatFanDirection2Pin;
extern byte fcFAEDirection1Pin;
extern byte fcFAEDirection2Pin;
extern byte fcBuzzerPin;

/**** Others ****/
extern byte iceButtonPin;
extern byte button1Pin;
extern byte button2Pin;
extern byte button3Pin;

extern byte ledPin;
extern byte fcBuzzerPin;


#endif //GLOBALS_HPP

