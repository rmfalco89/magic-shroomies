//
// Created by Riccardo Marino on 6/10/20.
//

#ifndef TEMPSENSOR_H
#define TEMPSENSOR_H

#include "Arduino.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

class DHTSensor {
private:
    DHT_Unified *sensor;
    byte numReadings;
    bool tempOnly;
    byte ledPin;
    byte consecutiveReadError;
    uint32_t delayMS;

public:
    bool celsius;
    float temperature;
    byte humidity;

    DHTSensor(byte pin, byte ledPin, byte sensorType, bool celsius, bool tempOnly = false);

    void read();

    float toF(float c);
};

#endif //TEMPSENSOR_H
