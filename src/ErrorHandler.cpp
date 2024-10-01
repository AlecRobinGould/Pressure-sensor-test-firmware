#include "ErrorHandler.h"

ErrorHandler::ErrorHandler(int errorPin)
    : _errorPin(errorPin), _errorState(false) {}

void ErrorHandler::begin() {
    pinMode(_errorPin, OUTPUT);
    digitalWrite(_errorPin, LOW); // Turn off the error LED initially
}

void ErrorHandler::setError(bool state) {
    _errorState = state;
    digitalWrite(_errorPin, state ? HIGH : LOW); // Turn on or off the LED based on the error state
}

bool ErrorHandler::hasError() const {
    return _errorState;
}
