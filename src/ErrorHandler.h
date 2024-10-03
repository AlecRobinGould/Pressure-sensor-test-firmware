// ErrorHandler.h
#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <Arduino.h>
#include "SDHandler.h"

enum ErrorType {
    SD_INIT_FAIL,
    SD_LOG_FAIL,
    ADC_FAIL,
    RS485_FAIL,
    NO_ERROR
};

class ErrorHandler {
public:
    ErrorHandler(int errorPin, SDHandler& sdHandler);
    void begin();
    void setError(ErrorType error);
    bool hasError() const;
    void update();

private:
    int _errorPin;
    bool _errorState;
    ErrorType _currentError;
    unsigned long _lastBlinkTime;
    bool _ledState;
    SDHandler& _sdHandler;  // Reference to SDHandler
    void logError(const String& errorMessage);
};

#endif
