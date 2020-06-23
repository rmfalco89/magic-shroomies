/**
 * Magic Shroomies!
 *
 * Handles a growing chamber and a fruiting chamber, each equipped with a DHT temperature and humidity sensor,
 * plus a heating block to gently heat the air.
 * The fruiting chamber is also equipped with a mini humidifier and an extra fan for fresh air exchange.
 *
 * Provides a serial interface to inject commands on the fly, and a GUI to display the current temperatures and
 * even set the target temp and humidity.
 */
#define byte uint8_t

#include "Arduino.h"

#include "Globals.hpp"
#include "LCDHandler.hpp"
#include "GUI.h"
#include "FruitingChamber.h"
#include "SerialUtils.hpp"
#include "utils.hpp"
#include "MemoryFree.hpp"

void setup() {
    Serial.begin(9600);

    pinMode(ledPin, OUTPUT);

    printNTimes('#', 40);

    Serial.print("freeMemory before logo =");
    Serial.println(freeMemory());
    showLogo();
    Serial.print("freeMemory after logo =");
    Serial.println(freeMemory());
    delay(1000);


    setupLcd();
    setupGUI();
    setupFC();
    Serial.println();
    printNTimes('*', 40);
    Serial.println(F("****** Welcome to Magic Shroomies ******"));
    Serial.println(F("**** ------ FRUITING CHAMBER ------ ****"));
    printNTimes('*', 40);
    Serial.println();

    buzz(1, 300, (uint16_t) 220);
}

unsigned long prevMillis = 0;
void loop() {
    if (SHROOMIES_DEBUG)
        readSerial(parseInput);

    blinkLedNonBlocking();

    loopFC();
    loopGUI();

//    loopLcd();
    if (!SHROOMIES_DEBUG)
        loopLcd();
    else {
        Serial.print("LCD content: ");
        char buf[50];
        myGui->getLCDContent(buf);
        Serial.println(buf);
    }

    if (millis() > prevMillis + 8000) {
        prevMillis = millis();
        Serial.print("freeMemory()=");
        Serial.println(freeMemory());
    }

    delay(100);
}

// works
//RAM:   [====      ]  44.8% (used 917 bytes from 2048 bytes)
//Flash: [========  ]  81.7% (used 25106 bytes from 30720 bytes)


// also this one WORKS (no lcd, yes debug):
//RAM:   [=====     ]  45.8% (used 937 bytes from 2048 bytes)
//Flash: [========  ]  82.5% (used 25346 bytes from 30720 bytes)