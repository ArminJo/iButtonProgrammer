/*
 * iButtonProgrammer.cpp
 *
 * Arduino UNO / Nano iButton / Dallas key reader as well as programmer for writable iButtons of type RW1990
 *
 * The program reads an iButton and writes a constant ID value if receiving "w" over Serial.
 * Reading supports
 *  - (DS1990A), (DS1990R), DS2401, DS2411
 *  - (DS1973), DS2433 | 4Kb EEPROM
 * Writing was successfully tested with RW1990 ones.
 *
 * Based on:
 * https://wolf-u.li/verwendung-eines-ibutton-an-einem-arduino-via-onewire/
 * and
 * https://gist.github.com/swiftgeek/0ccfb7f87918b56b2259
 * and
 * https://github.com/meawoppl/eepromTool-ds2433/blob/master/arduino/onewireProxy/onewireProxy.ino
 *
 *  Copyright (C) 2022  Armin Joachimsmeyer
 *  armin.joachimsmeyer@gmail.com
 *
 *  https://gist.github.com/ArminJo/a8c0ad4034852ef0be7b7a3e4c5d7a0d
 */

/*
 * For iButton family codes see: https://www.maximintegrated.com/en/design/technical-documents/app-notes/1/155.html
 * and https://owfs.sourceforge.net/family.html
 */

#include <Arduino.h>
#include "OneWire.h" // https://github.com/PaulStoffregen/OneWire
#include "HexDump.h" // Include sources

#define IBUTTON_ONE_WIRE_PIN 8
OneWire iButton(IBUTTON_ONE_WIRE_PIN); // iButton connected on PIN 8.

uint8_t sReadIDBuffer[8];
uint8_t sReadMemoryBuffer[1024];

uint8_t sIDToWriteArray[8] = { 0x1, 0x60, 0x60, 0xCF, 0x3, 0x0, 0x0, 0xA8 }; // The button ID to write on receiving a "w" over Serial

void doWriteID(uint8_t *aIdArray);
void printIDArray(uint8_t *aIdArray);
void readMemory(void);

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
        if (sReadIDBuffer[0] == 0x01) {
            Serial.println(F("First byte is 0x01 => Device is an iButton / (DS1990A), (DS1990R), DS2401, DS2411"));
        } else if (sReadIDBuffer[0] == 0x23) {
            Serial.println(F("First byte is 0x23 => Device is an (DS1973), DS2433"));
            Serial.println(F("Read 512 bytes EEPROM"));
            readMemory();
            Serial.println(F("Done."));
            Serial.println();
            printMemoryHexDump(sReadMemoryBuffer, 512);
        }

        if (iButton.crc8(sReadIDBuffer, 7) == sReadIDBuffer[7]) {
            Serial.println(F("CRC is valid"));
            ReadBufferContainsValidID = true;
        } else {
            Serial.println(F("CRC is not valid"));
            ReadBufferContainsValidID = false;
        }
        Serial.println(F("Done."));
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
        doWriteID(sIDToWriteArray);
    } else if (tCharacter == 'c' && ReadBufferContainsValidID) {
        Serial.println();
        Serial.println();
        Serial.println(F("******************************************"));
        if (sReadIDBuffer[0] != 0x01) {
            Serial.println(F("Received a \"c\" over Serial, but first byte is not \"0x01\" as required for an iButton. Cloning refused!"));
        } else {
            Serial.println(F("Received a \"c\" over Serial. Start cloning."));
            doWriteID(sReadIDBuffer);
        }
    }
    delay(1000);
}

void printIDArray(uint8_t *aIdArray) {
    for (uint_fast8_t i = 0; i < 8; i++) {
        printBytePaddedHex(aIdArray[i]);
    }
}

/*
 * Based on
 * https://gist.github.com/swiftgeek/0ccfb7f87918b56b2259
 * Write LSB first
 */
void writeByte(uint8_t data) {
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

/*
 * Read 512 bytes from one wire 0x0000 to sReadMemoryBuffer
 */
void readMemory(void) {
    memset(sReadMemoryBuffer, 0, sizeof(sReadMemoryBuffer));
    if (iButton.reset()) {
        iButton.write(0xCC); // Skip ROM (ID) -> only one device allowed at the bus
        iButton.write(0xF0); // Read memory
        iButton.write(0x00); // memory read start address LSB
        iButton.write(0x00); // memory read start address MSB
        // Read 512 byte / 4 KBit
        for (int i = 0; i < 512; i++) {
            sReadMemoryBuffer[i] = iButton.read();
        }
    }
}

void doWriteID(uint8_t *aIdArray) {
    Serial.print(F("1-Wire device ID to write after 5 seconds is:"));
    printIDArray(aIdArray);
    Serial.println();

    Serial.println(F("Now wait 5 seconds"));
    Serial.println();
    delay(5000);

    uint8_t crc;
    crc = iButton.crc8(aIdArray, 7);
    Serial.print(F("CRC=0x"));
    Serial.println(crc, HEX);

    // The reading of data is necessary for successful programming !!!
    iButton.skip();
    iButton.reset();
    iButton.write(0x33);
    Serial.print(F("ID before write:"));
    for (uint_fast8_t i = 0; i < 8; i++) {
        printBytePaddedHex(iButton.read());
    }
    Serial.println();

    Serial.print(F("Writing iButton ID:"));
    iButton.skip();
    iButton.reset();
    // write ID
    iButton.write(0xD5);
    for (uint_fast8_t i = 0; i < 8; i++) {
        printBytePaddedHex(aIdArray[i]);
        writeByte(aIdArray[i]);
    }
    iButton.reset();
    Serial.println();

    Serial.println(F("Done."));
    Serial.println();
}
