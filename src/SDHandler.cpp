#include "SDHandler.h"
#include "ErrorHandler.h"
extern ErrorHandler errorHandler; // Declare the external instance

SDHandler::SDHandler(int chipSelect, int logLEDPin)
    : _chipSelect(chipSelect), _logLEDPin(logLEDPin),_currentLogNumber(1) {}

bool SDHandler::begin() {
    if (!SD.begin(_chipSelect, SD_SCK_MHZ(50))) {  // Initialize SD card with higher speed if supported
        return false;
    }
    pinMode(_logLEDPin, OUTPUT);
    digitalWrite(_logLEDPin, HIGH);  // Turn off LED initially

    findNextLogFile();  // Find the next available log file name
    return true;
}

bool SDHandler::logData(const String& data) {
    if (!logFile.open(_logFileName.c_str(), O_APPEND | O_WRITE)) {
        errorHandler.setError(SD_LOG_FAIL); // Set error state if logging fails
        return false;
    }
    else{
        if (errorHandler.isErrorActive(SD_LOG_FAIL)) {
            errorHandler.clearError(SD_LOG_FAIL);  // Clear error state if logging is successful
            errorHandler.clearError(SD_INIT_FAIL);  // Clear SD initialization error if logging is successful
        }
    }
    digitalWrite(_logLEDPin, LOW);  // Turn onLED
    // Serial.print("Filename: ");
    // Serial.println(_logFileName);

    logFile.println(data.c_str());
    logFile.sync(); // Ensure data is written to SD card
    logFile.close();
    digitalWrite(_logLEDPin, HIGH);  // Turn off LED
    return true;
}

void SDHandler::logIncrement() {
    _currentLogNumber++; // Increment the log number
    findNextLogFile();   // Find the next available log file name
}

void SDHandler::findNextLogFile() {
    char fileName[20];
    bool fileExists;

    do {
        snprintf(fileName, sizeof(fileName), "Vac%03d.txt", _currentLogNumber);
        fileExists = SD.exists(fileName);

        if (fileExists) {
            _currentLogNumber++;
        } else {
            _logFileName = String(fileName);  // Update the log file name
        }
    } while (fileExists);
    Serial.println(_logFileName);
    if (logFile.open(_logFileName.c_str(), O_WRITE | O_CREAT | O_APPEND)){
        logFile.close();
    }
}
