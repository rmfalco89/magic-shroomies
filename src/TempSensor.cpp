//
// Created by Riccardo Marino on 6/10/20.
//

#define MAX_READ_ERROR 10

#include "TempSensor.h"
#include "Globals.hpp"

DHTSensor::DHTSensor(byte pin, byte ledPin, byte sensorType, bool celsius, bool tempOnly) {
    digitalWrite(this->ledPin, LOW);

    this->sensor = new DHT_Unified(pin, sensorType);
    this->celsius = celsius;
    this->tempOnly = tempOnly;
    this->ledPin = ledPin;

    switch (sensorType) {
        case DHT11:
        case DHT21:
        case DHT22:
        default:
            this->numReadings = (SHROOMIES_DEBUG ? 1 : 2);
            break;
    }
    this->temperature = -99.;
    this->humidity = 1;

    // Initialize device.
    this->sensor->begin();
    Serial.println(F("DHTxx Unified Sensor Example"));
    // Print temperature sensor details.

    sensor_t sensor;
    this->sensor->temperature().getSensor(&sensor);
    // Set delay between sensor readings based on sensor details.
    this->delayMS = sensor.min_delay / 1000;

    if (SHROOMIES_DEBUG) {
        Serial.println(F("------------------------------------"));
        Serial.println(F("Temperature Sensor"));
        Serial.print(F("Sensor Type: "));
        Serial.println(sensor.name);
        Serial.print(F("Driver Ver:  "));
        Serial.println(sensor.version);
        Serial.print(F("Unique ID:   "));
        Serial.println(sensor.sensor_id);
        Serial.print(F("Max Value:   "));
        Serial.print(sensor.max_value);
        Serial.println(F("°C"));
        Serial.print(F("Min Value:   "));
        Serial.print(sensor.min_value);
        Serial.println(F("°C"));
        Serial.print(F("Resolution:  "));
        Serial.print(sensor.resolution);
        Serial.println(F("°C"));
        Serial.println(F("------------------------------------"));
        // Print humidity sensor details.
        this->sensor->humidity().getSensor(&sensor);
        Serial.println(F("Humidity Sensor"));
        Serial.print(F("Sensor Type: "));
        Serial.println(sensor.name);
        Serial.print(F("Driver Ver:  "));
        Serial.println(sensor.version);
        Serial.print(F("Unique ID:   "));
        Serial.println(sensor.sensor_id);
        Serial.print(F("Max Value:   "));
        Serial.print(sensor.max_value);
        Serial.println(F("%"));
        Serial.print(F("Min Value:   "));
        Serial.print(sensor.min_value);
        Serial.println(F("%"));
        Serial.print(F("Resolution:  "));
        Serial.print(sensor.resolution);
        Serial.println(F("%"));
        Serial.println(F("------------------------------------"));
    }
}

void DHTSensor::read() {
    sensors_event_t event;

    digitalWrite(this->ledPin, HIGH);

    if (SHROOMIES_DEBUG)
        Serial.println(F("Reading temp"));

    float total = 0;
    byte count = 0;
    for (byte i = 0; i < this->numReadings; i++) {
        this->sensor->temperature().getEvent(&event);

        if (!isnan(event.temperature)) {
            total += event.temperature;
            count++;
            Serial.print(F("Read temp: "));
            Serial.println(event.temperature);

            if (SHROOMIES_DEBUG) {
                Serial.println(event.temperature);
            }

            if (i + 1 < this->numReadings)
                delay(1500);
        } else {
            Serial.println(F("NAN"));
            delay(2000);
        }
    }

    // If sensor breaks, set temp to -100 to trigger SOS buzz
    if (count == 0) {
        this->consecutiveReadError++;
        if (this->consecutiveReadError >= MAX_READ_ERROR) {
            this->temperature = -100.;
        }
    } else {
        this->temperature = total / count;
        if (!this->celsius)
            this->temperature = this->toF(this->temperature);
        this->consecutiveReadError = 0;
    }

    if (!tempOnly) {
        if (SHROOMIES_DEBUG)
            Serial.println(F("Reading humidity"));
        total = 0;
        count = 0;
        for (int i = 0; i < this->numReadings; i++) {
            this->sensor->humidity().getEvent(&event);

            if (!isnan(event.relative_humidity)) {
                total += event.relative_humidity;
                count++;
                Serial.print(F("Read RH: "));
                Serial.println(event.relative_humidity);

                if (i + 1 < this->numReadings)
                    delay(1500);
            } else {
                Serial.println(F("NAN"));
                delay(2000);
            }
        }

        // If sensor breaks, set humidity to 0 to trigger SOS buzz
        if (count == 0) {
            this->consecutiveReadError++;
            if (this->consecutiveReadError >= MAX_READ_ERROR) {
                this->humidity = 0;
            }
        } else {
            this->humidity = (byte) (total / count);
            this->consecutiveReadError = 0;
        }
    }

    digitalWrite(this->ledPin, LOW);
}

float DHTSensor::toF(float c) {
    return ((c * 9) + 3) / 5 + 32;
}