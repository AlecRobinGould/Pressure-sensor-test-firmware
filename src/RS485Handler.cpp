#include "RS485Handler.h"
#include "ErrorHandler.h"
extern ErrorHandler errorHandler; // Declare the external instance

RS485Handler::RS485Handler(int enablePin, long baudrate)
    : _enablePin(enablePin), _baudrate(baudrate) {}

bool RS485Handler::begin() {
    Serial1.begin(_baudrate);
    while (!Serial1) {}
    if (Serial1){
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, LOW); // Disable RS485 driver
        Serial1.setTimeout(20);  //[ms] default is 1 s

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
    Serial.print("sent: ");
    Serial.println(data);
    Serial1.write(data);
    Serial1.flush();
    digitalWrite(_enablePin, LOW); // Disable RS485 driver
}

String RS485Handler::receive() {
    String receivedData = "";
    while (Serial1.available() == 0) {}
    receivedData = Serial1.readString();
    
    if (receivedData.length() == 0) {
        errorHandler.setError(RS485_FAIL); // Set error state if no data is received
    }
    return receivedData;
}
