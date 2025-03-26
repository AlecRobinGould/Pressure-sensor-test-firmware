#ifndef RS485HANDLER_H
#define RS485HANDLER_H

#pragma once

// #ifndef UNIT_TEST  // Protect hardware access
#include <Arduino.h>
// #else              // Use mocks during testing
// #include "mocks/MockSdFat.h"
// #include "mocks/MockSerial.h"
// #include "mocks/MockWire.h"
// #endif

class RS485Handler {
public:
    RS485Handler(int enablePin, long baudrate);
    bool begin();
    void send(const int data);
    String receive(unsigned long baseTime, unsigned long timeCorrection);

private:
    int _enablePin;
    long _baudrate;
};

#endif
