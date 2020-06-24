//
// Created by Riccardo Marino on 6/10/20.
//
#include "FruitingChamber.h"

#include "Globals.hpp"
#include "utils.hpp"
#include "MemoryFree.hpp"

/**
 * The fruiting chamber is a box with holes, that includes:
 * - a temperature and relative-humidity sensors
 * - a heat lamp
 * - a fan to gently move the warmth generated by the heat lamp and distribute it evenly in the chamber
 * - a fan to allow FAE (Fresh Air Exchange)
 * - a humidifier
 *
 * Temperature and humidity are read every ~5 seconds, the heat lamp and the humidifier are activated in order to keep
 * a constant temperature and humidity.
 */

static byte fcTargetTemp = 70;
static byte tempGap = 1;

static byte fcTargetHumidity = 95;
static byte humidityGap = 2;
static uint32_t lastHumidifierSwitchMillis = 0;
static uint16_t humidifierMaxActivityMillis = 60000;

static uint32_t prevMillis = 0;
uint16_t updateInterval = 5000;

static const uint32_t FAEInterval = (uint32_t) 1000 * 60 * 60 * 4; // turn on the FAE fan every 4 hours
static uint32_t FAEDuration = (uint32_t) 1000 * 60 * 2; // turn on FAE fan for 2 minutes
static uint32_t lastFAEOnMillis = 0;
static const uint32_t FAEIntervalShortPush =
        (uint32_t) 1000 * 60 * 15; // turn on the FAE fan every 15 minutes for a quick  push
static uint16_t FAEDurationShortPush = 1000L * 5; // duration of quick push (5 seconds)
static uint32_t lastFAEOnShortPushMillis = 0;
static bool FAEFanOn = false;
static bool FAEFanOnShortPush = false;

const byte fcNormalFanSpeed = 60;
const byte fcCoolingDownFanSpeed = 80;
static const uint8_t FAEFanSpeed = 255;
bool humidifierOn = true;

bool ice = false;

byte tempReadFrequency = 3;
byte repetition = 0;

HeatBlock *fcHeatBlock;
DHTSensor *fcTempSensor;

void setupFC() {
    pinMode(fcHeatFanDirection1Pin, OUTPUT);
    pinMode(fcHeatFanDirection2Pin, OUTPUT);
    digitalWrite(fcHeatFanDirection1Pin, LOW);
    digitalWrite(fcHeatFanDirection2Pin, HIGH);

    pinMode(fcFAEDirection1Pin, OUTPUT);
    pinMode(fcFAEDirection2Pin, OUTPUT);
    digitalWrite(fcFAEDirection1Pin, HIGH);
    digitalWrite(fcFAEDirection2Pin, LOW);
    pinMode(fcFAEFanPin, OUTPUT);

    pinMode(fcHumidifierPin, OUTPUT);

    fcHeatBlock = new HeatBlock(fcHeatLampPin, fcHeatFanSpeedPin, fcNormalFanSpeed, fcCoolingDownFanSpeed, false);
    fcTempSensor = new DHTSensor(fcTempPin, fcTempOnLedPin, DHT22, false, false);

    fcTempSensor->read();

    if (SHROOMIES_DEBUG) {
        Serial.println(F("End of setupFC"));
        updateInterval += 5000;
    }

    Serial.println(F("setupFC completed"));
}

void loopFC() {
    // update every updateInterval millis
    if (millis() < prevMillis + updateInterval)
        return;

    ++repetition %= tempReadFrequency;

    prevMillis = millis();

    if (SHROOMIES_DEBUG)
        Serial.println(F("FC loop"));

    // read temp and humidity
    if (repetition == 0)
        fcTempSensor->read();

    // if sensor didn't return valid values for various consecutive times, turn off heat block and buzzSOS.
    if (fcTempSensor->temperature == -100. || fcTempSensor->humidity == 0) {
        fcHeatBlock->shutDown();
        buzzSOS();
        return;
    }

    /*** handle temp ***/
    if (SHROOMIES_DEBUG)
        Serial.println(F("FC - handle temperature"));

    if (fcTempSensor->temperature < fcTargetTemp - tempGap && !fcHeatBlock->isActive()) // low temp, heat up
        fcHeatBlock->heatUp();
    else if (ice && fcTempSensor->temperature > fcTargetTemp + tempGap) // temp too high, but ice in FC -> cool down
        fcHeatBlock->moveAir();
    else if (fcHeatBlock->isActive() && fcTempSensor->temperature > fcTargetTemp) // warm enough, cool heat lamp down
        fcHeatBlock->stopHeatingUp();

    /*** handle humidity ***/
    if (SHROOMIES_DEBUG)
        Serial.println(F("FC - handle humidity"));
    if (fcTempSensor->humidity < (fcTargetHumidity - humidityGap)) {// RH less than target - allowed gap
        if (!humidifierOn && millis() > lastHumidifierSwitchMillis + humidifierMaxActivityMillis / 2)
            fcSwitchHumidifier(true);
        else if (humidifierOn && millis() > lastHumidifierSwitchMillis + humidifierMaxActivityMillis)
            fcSwitchHumidifier(false);
    } else if (fcTempSensor->humidity >= fcTargetHumidity)
        fcSwitchHumidifier(false);


    /*** handle FreshAirExchange ***/
    if (FAEFanOn) {
        if (millis() > (lastFAEOnMillis + FAEDuration)) {
            if (SHROOMIES_DEBUG)
                Serial.println("Turning FAE fan off after 2 minutes");
            fcSwitchFAE(false, false);
        }
    } else {
        if (millis() > (lastFAEOnMillis + FAEInterval)) { // if currently off check if it's time to turn it on
            if (SHROOMIES_DEBUG)
                Serial.println("Turning FAE fan on");
            fcSwitchFAE(true, false);
        }
    }

    /*** handle FAE short push ***/
    if (FAEFanOnShortPush) {
        if (millis() > (lastFAEOnShortPushMillis + FAEDurationShortPush)) {
            if (SHROOMIES_DEBUG)
                Serial.println("Turning FAE fan off after short push");
            fcSwitchFAE(false, true);
        }
    } else if (!FAEFanOn) {
        if (millis() >
            (lastFAEOnShortPushMillis + FAEIntervalShortPush)) { // if currently off check if it's time to turn it on
            fcSwitchFAE(true, true);
        }
    }

    // check that temp never goes too hot
    if (fcTempSensor->temperature >= criticalHighTemp) {
        fcHeatBlock->switchHeatLamp(false);
        fcHeatBlock->stopHeatingUp();
        buzzSOS();
    }

    fcHeatBlock->loop();
}

