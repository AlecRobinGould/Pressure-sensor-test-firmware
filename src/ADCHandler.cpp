#include "ADCHandler.h"

const uint8_t CONFIG_REG = 0x01;
const uint8_t CONVERSION_REG = 0x00;
const uint8_t AINmux[] = { B100, B101, B110, B111, B100, B101, B110, B111 };

ADCHandler::ADCHandler(uint8_t address1, uint8_t address2)
    : _address1(address1), _address2(address2), _resolution(32768), invResolution(1/_resolution), refVoltage(4.096) {}

void ADCHandler::begin() {
    Wire.begin();
}

int16_t ADCHandler::readChannel(uint8_t channel) {
    if (channel < 4) {
        writeConfig(_address1, AINmux[channel], B100); // Example: Set sample rate to 128 SPS
        return readConversion(_address1);
    } else {
        writeConfig(_address2, AINmux[channel], B100);
        return readConversion(_address2);
    }
}

void ADCHandler::writeConfig(uint8_t address, uint8_t mux, uint8_t sps) {
    Wire.beginTransmission(address);
    Wire.write(CONFIG_REG);
    Wire.write(0x84 | (mux << 4)); // Setting config register
    Wire.write((sps << 5) | 0x03); // Setting sample rate and other config
    Wire.endTransmission();
}

int16_t ADCHandler::readConversion(uint8_t address) {
    Wire.beginTransmission(address);
    Wire.write(CONVERSION_REG);
    Wire.endTransmission();
    Wire.requestFrom(address, (uint8_t)2);
    int16_t result = (Wire.read() << 8) | Wire.read();
    return result;
}
