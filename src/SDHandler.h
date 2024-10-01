#ifndef SDHANDLER_H
#define SDHANDLER_H

#include <SD.h>

class SDHandler {
public:
    SDHandler(int chipSelect);
    bool begin();
    bool logData(const String& data);
    void logIncrement();  // Function to increment the log file number

private:
    int _chipSelect;
    String _logFileName;
    int _currentLogNumber;

    void findNextLogFile();
};

#endif
