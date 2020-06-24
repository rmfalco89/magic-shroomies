//
// Created by Riccardo Marino on 6/2/20.
//

#include "utils.hpp"

uint32_t previousMillis;
int blinkInterval = 2500, ledState = LOW;

byte short_millis = 30;
byte repetitions = 10;
byte count = 0;

void blinkLedNonBlocking() {
    if (count == repetitions) {
        if (millis() > previousMillis + blinkInterval + repetitions * short_millis) {
            previousMillis = millis();
            count = 0;
        }
    } else if ((millis() > (previousMillis + (count + 1) * short_millis)) && count < repetitions)
        count++;

    ledState = (count % 2 == 0) ? LOW : HIGH;
    digitalWrite(ledPin, ledState);
}

void buzz(byte repetitions, byte duration, uint16_t toneValue) {
    for (byte i = 0; i < repetitions; i++) {
        tone(fcBuzzerPin, toneValue);
        delay(duration);
        noTone(fcBuzzerPin);
        if (i + 1 < repetitions)
            delay(duration);
    }
}

void buzzSOS() {
    int short_tone_delay = 100;
    int long_tone_delay = 400;
    int separator_delay = 300;

    for (byte i = 0; i < 2; i++) {

        // first set: 3 quick tones
        for (byte i = 0; i < 3; i++) {
            tone(fcBuzzerPin, 500);
            delay(short_tone_delay);
            noTone(fcBuzzerPin);
            delay(short_tone_delay);
        }

        delay(separator_delay);

        // second set: 3 long tones
        for (byte i = 0; i < 3; i++) {
            tone(fcBuzzerPin, 500);
            delay(long_tone_delay);
            noTone(fcBuzzerPin);
            delay(long_tone_delay);
        }

        delay(separator_delay);
    }
}

void printNTimes(char c, byte rep) {
    for (byte i = 1; i < rep; i++)
        Serial.print(c);
    Serial.println(c);
}