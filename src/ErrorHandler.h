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
    NO_ERROR,
    ERROR_COUNT
};

class ErrorHandler {
public:
    ErrorHandler(int errorPin, SDHandler& sdHandler);
    void setError(ErrorType error);
    void clearError(ErrorType error);
    void clearAllErrors();
    bool isErrorActive(ErrorType error) const;

    void begin();
    void setLED(bool state);
    bool hasError() const;
    void update();

private:
    bool errorStates[ERROR_COUNT];
    int _errorPin;
    bool _errorState;
    ErrorType _currentError;
    unsigned long _lastBlinkTime;
    bool _ledState;
    SDHandler& _sdHandler;  // Reference to SDHandler
    void logError(const String& errorMessage);
};

#endif
