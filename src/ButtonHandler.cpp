#include "ButtonHandler.h"

ButtonHandler::ButtonHandler(int pin, unsigned long debounceDelay)
    : _pin(pin), _debounceDelay(debounceDelay), _lastDebounceTime(0),
      _buttonState(HIGH), _lastButtonState(HIGH), _isPressed(false), logState(false) {}

void ButtonHandler::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

void ButtonHandler::update() {
    int reading = digitalRead(_pin);
    if (reading != _lastButtonState) {
        _lastDebounceTime = millis();
    }

    if ((millis() - _lastDebounceTime) > _debounceDelay) {
        if (reading != _buttonState) {
            _buttonState = reading;
            if (_buttonState == LOW) {
                _isPressed = true;
                if(logState){
                    logState = false;
                }
                else{
                    logState = true;
                }
            }
        }
    }

    _lastButtonState = reading;
}

bool ButtonHandler::isPressed() {
    if (_isPressed) {
        _isPressed = false;
        return true;
    }
    return false;
}
