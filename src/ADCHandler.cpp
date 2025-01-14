/*****************************************
* Library for the ADS1115
*
* Written by Alec Robin Gould
*
*******************************************/
#include "ADCHandler.h"

constexpr uint8_t CONFIG_REG = 0x01;
constexpr uint8_t CONVERSION_REG = 0x00;
constexpr uint8_t AINmux[] = { B100, B101, B110, B111, B100, B101, B110, B111 };

ADCHandler::ADCHandler(uint8_t address1, uint8_t address2)
    :  _address1(address1), _address2(address2), _resolution(32768.0), invResolution(1.0/32768.0), refVoltage(4.096) {}

void ADCHandler::begin() {
    Wire.begin();
}

void ADCHandler::setRefVoltage(double voltage) {
    refVoltage = voltage;
}

void ADCHandler::setResolution(uint16_t res) {
    _resolution = res;
    invResolution = 1.0 / static_cast<double>(res);
}

void ADCHandler::setSampleRate(uint8_t sps) {
    _currentSPS = sps & 0x07; // Ensure SPS is within valid range
}

double ADCHandler::getVoltage(int16_t rawValue) const {
    return rawValue * refVoltage * invResolution;
}

int16_t ADCHandler::readChannel(uint8_t channel) {
    if (channel >= 8) {
        // Handle invalid channel
        return -1; // Return an error value
    }
    if (channel < 4) {
        return readConversion(_address1, AINmux[channel]);
    } else {
        return readConversion(_address2, AINmux[channel]);
    }
}

int16_t ADCHandler::writeConfig(uint8_t address, uint8_t mux, uint8_t sps) {
    uint16_t config = 0;
    config |= (1 << 15);                  // Start single-conversion
    config |= (mux & 0x07) << 12;        // Set MUX for channel
    config |= (sps & 0x07) << 5;         // Set SPS
    config |= (1 << 9);                  // Single-shot mode
    config |= (1 << 8);                  // ±4.096V range

    Wire.beginTransmission(address);
    Wire.write(CONFIG_REG);
    Wire.write((config >> 8) & 0xFF); // MSB
    Wire.write(config & 0xFF);        // LSB
    if (Wire.endTransmission() != 0) {
        // Handle error, e.g., log or set an error flag
        return -2; // Return an error value
        Serial.println("Failed to write configuration!");
    }
    return 0;
}

int16_t ADCHandler::readConversion(uint8_t address, uint8_t channel) {

    writeConfig(address, channel, 0x02); // Set SPS to 32

    muxConfig = (1 << 7) | (channel << 4) | B0011;

    Wire.beginTransmission(address);
    Wire.write(CONVERSION_REG);
    Wire.write(muxConfig);
    Wire.write(spsConfig);
    waitConversion = 31.3;
    waitConversion = ceil(waitConversion * 1.1 + 0.02);

    if (Wire.endTransmission() != 0) {
        // Handle transmission error
        return -2; // Return an error value
    }
    delay(waitConversion);

    Wire.beginTransmission(address);
    Wire.write(CONVERSION_REG);  // Pointer to conversion reg.
    Wire.endTransmission();

    if (Wire.requestFrom(address, (uint8_t)2) != 2) {
        // Handle reception error
        return -3; // Return an error value
    }
    
    // Read MSB and LSB
    uint8_t msb = Wire.read();
    uint8_t lsb = Wire.read();

    // Combine MSB and LSB into a single 16-bit signed value
    int16_t result = (msb << 8) | lsb;
    return result;
}