void fcSwitchFAE(bool on, bool shortPush, int speed) {
    if (speed == -1)
        speed = FAEFanSpeed;

    if (on && !FAEFanOn) {
        if (shortPush) {
            lastFAEOnShortPushMillis = millis(); // register last activation time of short FAE push
            FAEFanOnShortPush = true;
            FAEFanOn = true;
            analogWrite(fcFAEFanPin, speed); // activate it
            if (SHROOMIES_DEBUG) {
                buzz(1, 50, 500);
                Serial.println(F("Turned on fc-FAE-fan for a SHORT PUSH"));
            }

        } else {
            lastFAEOnMillis = millis(); // register last activation time of FAE
            FAEFanOn = true;
            analogWrite(fcFAEFanPin, speed); // activate it
            if (SHROOMIES_DEBUG)
                Serial.println(F("Turned on fc-FAE-fan"));
        }
    } else if (!on && FAEFanOn) {
        FAEFanOn = false;
        FAEFanOnShortPush = false;
        analogWrite(fcFAEFanPin, LOW); // turn it off!
        if (SHROOMIES_DEBUG)
            Serial.println(F("Turned off fc-FAE-fan"));
    }
}

void setFCHeatLamp(bool on) {
    fcHeatBlock->switchHeatLamp(on);
}

void setFCHeatFanToSpeed(int speed) {
    fcHeatBlock->setFanSpeed(speed);
}

void fcSwitchHumidifier(bool on) {
    Serial.print(F("Switched humidifier "));
    Serial.println(on ? F("on") : F("off"));
    lastHumidifierSwitchMillis = millis();
    humidifierOn = on;
    digitalWrite(fcHumidifierPin, on ? HIGH : LOW);
}

void setFcTargetTemp(float temp) {
    fcTargetTemp = temp;
}

void setFcTargetHumidity(float humidity) {
    fcTargetHumidity = humidity;
}

void fcSwitchIce() {
    ice = !ice;
}

void getFCSummary(char *s) {
    bool showHumidifierAndFAE = false;
    bool showIce = true;
    bool showTime = true;
    char convStr[10];

    strcpy(s, "");
    // temp
    dtostrf(fcTempSensor->temperature, 2, 1, convStr);
    strcat(s, convStr);
    strcat(s, fcTempSensor->celsius ? "C" : "F");
    strcat(s, " ");

    // humidity
    snprintf(convStr, 3, "%d", fcTempSensor->humidity);
    strcat(s, convStr);
    strcat(s, "%");

    // empty line
    strcat(s, "> ");

    // humidifier and FAE fan
    if (showHumidifierAndFAE) {
        strcat(s, ">H. ");
        strcat(s, humidifierOn ? "1" : "0");
        strcat(s, " F. ");
        strcat(s, FAEFanOn ? "1" : "0");
    }

    if (showIce) {
        strcat(s, ">");
        if (!ice)
            strcat(s, "No ");
        strcat(s, "Ice");
    }

    if (showTime) {
        unsigned int secInHour = 3600;
        byte secInMin = 60;
        byte hrsInDay = 24;
        // macros from DateTime.h
        uint32_t timeInSeconds = millis() / 1000;

        strcat(s, ">");

        // days
        if (timeInSeconds > secInHour * hrsInDay) {
            itoa((int) (timeInSeconds / (secInHour * hrsInDay)), convStr, 10);
            strcat(s, convStr);
            strcat(s, "d ");
        }

        // hours
        itoa((int) (timeInSeconds % (secInHour * hrsInDay) / secInHour), convStr, 10);
        strcat(s, convStr);
        strcat(s, "h");

        // minutes
        itoa((int) ((timeInSeconds / secInMin) % secInMin), convStr, 10);
        strcat(s, convStr);
        strcat(s, "m");

        // seconds
        itoa((int) (timeInSeconds % secInMin), convStr, 10);
        strcat(s, convStr);
        strcat(s, "s");
    }
}
