//
// Created by Riccardo Marino on 6/7/20.
//

#include "GUI.h"

#include <ezButton.h>
#include "LCDHandler.hpp"

#include "FruitingChamber.h"

GUI *myGui = nullptr;
ezButton buttonIce(iceButtonPin);
ezButton buttonPlus(button1Pin);
ezButton buttonMinus(button2Pin);
ezButton buttonMode(button3Pin);

/**
 * Button States:
 * 0 = no press
 * 0 = short press
 * 0 = long press
 */
int buttonModeState = 0;
int buttonPlusState = 0;
int buttonMinusState = 0;
//static uint32_t prevMillis = 0;
byte minUpdateInterval = 50;

void setupGUI() {
    buttonIce.setDebounceTime(50);
    buttonPlus.setDebounceTime(50);
    buttonMinus.setDebounceTime(50);
    buttonMode.setDebounceTime(50);

    myGui = new GUI();
    Serial.println(F("setupGUI completed"));
}

void loopGUI() {
    // must call the loop function on the buttons
    buttonIce.loop();
    buttonPlus.loop();
    buttonMinus.loop();
    buttonMode.loop();

    if (buttonIce.isReleased())
        fcSwitchIce();

    if (buttonMode.isReleased())
        myGui->buttonModeAction();

    if (buttonPlus.isPressed())
        myGui->buttonPlusAction();
    else if (buttonMinus.isPressed())
        myGui->buttonMinusAction();

}

GUI::GUI(Mode m) : currentMode(m) {
    if (SHROOMIES_DEBUG) {
        Serial.print(F("Inited GUI - initial mode: "));
        Serial.println(this->currentMode);
    }
}

void GUI::buttonModeAction() {
    this->currentMode = static_cast<Mode>(this->currentMode + 1);
    if (this->currentMode == Mode::num_values)
        this->currentMode = Mode::hello;
}


void GUI::buttonPlusAction() {
    Serial.println(F("buttonPlusAction"));
}

void GUI::buttonMinusAction() {
    Serial.println(F("buttonMinusAction"));
}

void GUI::getLCDContent(char *s) {
    switch (this->currentMode) {
        case hello:
            strcpy(s, "Shroomies!");
            break;
        case fcShow:
            getFCSummary(s);
            break;
        default:
            strcpy(s, this->modesNames[this->currentMode]);
    }
}
