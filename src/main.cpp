#include <Arduino.h>
#include "ADCHandler.h"
#include "ButtonHandler.h"
#include "SDHandler.h"
#include "RS485Handler.h"
#include "ErrorHandler.h"

// Pin definitions
const int chipSelect = 4;
const int enablePin = 8;
const int buttonPin = 7;
const int errorPin = 4;



// variables
double voltage;
String logData, pressure;
char pfeiffer[11];
bool cleaner; //manage log string and names

// Create handler objects
ADCHandler adcHandler(0x48, 0x49);
ButtonHandler buttonHandler(buttonPin);
SDHandler sdHandler(chipSelect);
RS485Handler rs485Handler(enablePin);
ErrorHandler errorHandler(errorPin, sdHandler);

void setup() {
    Serial.begin(9600);
    errorHandler.begin();
    adcHandler.begin();
    buttonHandler.begin(); // Initializes the button and sets up the interrupt
    if (sdHandler.begin()) {
        Serial.println("SD card initialized.");
    } else {
        Serial.println("SD card initialization failed!");
        errorHandler.setError(SD_INIT_FAIL);
    }
    rs485Handler.begin(9600);
    cleaner = false;
}

void loop() {
    buttonHandler.checkSerialForButtonPress();  // Check for serial input to simulate button press
    if (ButtonHandler::logState) {
        Serial.println("Button pressed. Logging data...");
        for (uint8_t channel = 0; channel < 8; channel++) {
            int16_t adcValue = adcHandler.readChannel(channel);
            voltage = adcValue * adcHandler.refVoltage * adcHandler.invResolution;
            logData += String(voltage, 4);            
        }

        rs485Handler.send(0x05);
        pressure = rs485Handler.receive();
        strncpy(pfeiffer, &pressure[2], sizeof(pfeiffer));
        logData += String(pfeiffer);

        if (!sdHandler.logData(logData)) {
            errorHandler.setError(SD_LOG_FAIL);
        } else {
            Serial.println(logData);
        }
        delay(450);
        cleaner = true;
    }
    if (ButtonHandler::logState == false && cleaner) {
        logData = "";
        sdHandler.logIncrement();
        cleaner = false;
    }
    errorHandler.update(); // Ensure errorHandler's update method is called regularly
}
