//
// Created by Riccardo Marino on 6/2/20.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Arduino.h"
#include "Globals.hpp"

void buzzSOS();

void  buzz(byte repetitions, byte duration, uint16_t toneValue);

void blinkLedNonBlocking();

void printNTimes(char c, byte rep);


#endif //UTILS_HPP
