#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
    ButtonHandler(int pin, unsigned long debounceDelay = 50);
    void begin();
    void update();
    bool isPressed();
    bool logState;
    static void toggleLogState(); // ISR function must be static
    volatile static bool logState; // Needs to be volatile since it’s used in ISR

private:
    int _pin;
    unsigned long _debounceDelay;
    unsigned long _lastDebounceTime;
    bool _buttonState;
    bool _lastButtonState;
    bool _isPressed;
};

#endif
