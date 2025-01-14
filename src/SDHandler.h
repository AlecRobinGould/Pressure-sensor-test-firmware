#ifndef SDHANDLER_H
#define SDHANDLER_H

#include <SdFat.h>

class SDHandler {
public:
    SDHandler(int chipSelect, int logLEDPin);
    bool begin();
    bool logData(const String& data);
    void logIncrement();  // Function to increment the log file number

private:
    int _chipSelect;
    int _logLEDPin;
    String _logFileName;
    int _currentLogNumber;
    SdFat SD;  // SdFat object
    SdFile logFile;

    void findNextLogFile();
};

#endif
