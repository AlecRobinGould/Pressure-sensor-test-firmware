#ifndef BUTTONHANDLER_H
#define BUTTONHANDLER_H

#include <Arduino.h>

class ButtonHandler {
public:
    ButtonHandler(int pin, unsigned long debounceDelay = 50);
    void begin();
    void update();
    bool isPressed();
    static void toggleLogState(); // ISR function must be static
    void checkSerialForButtonPress();  // Add this method for simulating button press via Serial
    volatile static bool logState;
    static unsigned long lastInterruptTime; // Last interrupt time for debouncing

private:
    int _pin;
    unsigned long _debounceDelay;
    unsigned long _lastDebounceTime;
    bool _buttonState;
    bool _lastButtonState;
    bool _isPressed;
};


#endif
