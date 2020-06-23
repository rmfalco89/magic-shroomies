//
// Created by Riccardo Marino on 5/30/20.
//
#ifndef SERIALUTILS_HPP
#define SERIALUTILS_HPP

#include "Arduino.h"
#include "Globals.hpp"
#include "utils.hpp"
#include "Commands.hpp"


extern char startMarker;
extern char endMarker;

void readSerial(void (*call_back)(bool));

void parseInput(bool);

#endif //SERIALUTILS_HPP
