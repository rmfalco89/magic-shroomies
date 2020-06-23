//
// Created by Riccardo Marino on 6/10/20.
//
#include "Arduino.h"
#include "HeatBlock.h"
#include "Globals.hpp"

const uint16_t maxMillisOn = 30000;
const uint16_t minMillisOff = 30000;
const uint32_t coolingDownMillis = 120000;

HeatBlock::HeatBlock(byte heatLampPin, byte fanSpeedPin, byte normalFanSpeed, byte coolingDownFanSpeed,
                     bool heatLampInvertedRelay) {
    pinMode(heatLampPin, OUTPUT);
    pinMode(fanSpeedPin, OUTPUT);

    this->heatLampPin = heatLampPin;
    this->fanSpeedPin = fanSpeedPin;
    this->normalFanSpeed = normalFanSpeed;
    this->coolingDownFanSpeed = coolingDownFanSpeed;

    this->active = false;
    this->heatLampOn = false;
    this->coolingDown = false;
    this->movingAir = false;

    this->lastActionTime = 0;
    this->heatLampInvertedRelay = heatLampInvertedRelay;

    this->fanSpeed = 0;

    this->switchHeatLamp(false);
}

void HeatBlock::heatUp() {
    if (SHROOMIES_DEBUG) {
        Serial.println(F("Starting heat block"));
    }
    this->active = true;
}

void HeatBlock::stopHeatingUp() {
    if (SHROOMIES_DEBUG)
        Serial.println(F("Stopping heat block"));

    this->active = true;
    this->coolingDown = true;
    this->movingAir = false;
}

void HeatBlock::moveAir() {
    if (SHROOMIES_DEBUG)
        Serial.println(F("Moving air in heat block"));

    this->active = true;

    if (!this->movingAir) {
        this->setFanSpeed(normalFanSpeed);
        this->movingAir = true;
    }
}

void HeatBlock::shutDown() {
    if (SHROOMIES_DEBUG)
        Serial.println(F("Shutting down heat block"));

    this->active = false;
    this->switchHeatLamp(false);
    this->setFanSpeed(0);
}

void HeatBlock::switchHeatLamp(bool on) {
    if (SHROOMIES_DEBUG) {
        Serial.print(F("Turning heat lamp "));
        Serial.println(on ? F("on") : F("off"));
    }

    this->heatLampOn = on;
    if (this->heatLampInvertedRelay)
        on = !on;
    digitalWrite(this->heatLampPin, on ? HIGH : LOW);
    this->lastActionTime = millis();
}

void HeatBlock::setFanSpeed(byte speed) {
    if (SHROOMIES_DEBUG) {
        Serial.print(F("Setting heat fan to speed "));
        Serial.println(speed);
    }

    analogWrite(this->fanSpeedPin, speed);
    this->lastActionTime = millis();
}

void HeatBlock::loop() {
    /**
     * If active is false, just turn off the heat lamp and the fan.
     * Otherwise, turn on the heat lamp at intervals of 30seconds,
     */
    if (!this->active) {
        return;
    }

    if (!this->coolingDown) {
        // not cooling down, but active. Switch heat lamp on and off every TOT seconds, and fan always on at low speed
        if (this->heatLampOn && millis() > this->lastActionTime + maxMillisOn)
            // on for too long, switch off!
            this->switchHeatLamp(false);

        else if (!this->movingAir && !this->heatLampOn && millis() > this->lastActionTime + minMillisOff) {
            // off for enough time, switch it on again and make sure that fan is spinning at normal speed!
            this->switchHeatLamp(true);
            this->setFanSpeed(this->normalFanSpeed);
        }
    }
    if (this->coolingDown) {
        if (this->heatLampOn) { // heat lamp is on, turn it off, set fan speed to high, and start count down.
            this->switchHeatLamp(false);
            this->setFanSpeed(this->coolingDownFanSpeed);
            this->lastActionTime = millis();
        }
        // heat lamp is off. Check if enough time has passed, and turn everything off.
        if (millis() > this->lastActionTime + coolingDownMillis) {
            this->switchHeatLamp(false); // this should be unnecessary, but keep it just in case.
            this->setFanSpeed(0);
            this->active = false;
            this->coolingDown = false;
        }
    }
}