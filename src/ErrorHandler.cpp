#include "ErrorHandler.h"

// ErrorHandler errorHandler(4, sdHandler);  // Make sure sdHandler is declared before this

ErrorHandler::ErrorHandler(int errorPin, SDHandler& sdHandler)
    : _errorPin(errorPin), _errorState(false), _currentError(NO_ERROR),
      _lastBlinkTime(0), _ledState(false), _sdHandler(sdHandler) {}

void ErrorHandler::begin() {
    pinMode(_errorPin, OUTPUT);
    digitalWrite(_errorPin, HIGH);  // Turn off LED initially
    for (int i = 0; i < ERROR_COUNT; ++i) {
        errorStates[i] = false;
    }
}

void ErrorHandler::setLED(bool state) {
    digitalWrite(_errorPin, state ? LOW : HIGH);
}

void ErrorHandler::clearError(ErrorType error) {
    if (error < ERROR_COUNT) {
        errorStates[error] = false;
    }
    digitalWrite(_errorPin, HIGH);  // Turn off LED by default
    for (int i = 0; i < ERROR_COUNT; ++i) {
        if (errorStates[i]) {
            digitalWrite(_errorPin, LOW); // Turn on LED if any error is active
            return;
        }
    }
}

void ErrorHandler::clearAllErrors() {
    for(int i = 0; i < ERROR_COUNT; ++i) {
        errorStates[i] = false;
    }
    _errorState = false;
}

bool ErrorHandler::isErrorActive(ErrorType error) const {
    if (error < ERROR_COUNT) {
        return errorStates[error];
    }
    return false;
}

void ErrorHandler::setError(ErrorType error) {
    // Serial.println(error);
    if (error < ERROR_COUNT) {
        errorStates[error] = true;
    }
    _errorState = (error != NO_ERROR);

    if (_errorState) {
        String errorMessage;
        switch (_currentError) {
            case SD_INIT_FAIL: errorMessage = "SD Card Initialization Failed"; break;
            case SD_LOG_FAIL: errorMessage = "SD Card Logging Failed"; break;
            case ADC_FAIL: errorMessage = "ADC Communication Failed"; break;
            case RS485_FAIL: errorMessage = "RS485 Communication Failed"; break;
            default: break;
        }
        setLED(true);
        // if (_currentError == SD_INIT_FAIL || _currentError == SD_LOG_FAIL) {
        //     digitalWrite(_errorPin, LOW);  // Turn on LED if error is SD card related
        // }
        // // else if (_sdHandler.logData(errorMessage)) {
        // //     digitalWrite(_errorPin, LOW); // Static LED since SD card logging succeeded
        // // } 
        // else {
        //     digitalWrite(_errorPin, HIGH);  // Set LED to start blinking if log failed
        // }
    } else {
        setLED(false);
    }
}

bool ErrorHandler::hasError() const {
    return _errorState;
}