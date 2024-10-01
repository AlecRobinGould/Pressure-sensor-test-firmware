
#ifndef ADCHANDLER_H
#define ADCHANDLER_H

#include <Wire.h>
#include <Arduino.h>

class ADCHandler {
public:
    ADCHandler(uint8_t address1, uint8_t address2);
    void begin();
    int16_t readChannel(uint8_t channel);
    const double refVoltage;
    double invResolution;

private:
    uint8_t _address1, _address2;
    void writeConfig(uint8_t address, uint8_t mux, uint8_t sps);
    int16_t readConversion(uint8_t address);
    const int _resolution;
};

#endif
