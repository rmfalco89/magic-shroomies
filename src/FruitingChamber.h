//
// Created by Riccardo Marino on 6/10/20.
//

#ifndef FRUITINGCHAMBER_H
#define FRUITINGCHAMBER_H

#include "Arduino.h"
#include "HeatBlock.h"
#include "TempSensor.h"

void setupFC();

void loopFC();

void fcSwitchFAE(bool on, bool shortPush, int speed = -1);

void fcSwitchHumidifier(bool on);

void setFcTargetTemp(float temp);

void setFcTargetHumidity(float humidity);

void setFCHeatLamp(bool on);

void setFCHeatFanToSpeed(int speed);

void getFCSummary(char * s);

// for debugging purpose only
extern HeatBlock *fcHeatBlock;
extern DHTSensor *fcTempSensor;

#endif //FRUITINGCHAMBER_H
