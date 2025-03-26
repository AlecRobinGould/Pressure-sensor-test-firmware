#ifndef SDHANDLER_H
#define SDHANDLER_H

#pragma once

// #ifndef UNIT_TEST  // Protect hardware access
#include <SdFat.h>
// #else              // Use mocks during testing
// #include "mocks/MockSdFat.h"
// #include "mocks/MockSerial.h"
// #include "mocks/MockWire.h"
// #endif

class SDHandler {
public:
    SDHandler(int chipSelect, int logLEDPin);
    bool begin();
    bool logData(const String& data);
    void logIncrement();  // Function to increment the log file number

    void findNextLogFile();
    String getLogFileName() const { return _logFileName; }
private:
    int _chipSelect;
    int _logLEDPin;
    String _logFileName;
    int _currentLogNumber;
    SdFat SD;  // SdFat object
    SdFile logFile;

    
};

#endif
