#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

#include <Arduino.h>

class ErrorHandler {
public:
    ErrorHandler(int errorPin);
    void begin();
    void setError(bool state);
    bool hasError() const;

private:
    int _errorPin;
    bool _errorState;
};

extern ErrorHandler errorHandler;

#endif
