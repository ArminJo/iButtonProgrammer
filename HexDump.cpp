/*
 * HexDump.hpp
 * Hex memory dump utility functions for Arduino. 0x00 and 0xFF are printed as spaces.
 *
 * Sample output:
 * 0x0000:  0xF1 0x81 0x82 0x00 0x08 0x02 0x00 0x27 0xFF 0xFF 0x0E 0xB3 0x81 0xFC 0x9B 0x47  ... .. '  .....G
 * 0x0020:  0x00 0x00 0x00 0x00 0x20 0x65 0x00 0x0F 0xBE 0xEB 0x9B 0x98 0x2C 0xF1 0x08 0x2C       e .....,..,
 *
 *  Copyright (C) 2022  Armin Joachimsmeyer
 *  Email: armin.joachimsmeyer@gmail.com
 *
 *  This file is part of Arduino-Utils https://github.com/ArminJo/Arduino-Utils.
 *
 *  ArduinoUtils is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/gpl.html>.
 */

#ifndef _HEX_DUMP_HPP
#define _HEX_DUMP_HPP

#include <Arduino.h>
#define BYTES_PER_LINE  16

/*
 * Print with leading space and padded with 0
 */
void printBytePaddedHex(uint8_t aHexValueToPrint) {
    Serial.print(F(" 0x"));
    if (aHexValueToPrint < 0x10) {
        Serial.print('0');
    }
    Serial.print(aHexValueToPrint, HEX);
}

void printWordPaddedHex(uint16_t aHexValueToPrint) {
    Serial.print(F("0x"));
    if (aHexValueToPrint < 0x1000) {
        Serial.print('0');
    }
    if (aHexValueToPrint < 0x100) {
        Serial.print('0');
    }
    if (aHexValueToPrint < 0x10) {
        Serial.print('0');
    }
    Serial.print(aHexValueToPrint, HEX);

}

void printMemoryHexDump(uint8_t *aMemory, size_t aSizeOfMemoryToPrint) {
    for (uint16_t tIndex = 0; tIndex < aSizeOfMemoryToPrint; tIndex += BYTES_PER_LINE) {
        printWordPaddedHex(tIndex);
        Serial.print(F(": "));
        for (uint_fast8_t i = 0; i < BYTES_PER_LINE; i++) {
            printBytePaddedHex(aMemory[tIndex + i]);
        }
        Serial.print(F("  "));

        for (uint_fast8_t i = 0; i < BYTES_PER_LINE; i++) {
//            if(isalnum(tIndex+i)){ // requires 40 bytes more program space
            if (' ' <= aMemory[tIndex + i] && aMemory[tIndex + i] <= '~') {
                Serial.print((char)aMemory[tIndex + i]);
            } else if (aMemory[tIndex + i] != 0x00 && aMemory[tIndex + i] != 0xFF) {
                // for non printable characters except 0 and FF
                Serial.print('.');
            } else {
                Serial.print(' ');
            }
        }
        tIndex += BYTES_PER_LINE;
        Serial.println();
    }
}

#endif // _HEX_DUMP_HPP
