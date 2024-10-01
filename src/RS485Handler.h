#ifndef RS485HANDLER_H
#define RS485HANDLER_H

#include <Arduino.h>

class RS485Handler {
public:
    RS485Handler(int enablePin);
    void begin(long baudRate);
    void send(const int data);
    String receive();

private:
    int _enablePin;
};

#endif
