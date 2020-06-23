//
// Created by Riccardo Marino on 6/7/20.
//

#include "Arduino.h"

#ifndef GUI_H
#define GUI_H

void setupGUI();

void loopGUI();


class GUI {
public:
    /**
     * hello: Just show an hello message
     * fcShow : Show Temp & Humidity in Fruiting Chamber
     * gcShow : Show Temp in Growing Chamber
     * fcSetTemp : Set Temp  in Fruiting Chamber
     * fcSetHumidity : Set Humidity in Fruiting Chamber
     * gcSetTemp : Set Temp in Growing Chamber
     */
    enum Mode {
        hello, fcShow, fcSetTemp, fcSetHumidity, num_values
    };
    const char *modesNames[5] = {"hello", "fcShow", "fcSetTemp", "fcSetHumidity", "num_values"};
    Mode currentMode; //current display state

    GUI(Mode=Mode::fcShow);

    // Member functions declaration
    int getState();

    void getLCDContent(char * buf);

    void nextState();

    void previousState();

    char* getCurrentMenu();

    void buttonModeAction();

    void buttonMode2Action();

    void buttonPlusAction();

    void buttonMinusAction();

};

extern GUI *myGui;


#endif //GUI_H
