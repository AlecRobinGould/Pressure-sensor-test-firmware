#ifndef ADCHANDLER_H
#define ADCHANDLER_H

#include <Wire.h>
#include <Arduino.h>

class ADCHandler {
private:
    uint8_t _address1, _address2;
    int16_t writeConfig(uint8_t address, uint8_t mux, uint8_t sps);
    int16_t readConversion(uint8_t address, uint8_t channel);
    long _resolution;
    uint8_t _currentSPS;     // Current sample rate
    
public:
    void setRefVoltage(double voltage);
    void setResolution(uint16_t res);
    void setSampleRate(uint8_t sps);
    double getVoltage(int16_t rawValue) const;
    ADCHandler(uint8_t address1, uint8_t address2);
    void begin();
    int16_t readChannel(uint8_t channel);

    double invResolution;
    double refVoltage;       // Reference voltage

    int16_t MSB_LSB[2];  // ADC word
    uint8_t waitConversion;
    uint8_t muxConfig;
    uint8_t spsConfig;
};

#endif

/*For example, to write to the configuration register to set the ADS111x to continuous-conversion mode and then 
read the conversion result, send the following bytes in this order:

1. Write to Config register:
    • First byte: 10010000b (first 7-bit I2C address followed by a low R/W bit)
    • Second byte: 00000001b (points to Config register)
    • Third byte: 10000100b (MSB of the Config register to be written)
    • Fourth byte: 10000011b (LSB of the Config register to be written)
2. Write to Address Pointer register:
    • First byte: 10010000b (first 7-bit I2C address followed by a low R/W bit)
    • Second byte: 00000000b (points to Conversion register)
3. Read Conversion register:
    • First byte: 10010001b (first 7-bit I2C address followed by a high R/W bit)
    • Second byte: the ADS111x responds with the MSB of the Conversion register.
    • Third byte: the ADS111x responds with the LSB of the Conversion register.*/