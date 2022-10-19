/*
 * iButtonProgrammer.cpp
 *
 * Arduino UNO / Nano iButton / Dallas key reader as well as programmer for writable iButtons of type RW1990
 *
 * The program reads an iButton and writes a constant value if receiving "w" over Serial.
 * Writing was successfully tested with RW1990 ones.
 *
 * Based on:
 * https://wolf-u.li/verwendung-eines-ibutton-an-einem-arduino-via-onewire/
 * and
 * https://gist.github.com/swiftgeek/0ccfb7f87918b56b2259
 *
 *  Copyright (C) 2022  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  https://gist.github.com/ArminJo/a8c0ad4034852ef0be7b7a3e4c5d7a0d
 */

#include <Arduino.h>
#include "OneWire.h" // https://github.com/PaulStoffregen/OneWire

#define IBUTTON_ONE_WIRE_PIN 8
OneWire iButton(IBUTTON_ONE_WIRE_PIN); // iButton connected on PIN 8.

byte sReadIDBuffer[8];

byte sIDToWriteArray[8] = { 0x23, 0x92, 0x30, 0x57, 0x01, 0x00, 0x00, 0x06 }; // The button ID to write on receiving a "w" over Serial
//byte sIDToWriteArray[8] = { 0x23, 0xD4, 0x53, 0x57, 0x01, 0x00, 0x00, 0x9E };

void writeButton(byte *aIdArray);
void printIDArray(byte *aIdArray);

/*
 * Helper macro for getting a macro definition as string
 */
#if !defined(STR_HELPER)
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#endif

void setup() {
    Serial.begin(115200);
    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__));
    Serial.println();
    Serial.println(F("Connect inner part of your iButton to pin " STR(IBUTTON_ONE_WIRE_PIN)));
    Serial.println(F("Do not forget to connect pin " STR(IBUTTON_ONE_WIRE_PIN) " with 4.7 kOhm to VCC (5 volt)"));
    Serial.println(F("Now searching every second for a connected iButton"));
    Serial.print(F("Write "));
    printIDArray(sIDToWriteArray);
    Serial.println(F(" on receiving a \"w\" over Serial"));
    Serial.println(F("Clone ID of last button read on receiving a \"c\" over Serial"));
    Serial.println();
    /*
     * Better safe than sorry - check CRC of data to write
     */
    uint8_t tCRC = iButton.crc8(sIDToWriteArray, 7);
    if (tCRC != sIDToWriteArray[7]) {
        Serial.print(F("CRC 0x"));
        Serial.print(sIDToWriteArray[7], HEX);
        Serial.print(F(" of sIDToWriteArray is not valid, it must be 0x"));
        Serial.println(tCRC, HEX);
    }

    Serial.println();
    delay(250);
}

bool ReadBufferContainsValidID = false;

void loop() {

    iButton.reset_search();

    // Search for an iButton and assign the value to the sReadIDBuffer if found.
    if (iButton.search(sReadIDBuffer)) {
        // At this point an iButton is found
        Serial.print(F("1-Wire device detected, ID is:"));
        printIDArray(sReadIDBuffer);
        Serial.println();

        // Check if this is a iButton
        if (sReadIDBuffer[0] != 0x01) {
            Serial.println(F("First byte is not 0x01 => Device is not an iButton"));
        } else {
            Serial.println(F("First byte is 0x01 => Device is an iButton"));
        }

        if (iButton.crc8(sReadIDBuffer, 7) == sReadIDBuffer[7]) {
            Serial.println(F("CRC is valid"));
            ReadBufferContainsValidID = true;
        } else {
            Serial.println(F("CRC is not valid"));
            ReadBufferContainsValidID = false;
        }
        Serial.println("Done.");
        Serial.println();
    }

    /*
     * Check for write or clone
     */
    char tCharacter = Serial.read();
    if (tCharacter == 'w') {
        Serial.println();
        Serial.println(F("******************************************"));
        Serial.println(F("Received a \"w\" over Serial. Start writing."));
        writeButton(sIDToWriteArray);
    } else if (tCharacter == 'c' && ReadBufferContainsValidID) {
        Serial.println();
        Serial.println();
        Serial.println(F("******************************************"));
        Serial.println(F("Received a \"c\" over Serial. Start cloning."));
        writeButton(sReadIDBuffer);
    }
    delay(1000);
}

void printIDArray(byte *aIdArray) {
    for (int x = 0; x < 8; x++) {
        Serial.print(" 0x");
        Serial.print(aIdArray[x], HEX);
    }
}

/*
 * Based on
 * https://gist.github.com/swiftgeek/0ccfb7f87918b56b2259
 * Write LSB first
 */
void writeByte(byte data) {
    int data_bit;
    for (data_bit = 0; data_bit < 8; data_bit++) {
        if (data & 1) {
            digitalWrite(IBUTTON_ONE_WIRE_PIN, LOW);
            pinMode(IBUTTON_ONE_WIRE_PIN, OUTPUT);  // level low
            delayMicroseconds(60);
            pinMode(IBUTTON_ONE_WIRE_PIN, INPUT);  // level up
            delay(10);
        } else {
            digitalWrite(IBUTTON_ONE_WIRE_PIN, LOW);
            pinMode(IBUTTON_ONE_WIRE_PIN, OUTPUT);  // level low
            pinMode(IBUTTON_ONE_WIRE_PIN, INPUT);  // level high
            delay(10);
        }
        data = data >> 1;
    }
}

void writeButton(byte *aIdArray) {
    Serial.print(F("1-Wire device ID to write after 5 seconds is:"));
    printIDArray(aIdArray);
    Serial.println();

    Serial.println(F("Now wait 5 seconds"));
    Serial.println();
    delay(5000);

    byte crc;
    crc = iButton.crc8(aIdArray, 7);
    Serial.print(F("CRC=0x"));
    Serial.println(crc, HEX);

    // The reading of data is necessary for successful programming !!!
    iButton.skip();
    iButton.reset();
    iButton.write(0x33);
    Serial.print(F("ID before write:"));
    for (byte x = 0; x < 8; x++) {
        Serial.print(' ');
        Serial.print(iButton.read(), HEX);
    }
    Serial.println();

    Serial.print(F("Writing iButton ID:"));
    iButton.skip();
    iButton.reset();
    // write ID
    iButton.write(0xD5);
    for (byte x = 0; x < 8; x++) {
        Serial.print(" 0x");
        Serial.print(aIdArray[x], HEX);
        writeByte(aIdArray[x]);
    }
    iButton.reset();
    Serial.println();

    Serial.println("Done.");
    Serial.println();
}
