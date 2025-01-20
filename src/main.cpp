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
const int errorPin = 6;
const int logLEDPin = 5;


unsigned long baseTime = 0;
unsigned long setTime;
unsigned long timeCorrection = 50;
long baudrate = 9600;



// variables
double voltage;
String logDataS, pressure;
char pfeiffer[11];
bool cleaner; //manage log string and names

// Create handler objects
ADCHandler adcHandler(0x48, 0x49);
ButtonHandler buttonHandler(buttonPin);
SDHandler sdHandler(chipSelect, logLEDPin);
RS485Handler rs485Handler(enablePin, baudrate);
ErrorHandler errorHandler(errorPin, sdHandler);

void setup() {
    Serial.begin(9600);
    while (!Serial){
        Serial.println("Serial initialized...");
    }
    errorHandler.begin();
    adcHandler.begin();
    buttonHandler.begin(); // Initializes the button and sets up the interrupt
    if (sdHandler.begin()) {
        Serial.println("SD card initialised.");
    } else {
        Serial.println("SD card initialization failed!");
        errorHandler.setError(SD_INIT_FAIL);
    }

    if (rs485Handler.begin()) {
        Serial.println("RS485 Initialised");
    }else{
        Serial.println("RS485 initialization failed!");
        errorHandler.setError(RS485_FAIL);
    }
    cleaner = false;
}

void loop() {
    
    buttonHandler.checkSerialForButtonPress();  // Check for serial input to simulate button press
    if (ButtonHandler::logState) {
        baseTime = millis();
        // Serial.println("Button pressed. Logging data...");
        for (uint8_t channel = 0; channel < 8; channel++) {
            int16_t adcValue = adcHandler.readChannel(channel);
            if (adcValue <= -1) {
                errorHandler.setError(ADC_FAIL);
            }
            else if (errorHandler.isErrorActive(ADC_FAIL)) {
                errorHandler.clearError(ADC_FAIL);

            }
            voltage = adcValue * adcHandler.refVoltage * adcHandler.invResolution;
            logDataS += String(voltage, 4);
            logDataS += ", ";            
        }
        
        pressure = "";
        rs485Handler.send(0x05);

        // This tracks the time taken to get here
        setTime = (millis() - baseTime) + timeCorrection;

        pressure = rs485Handler.receive(setTime, timeCorrection);
        timeCorrection = millis();
        Serial.print("Pressure: ");
        Serial.println(pressure);
        strncpy(pfeiffer, &pressure[2], sizeof(pfeiffer));
        logDataS += String(pfeiffer);

        if (!sdHandler.logData(logDataS)) {
            // errorHandler.setError(SD_LOG_FAIL);
            while (!sdHandler.begin()){
                Serial.println("Trying to initialize SD card...");
                // delay(200);
                errorHandler.setLED(true);
                delay(300);
                errorHandler.setLED(false);
            }
            if (errorHandler.hasError()) {
                errorHandler.setLED(true);
            }
            else{
                errorHandler.setLED(false);
            }
        } else {
            if (errorHandler.isErrorActive(SD_LOG_FAIL)) {
                errorHandler.clearError(SD_LOG_FAIL);
                errorHandler.clearError(SD_INIT_FAIL);
            }
            Serial.print("logdata: ");
            Serial.println(logDataS);
        }

        cleaner = true;
        logDataS = "";

        // This tracks the time taken to log the data and offsets the timeout delay
        timeCorrection = millis() - timeCorrection;
        Serial.println(timeCorrection);
    }
    if (ButtonHandler::logState == false && cleaner) {
        logDataS = "";
        sdHandler.logIncrement();
        cleaner = false;
    }
    // errorHandler.update(); // Ensure errorHandler's update method is called regularly
}
