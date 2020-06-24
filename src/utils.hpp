//
// Created by Riccardo Marino on 6/2/20.
//

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Arduino.h"
#include "Globals.hpp"

//// macros from DateTime.h
///* Useful Constants */
//#define SECS_PER_MIN  (60U)
//#define SECS_PER_HOUR (3600U)
//#define SECS_PER_DAY  (SECS_PER_HOUR * 24)
//
///* Useful Macros for getting elapsed time */
//#define numberOfSeconds(_time_) (_time_ % SECS_PER_MIN)
//#define numberOfMinutes(_time_) ((_time_ / SECS_PER_MIN) % SECS_PER_MIN)
//#define numberOfHours(_time_) (( _time_% SECS_PER_DAY) / SECS_PER_HOUR)
//#define elapsedDays(_time_) ( _time_ / SECS_PER_DAY)


void buzzSOS();

void buzz(byte repetitions, byte duration, uint16_t toneValue);

void blinkLedNonBlocking();

void printNTimes(char c, byte rep);


#endif //UTILS_HPP
