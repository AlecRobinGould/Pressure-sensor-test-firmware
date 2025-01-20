#include "RS485Handler.h"
#include "ErrorHandler.h"
extern ErrorHandler errorHandler; // Declare the external instance

RS485Handler::RS485Handler(int enablePin, long baudrate)
    : _enablePin(enablePin), _baudrate(baudrate) {}

bool RS485Handler::begin() {
    Serial1.begin(_baudrate);
    uint16_t timeout = 0;
    while (!Serial1) {
        timeout++;
        delay(1);
        if (timeout > 3000) {
            errorHandler.setError(RS485_FAIL); // Set error state if RS485 communication fails
            return false;
        }
    }
    if (Serial1){
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, LOW); // Disable RS485 driver
        Serial1.setTimeout(200);  //[ms] default is 1 s

        digitalWrite(_enablePin, HIGH); // Disable RS485 driver
        Serial1.print("PR1");    // Request gauge 1
        Serial1.write(0x0d);     // ENQ command
        Serial1.flush();
        digitalWrite(_enablePin, LOW); // Disable RS485 driver
        return true;
    }
    else{
        return false;
    }
}

void RS485Handler::send(const int data) {
    digitalWrite(_enablePin, HIGH); // Enable RS485 driver
    delay(10);
    Serial1.write(data);
    Serial1.flush();
    digitalWrite(_enablePin, LOW); // Disable RS485 driver
}

String RS485Handler::receive(unsigned long baseTime, unsigned long timeCorrection) {
    digitalWrite(_enablePin, LOW); // Disable RS485 driver
    String receivedData = "";

    unsigned long startTime = millis();
    startTime = startTime - (baseTime + timeCorrection);
    while (Serial1.available() == 0) {
        // Add timeout
        if ((millis() - startTime) >= 1000) {
            Serial.println("RS485 timeout reached");
            break;
        }
    }
    receivedData = Serial1.readString();
    char vacData[11];
    strncpy(vacData, &receivedData[2], sizeof(vacData));
    Serial.println(vacData);
    
    if (receivedData.length() == 0 || receivedData == "") {
        if (errorHandler.isErrorActive(RS485_FAIL)){}
        else{
            errorHandler.setError(RS485_FAIL); // Set error state if no data is received
        }
    }
    else if (errorHandler.isErrorActive(RS485_FAIL)) {
        errorHandler.clearError(RS485_FAIL); // Clear error state if data is sent successfully
    }
    return receivedData;
}
