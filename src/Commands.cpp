//
// Created by Riccardo Marino on 6/2/20.
//

#include "Arduino.h"
#include "Commands.hpp"
#include "FruitingChamber.h"
#include "GUI.h"

void execCmd(char cmdParts[][maxStrLength]) {
    // blink led
    if (strcmp(cmdParts[0], "blink") == 0) {
        int pin = atoi(cmdParts[1]);
        if (!isnan(pin))
            ledPin = pin;
    } else if (strcmp(cmdParts[0], "heat-fan") == 0) {
        int speed;
        if (strcmp(cmdParts[1], "off") == 0)
            speed = 0;
        else
            speed = atoi(cmdParts[1]);
        if (!isnan(speed)) {
            Serial.print(F("Setting FC heat-fan to speed "));
            Serial.println(speed);
            setFCHeatFanToSpeed(speed);
        }
    } else if (strcmp(cmdParts[0], "heat-lamp") == 0) {
        bool on = strcmp(cmdParts[1], "on") == 0;
        Serial.print(F("Turning "));
        Serial.print(on ? F("on") : F("off"));
        Serial.println(F(" FC heat-lamp"));

        setFCHeatLamp(on);

    } else if (strcmp(cmdParts[0], "fae") == 0) {
        if (strcmp(cmdParts[1], "on") == 0) {
            Serial.println(F("Turn FC FAE-fan ON ar normal speed"));
            fcSwitchFAE(true, true);
        } else if (strcmp(cmdParts[1], "off") == 0) {
            Serial.println(F("Turn FC FAE-fan OFF"));
            fcSwitchFAE(false, true);
        } else {
            int speed = atoi(cmdParts[1]);
            if (!isnan(speed)) {
                Serial.print(F("Turn FC FAE-fan ON at speed "));
                Serial.println(speed);
                fcSwitchFAE(true, true, speed);
            }
        }
    } else if (strcmp(cmdParts[0], "humidifier") == 0) {
        bool on = strcmp(cmdParts[1], "on") == 0;
        Serial.print(F("Turning "));
        Serial.print(on ? F("on") : F("off"));
        Serial.println(F(" FC humidifier"));

        fcSwitchHumidifier(on);

    } else if (strcmp(cmdParts[0], "heat-block") == 0) {
        bool on = strcmp(cmdParts[1] ,"on")== 0;
        Serial.print(F("Turning "));
        Serial.print(on ? F("on") : F("off"));
        Serial.println(F(" FC heat-block"));

        if (on)
            fcHeatBlock->heatUp();
        else
            fcHeatBlock->stopHeatingUp();
    }
}
