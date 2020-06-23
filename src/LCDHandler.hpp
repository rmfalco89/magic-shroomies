//
// Created by Riccardo Marino on 6/4/20.
//

#ifndef LCDHANDLER_HPP
#define LCDHANDLER_HPP

#include "Arduino.h"

#include "Globals.hpp"

void showLogo();

void setupLcd();

void loopLcd();

void drawBitmap(void);

void drawText(char * content);

#endif //LCDHANDLER_HPP
