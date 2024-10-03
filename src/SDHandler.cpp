#include "SDHandler.h"
#include "ErrorHandler.h"
extern ErrorHandler errorHandler; // Declare the external instance

SDHandler::SDHandler(int chipSelect)
    : _chipSelect(chipSelect), _currentLogNumber(1) {}

bool SDHandler::begin() {
    if (!SD.begin(_chipSelect)) {
        return false;
    }
    
    findNextLogFile();  // Find the next available log file name
    return true;
}

bool SDHandler::logData(const String& data) {
    File logFile = SD.open(_logFileName, FILE_WRITE);
    if (logFile) {
        logFile.println(data);
        logFile.flush(); // Ensure data is written to SD card
        logFile.close();
        return true;
    }
    errorHandler.setError(SD_LOG_FAIL); // Set error state if logging fails
    return false;
}

void SDHandler::logIncrement() {
    _currentLogNumber++; // Increment the log number
    findNextLogFile();   // Find the next available log file name
}

void SDHandler::findNextLogFile() {
    char fileName[20];
    bool fileExists;

    do {
        snprintf(fileName, sizeof(fileName), "VacLog%04d.txt", _currentLogNumber);
        fileExists = SD.exists(fileName);

        if (fileExists) {
            _currentLogNumber++;
        } else {
            _logFileName = String(fileName);  // Update the log file name
        }
    } while (fileExists);
}
