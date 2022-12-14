# [iButtonProgrammer](https://github.com/ArminJo/iButtonProgrammer)
### Version 1.0.0
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://github.com/ArminJo/iButtonProgrammer/workflows/TestCompile/badge.svg)](https://github.com/ArminJo/iButtonProgrammer/actions)
![Hit Counter](https://visitor-badge.laobi.icu/badge?page_id=ArminJo_iButtonProgrammer)


The program reads an **[iButton / Dallas key](https://en.wikipedia.org/wiki/1-Wire)** ID.<br/>
If receiving "w" over Serial, it writes a constant value.<br/>
If receiving "c" over Serial, it clones the ID previously read from another iButton.<br/>
Writing was successfully tested with RW1990 ones.

# Sample serial monitor output
```
START ../src/iButtonProgrammer.cpp from Oct 19 2022

Connect inner part of your iButton to pin 8
Do not forget to connect pin 8 with 4.7 kOhm to VCC (5 volt)
Now searching every second for a connected iButton
Write  0x01 0x60 0x60 0xCF 0x03 0x00 0x00 0xA8 on receiving a "w" over Serial
Clone ID of last button read on receiving a "c" over Serial


1-Wire device detected, ID is: 0x1 0x60 0x6C 0xBF 0x3 0x0 0x0 0x21
First byte is 0x01 => Device is an iButton / (DS1990A), (DS1990R), DS2401, DS2411
CRC is valid
Done.


******************************************
Received a "w" over Serial. Start writing.
1-Wire device ID to write after 5 seconds is: 0x01 0x60 0x60 0xCF 0x03 0x00 0x00 0xA8
Now wait 5 seconds

CRC=0x6
ID before write: 0x01 0x60 0x6C 0xBF 0x03 0x00 0x00 0x21
Writing iButton ID: 0x01 0x60 0x60 0xCF 0x03 0x00 0x00 0xA8
Done.

1-Wire device detected, ID is: 0x01 0x60 0x60 0xCF 0x03 0x00 0x00 0xA8
First byte is 0x01 => Device is an iButton / (DS1990A), (DS1990R), DS2401, DS2411
CRC is valid
Done.

1-Wire device detected, ID is: 0x23 0xD4 0x53 0x57 0x01 0x00 0x00 0x9E
First byte is 0x23 => Device is an (DS1973), DS2433
Read 512 bytes EEPROM
Done.

0x0000:  0xFF 0x81 0x82 0x00 0x08 0x02 0x00 0x27 0xFF 0xFF 0x0E 0xB3 0x81 0x3A 0xC9 0x13  .. .. '  ...:..
0x0020:  0x00 0x00 0x00 0x00 0x6A 0xA4 0x00 0x0F 0xBE 0x24 0x0D 0xFF 0x05 0x7A 0x76 0x2C     j. ..$. .zv,
0x0040:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x0060:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x0080:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x00A0:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x00C0:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x00E0:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x0100:  0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 0x55 UUUUUUUUUUUUUUUU
0x0120:  0x51 0x93 0x3E 0x1E 0xC5 0x2C 0x62 0x7A 0x77 0xA0 0x02 0xAE 0x00 0xBD 0xC8 0x35 Q.>..,bzw... ..5
0x0140:  0xF4 0x75 0x72 0x82 0x02 0x30 0x5C 0xCE 0x10 0xD7 0x8E 0x27 0xA4 0xB5 0xEC 0x7C .ur..0\....'...|
0x0160:  0xAE 0x7B 0xBE 0x36 0xCE 0xE3 0x85 0x29 0x4B 0x55 0x4E 0x6C 0x5C 0xDB 0x49 0x48 .{.6...)KUNl\.IH
0x0180:  0x62 0x0C 0x8B 0xCB 0x8C 0x7A 0x6E 0x45 0xA9 0xF7 0x9D 0xC9 0xB3 0x06 0x49 0x0B b....znE......I.
0x01A0:  0xAB 0x1D 0xA1 0x76 0xAA 0x1B 0x1F 0xC5 0xC8 0x47 0xDD 0x2C 0x87 0xC8 0x6D 0x37 ...v.....G.,..m7
0x01C0:  0x75 0xBD 0x7D 0xE0 0x89 0x71 0x03 0xA8 0xA8 0x96 0x48 0x7A 0x66 0x9C 0x01 0x7A u.}..q....Hzf..z
0x01E0:  0x2B 0xF1 0xFA 0xB3 0x9C 0xB6 0xD9 0x63 0x5C 0xD2 0xDB 0x6D 0x3D 0x78 0x61 0xBC +......c\..m=xa.
CRC is valid
Done.


```

#### If you find this program useful, please give it a star.
