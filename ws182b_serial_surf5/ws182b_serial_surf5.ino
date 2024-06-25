/*
  Author : Sekolah Robot Indonesia
  website : sekolahrobot.co.id
  Project : SURF5 Wiznet 2024
  Description : receive data serial from surf5 to addressable led ws2812b
*/

#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>

SoftwareSerial mySerial(2, 3); // RX, TX


const int bufferSize = 100; // Define the buffer size
char buffer[bufferSize]; // Buffer to store incoming data
int bufferIndex = 0; // Index for the buffer


#define PINLED 4
#define NUMPIXELS 18
Adafruit_NeoPixel pixels(NUMPIXELS, PINLED, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200); // Initialize hardware serial communication
  mySerial.begin(115200); // Initialize software serial communication
  pinMode(PINLED, OUTPUT);
  Serial.println("cek"); // Print an initial message
}

void loop() {
  // Check if there is any data available to read from software serial
  while (mySerial.available() > 0) {
    char character = mySerial.read(); // Read a byte of data

    // Store the character in the buffer if there is space
    if (bufferIndex < bufferSize - 1) {
      buffer[bufferIndex++] = character;
    }
  }

  // If there is data in the buffer, process it
  if (bufferIndex > 0) {
    buffer[bufferIndex] = '\0'; // Null-terminate the buffer
    Serial.println(buffer); // Print the buffer content to hardware serial

    // Parse the buffer to extract unique values
    parseBuffer(buffer);

    bufferIndex = 0; // Reset the buffer index
    delay(1); // Small delay to prevent flooding the serial monitor
  }
}

void parseBuffer(const char* input) {
  int values[20]; // Array to store unique values
  int valueCount = 0; // Count of unique values
  char* token = strtok((char*)input, "(),[] "); // Tokenize the input string

  while (token != NULL) {
    int value = atoi(token); // Convert token to integer

    // Check if the value is already in the array
    bool exists = false;
    for (int i = 0; i < valueCount; i++) {
      if (values[i] == value) {
        exists = true;
        break;
      }
    }

    // If the value is not in the array, add it
    if (!exists) {
      values[valueCount++] = value;
    }

    token = strtok(NULL, "(),[] "); // Get the next token
  }

  // Print the unique values
  Serial.print("Unique values: ");
  for (int i = 0; i < valueCount; i++) {
    Serial.print(values[i]);
    pixels.setPixelColor(values[i], pixels.Color(0, 150, 0));
    pixels.show(); 
    if (i < valueCount - 1) {
      Serial.print(", ");
    }
  }
  Serial.println();
}
