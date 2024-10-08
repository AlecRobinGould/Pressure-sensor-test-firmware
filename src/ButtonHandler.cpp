#include "ButtonHandler.h"

volatile bool ButtonHandler::logState = false; // Initialize static variable
unsigned long ButtonHandler::lastInterruptTime = 0; // Initialize static variable

ButtonHandler::ButtonHandler(int pin, unsigned long debounceDelay)
    : _pin(pin), _debounceDelay(debounceDelay), _lastDebounceTime(0),
      _buttonState(HIGH), _lastButtonState(HIGH), _isPressed(false) {}

void ButtonHandler::begin() {
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pin), toggleLogState, FALLING);
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

void ButtonHandler::toggleLogState() {
    unsigned long currentTime = millis();
    if (currentTime - lastInterruptTime > 50) { // Debounce time of 50 ms
        logState = !logState;
        lastInterruptTime = currentTime;
    }
}

void ButtonHandler::checkSerialForButtonPress() {
    if (Serial.available()) {
        char input = Serial.read();
        if (input == 'p') {  // Simulate button press with 'p'
            logState = true;
            Serial.println("Simulated button press from serial.");
        } else if (input == 'r') {  // Simulate button release with 'r'
            logState = false;
            Serial.println("Simulated button release from serial.");
        }
    }
}