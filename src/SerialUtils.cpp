//
// Created by Riccardo Marino on 5/30/20.
// Source of example: https://forum.arduino.cc/index.php?topic=396450.0
//
#include "Arduino.h"
#include "SerialUtils.hpp"

char startMarker = '>';
char continuationMarker = ':';
char endMarker = '\n';
static bool recvInProgress = false;
static bool continuationCmd = false;
byte receivedCharsIdx;

//============

void readSerial(void (*call_back)(bool)) {
    char rc;

    while (Serial.available() > 0) {
        rc = (char) Serial.read();

        if (recvInProgress)
            if (rc != endMarker) {
                receivedChars[receivedCharsIdx++] = rc;

                if (receivedCharsIdx + 2 > maxReceivedCharsLen) {
                    buzz(1, 50, 508);
                    recvInProgress = false;
                    continuationCmd = false;
                    receivedCharsIdx = 0;
                }
            } else {
                receivedChars[receivedCharsIdx] = '\0';

                call_back(continuationCmd);

                // prepare for new input
                recvInProgress = false;
                continuationCmd = false;
                receivedCharsIdx = 0;
            }
        else if (rc == startMarker) {
            recvInProgress = true;
        } else if (rc == continuationMarker) {
            recvInProgress = true;
            continuationCmd = true;
        }
    }
}

//============

void parseInput(bool cmdContinued) {
    const char *delimiters = " \n\0";

    // temporary copy necessary to protect the original data since strtok() replaces the delimiters with \0
    char tempChars[receivedCharsIdx + 1];
    strcpy(tempChars, receivedChars);

    // variables to hold the parsed data
    char cmdParts[maxCmdArgs + 1][maxStrLength];
    for (auto & cmdPart : cmdParts)
        strcpy(cmdPart, "N/A");

    byte cmdIdx = 0;

    if (cmdContinued) {
        strcpy(cmdParts[cmdIdx++], currentCmd);
    }

    char *token;
    token = strtok(tempChars, delimiters);

    while (token != nullptr && strcmp(token, "") != 0 && cmdIdx < maxCmdArgs) {
//        Serial.print("Token ");
//        Serial.print(cmdIdx);
//        Serial.print(": ");
//        Serial.println(token);

        strcpy(cmdParts[cmdIdx++], token);
        token = strtok(nullptr, delimiters); // this continues where the previous call left off
    }

    if (SHROOMIES_DEBUG) {
        Serial.print(F("Command: "));
        Serial.print(cmdParts[0]);

        Serial.print(F(" - [ "));
        for (int i = 1; i < maxCmdArgs; i++) {
            Serial.print("'");
            Serial.print(cmdParts[i]);
            Serial.print(F("' "));
        }
        Serial.println(F("]"));
    }

    strcpy(currentCmd, cmdParts[0]);
    execCmd(cmdParts);
}


//============


//void parseInputNotWorking3(bool cmdContinued) {
//    const char *delimiter = " \n";
//
//    receivedChars = String("cmd arg");
//
//    unsigned int str_l = receivedChars.length();
//    char tempChars[str_l];
//    strcpy(tempChars, receivedChars.c_str());
//
//    // variables to hold the parsed data
//    String cmd[maxCmdArgs];
//    int cmdIdx = 0;
//    if (cmdContinued) {
//        Serial.println(F("cmd continues!"));
//        cmd[cmdIdx++] = currentCmd;
//    }
//
//    char *token; // this is used by strtok() as an index
//    char *rest = tempChars;
//
//    token = strtok_r(tempChars, delimiter, &rest);
//    while (token != nullptr) {
//        Serial.print("Part: '");
//        Serial.print(token);
//        Serial.println("'");
//
//        cmd[cmdIdx++] = String(token);
//
//        token = strtok_r(rest, delimiter, &rest);
//    }
//
//    Serial.print("Got command: '");
//    Serial.print(cmd[0]);
//    Serial.print("' and parameters ");
//    String strList = listToStr(cmd, maxCmdArgs, 1);
//    Serial.println(strList.c_str());
//
//    execCmd(cmd, maxCmdArgs);
//}


//============


//void parseInputNotWorking(bool cmdContinued) {
//    const char *delimiter = " \n";
//
//    receivedChars = String("cmd arg");
//
//    unsigned int str_l = receivedChars.length();
//    char tempChars[str_l];
//    strcpy(tempChars, receivedChars.c_str());
//
//    Serial.print(F("length: "));
//    Serial.println(str_l);
//    Serial.print(F("receivedChars: <"));
//    Serial.print(receivedChars);
//    Serial.println(">");
//
//    // variables to hold the parsed data
//    String cmd[maxCmdArgs];
//    int cmdIdx = 0;
//    if (cmdContinued) {
//        Serial.println(F("cmd continues!"));
//        cmd[cmdIdx++] = currentCmd;
//    }
//
//    char *token; // this is used by strtok() as an index
//    char *rest = tempChars;
//
//    token = strtok_r(tempChars, delimiter, &rest);
//    while (token) {
//        Serial.print("Part: '");
//        Serial.print(token);
//        Serial.println("'");
//        cmd[cmdIdx++] = String(token);
//
//        token = strtok_r(rest, delimiter, &rest);
//    }
//
//    Serial.print("Got command: '");
//    Serial.print(cmd[0]);
//    Serial.print("' and parameters ");
//    String strList = listToStr(cmd, maxCmdArgs, 1);
//    Serial.println(strList.c_str());
//
//    execCmd(cmd, maxCmdArgs);
//}
