# [iButtonProgrammer](https://github.com/ArminJo/iButtonProgrammer)
### Version 1.0.0
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![Build Status](https://github.com/ArminJo/iButtonProgrammer/workflows/TestCompile/badge.svg)](https://github.com/ArminJo/iButtonProgrammer/actions)
![Hit Counter](https://visitor-badge.laobi.icu/badge?page_id=ArminJo_iButtonProgrammer)


The program reads an **[iButton / Dallas key](https://en.wikipedia.org/wiki/1-Wire)** and writes a constant value if receiving "w" over Serial.<br/>
Writing was successfully tested with RW1990 ones.

# Sample serial monitor output
```
START ../src/iButtonProgrammer.cpp from Oct 19 2022

Connect inner part of your iButton to pin 8
Do not forget to connect pin 8 with 4.7 kOhm to VCC (5 volt)
Now searching every second for a connected iButton
Write  0x23 0x92 0x30 0x57 0x1 0x0 0x0 0x6 on receiving a "w" over Serial
Clone ID of last button read on receiving a "c" over Serial


1-Wire device detected, ID is: 0x1 0x60 0x6C 0xBF 0x3 0x0 0x0 0x21
First byte is 0x01 => Device is an iButton
CRC is valid
Done.

1-Wire device detected, ID is: 0x1 0x60 0x6C 0xBF 0x3 0x0 0x0 0x21
First byte is 0x01 => Device is an iButton
CRC is valid
Done.


******************************************
Received a "w" over Serial. Start writing.
1-Wire device ID to write after 5 seconds is: 0x23 0x92 0x30 0x57 0x1 0x0 0x0 0x6
Now wait 5 seconds

CRC=0x6
ID before write: 1 60 6C BF 3 0 0 21
Writing iButton ID: 0x23 0x92 0x30 0x57 0x1 0x0 0x0 0x6
Done.

1-Wire device detected, ID is: 0x23 0x92 0x30 0x57 0x1 0x0 0x0 0x6
Device is not an iButton
CRC is valid
Done.

1-Wire device detected, ID is: 0x23 0xD4 0x53 0x57 0x1 0x0 0x0 0x9E
First byte is not 0x01 => Device is not an iButton
CRC is valid
Done.



******************************************
Received a "c" over Serial. Start cloning.
1-Wire device ID to write after 5 seconds is: 0x23 0xD4 0x53 0x57 0x1 0x0 0x0 0x9E
Now wait 5 seconds
Done.

CRC=0x9E
ID before write: 1 60 6C BF 3 0 0 21
Writing iButton ID: 0x23 0xD4 0x53 0x57 0x1 0x0 0x0 0x9E
Done.

1-Wire device detected, ID is: 0x23 0xD4 0x53 0x57 0x1 0x0 0x0 0x9E
First byte is not 0x01 => Device is not an iButton
CRC is valid
Done.

```

#### If you find this program useful, please give it a star.
