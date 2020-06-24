//
// Created by Riccardo Marino on 6/10/20.
//

#ifndef HEATBLOCK_H
#define HEATBLOCK_H

class HeatBlock {
private:
    byte heatLampPin;
    byte fanSpeedPin;
    byte fanSpeed;
    byte normalFanSpeed;
    byte coolingDownFanSpeed;
    bool active;
    bool heatLampOn;
    bool coolingDown;
    bool movingAir;
    uint32_t lastActionTime;

public:
    bool heatLampInvertedRelay;

    HeatBlock(byte heatLampPin, byte fanSpeedPin, byte normalFanSpeed, byte coolingDownFanSpeed,
              bool heatLampInvertedRelay = false);

    void heatUp();

    void stopHeatingUp();

    void moveAir();

    void shutDown();

    void switchHeatLamp(bool);

    void setFanSpeed(byte);

    void loop();

    bool isActive() { return this->active; }

    void handleStatusLEDs();

};

#endif //HEATBLOCK_H
