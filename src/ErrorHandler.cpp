#include "ErrorHandler.h"

// ErrorHandler errorHandler(4, sdHandler);  // Make sure sdHandler is declared before this

ErrorHandler::ErrorHandler(int errorPin, SDHandler& sdHandler)
    : _errorPin(errorPin), _errorState(false), _currentError(NO_ERROR),
      _lastBlinkTime(0), _ledState(false), _sdHandler(sdHandler) {}

void ErrorHandler::begin() {
    pinMode(_errorPin, OUTPUT);
    digitalWrite(_errorPin, LOW);  // Turn off LED initially
}

void ErrorHandler::setError(ErrorType error) {
    _currentError = error;
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

        // Log error to Serial and SD card
        if (Serial) {
            Serial.println(errorMessage);
        }
        
        if (_sdHandler.logData(errorMessage)) {
            digitalWrite(_errorPin, HIGH); // Static LED since SD card logging succeeded
        } else {
            digitalWrite(_errorPin, LOW);  // Set LED to start blinking if log failed
        }
    } else {
        digitalWrite(_errorPin, LOW);  // Turn off LED if no error
    }
}

bool ErrorHandler::hasError() const {
    return _errorState;
}

void ErrorHandler::update() {
    if (_errorState) {
        if (!_sdHandler.logData("") && !Serial) {  // Neither SD card nor Serial is active
            if (millis() - _lastBlinkTime >= 125) { // Blink LED at 4 times a second (125ms on/off)
                _ledState = !_ledState;
                digitalWrite(_errorPin, _ledState ? HIGH : LOW);
                _lastBlinkTime = millis();
            }
        } else {
            digitalWrite(_errorPin, HIGH);  // Static LED when either SD or Serial is active
        }
    }
}
