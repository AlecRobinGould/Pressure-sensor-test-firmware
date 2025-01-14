#include "ButtonHandler.h"

volatile bool ButtonHandler::logState = false; // Initialize static variable
unsigned long ButtonHandler::lastInterruptTime = 0; // Initialize static variable
int ButtonHandler::buttonPin = 0;

ButtonHandler::ButtonHandler(int pin, unsigned long debounceDelay)
    : _pin(pin), _debounceDelay(debounceDelay), _lastDebounceTime(0),
      _buttonState(HIGH), _lastButtonState(HIGH), _isPressed(false) {buttonPin = pin;}

void ButtonHandler::begin() {
    pinMode(_pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(_pin),toggleLogState, FALLING);
    lastInterruptTime = millis();
}

void ButtonHandler::toggleLogState() {
    unsigned long currentTime = millis();
    int reading = digitalRead(buttonPin);
    while (!reading){reading = digitalRead(buttonPin);}
    // Serial.println("Times:");
    // Serial.println(currentTime);
    // Serial.println("end");
    // Serial.println(lastInterruptTime);
    if (currentTime - lastInterruptTime >= 150 && reading) { // Debounce time of 50 ms
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