#include <Arduino.h>
#include <SdFat.h>

// Pin definitions
const int chipSelect = 4;

String filename;

// SdFat object
SdFat SD;
SdFile logFile;
SdFile root;  // Add root to iterate through files

void listFiles() {
    if (!root.open("/")) {  // Open the root directory
        Serial.println("Error opening root directory!");
        return;
    }

    SdFile entry;
    while (entry.openNext(&root, O_READ)) {
        char fileName[13];  // Buffer to hold file names
        entry.getName(fileName, sizeof(fileName));  // Get the file name
        Serial.println(fileName);  // Print the file name
        entry.close();  // Close the file to avoid memory leaks
    }

    root.close();  // Close the root directory when done
}

void setup() {
    filename = "Vac001.txt";
    Serial.begin(9600);
    delay(1000); // Give some time for Serial monitor to initialize
    while(!Serial) {} // wait for com port to open

    // Initialize SD card
    if (!SD.begin(chipSelect, SD_SCK_MHZ(50))) {  // Adjust speed if necessary
        Serial.println("SD card initialization failed!");
        return;
    } else {
        Serial.println("SD card initialized.");
    }

    // List all files on SD card
    Serial.println("Listing all files:");
    listFiles();  // Call the function to list all files

    // Open log file and write data
    if (logFile.open(filename.c_str(), O_WRITE | O_CREAT | O_APPEND)) {
        Serial.println("Writing to SD card...");
        logFile.println("Test data written to SD card.");
        logFile.close();
        Serial.println("Data successfully written to SD card.");
    } else {
        Serial.println("Error opening log file!");
    }

    // Try reading back from the file
    if (logFile.open(filename.c_str(), O_READ)) {
        Serial.println("Reading file:");
        while (logFile.available()) {
            Serial.write(logFile.read());
        }
        logFile.close();
    } else {
        Serial.println("Error reading log file!");
    }
}

void loop() {
    // Do nothing in loop
}
