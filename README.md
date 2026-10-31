# SoftI2C — Arduino Software I2C over GPIO

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Commits since latest](https://img.shields.io/github/commits-since/yasir-shahzad/SoftI2C/latest)](https://github.com/yasir-shahzad/SoftI2C/commits/master)
[![GitHub issues](https://img.shields.io/github/issues/yasir-shahzad/SoftI2C.svg)](https://github.com/yasir-shahzad/SoftI2C/issues)
[Hit Counter](https://visitor-badge.laobi.icu/badge?page_id=yasir-shahzad_SoftI2C)

**SoftI2C** is an Arduino-compatible software I2C (bit-banged I2C) library that uses GPIO pins selected by the sketch. It provides I2C-style write/read operations through a class that implements Arduino's `Stream` interface.

## Identify this library

- Library name: **SoftI2C**
- Maintainer: **Yasir Shahzad**
- Repository: <https://github.com/yasir-shahzad/SoftI2C>
- Header: `#include <SoftI2C.h>`

To use this library, install it from the repository linked above and include `SoftI2C.h` in your sketch.

Search terms: Arduino software I2C, I2C over GPIO, bit-banged I2C, software I2C library, Arduino `Stream`.

## Features

- Generate I2C communication using the SDA and SCL GPIO pins supplied to `SoftI2C`.
- Use `beginTransmission()`, `write()`, `endTransmission()`, and `requestFrom()` for transfers.
- Read received bytes through the `Stream` methods `available()`, `read()`, and `peek()`.
- Limit each receive request to the library's `I2C_BUFFER_LENGTH` (32 bytes by default).

## Install

1. Open the [SoftI2C GitHub repository](https://github.com/yasir-shahzad/SoftI2C) and choose **Code > Download ZIP**.
2. In Arduino IDE, choose **Sketch > Include Library > Add .ZIP Library...** and select the downloaded ZIP.
3. Include the library in your sketch with `#include <SoftI2C.h>`.

## Wiring and pull-ups

Connect the sketch's selected SDA and SCL pins to the matching lines on the I2C device, and connect the grounds. I2C lines need pull-up resistors to a logic voltage supported by every device on the bus. With the default constructor setting (`usePullup = false`), this library releases the GPIO lines as inputs without enabling internal pull-ups, so provide suitable external pull-ups. Passing `true` enables the board's internal GPIO pull-ups; use external pull-ups appropriate for the bus when needed, and check the board and device voltage limits.

## Basic use

The public API takes **7-bit I2C addresses**; do not shift an address or add the read/write bit yourself. This example scans the standard 7-bit address range for devices that acknowledge:

```cpp
#include <SoftI2C.h>

// Software I2C pins
#define I2C_SDA 10
#define I2C_SCL 5

SoftI2C SoftWire(I2C_SDA, I2C_SCL);

void setup()
{
    Serial.begin(9600);
    SoftWire.begin();

    Serial.println();
    Serial.println("Software I2C Scanner");
    Serial.println("--------------------");
}

void loop()
{
    uint8_t error;
    uint8_t deviceCount = 0;

    Serial.println();
    Serial.println("Scanning I2C bus...");

    for (uint8_t address = 0x08; address <= 0x77; address++)
    {
        SoftWire.beginTransmission(address);
        error = SoftWire.endTransmission();

        if (error == 0)
        {
            Serial.print("Device found at 0x");

            if (address < 0x10)
                Serial.print("0");

            Serial.println(address, HEX);
            deviceCount++;
        }
        else if (error == 4)
        {
            Serial.print("I2C error at 0x");
            Serial.println(address, HEX);
        }
    }

    Serial.println();

    if (deviceCount == 0)
    {
        Serial.println("No I2C devices found.");
    }
    else
    {
        Serial.print("Total devices found: ");
        Serial.println(deviceCount);
    }

    delay(5000);
}
```

This scanner sketch is also available as [`SoftI2CScan.ino`](examples/SoftI2CScan/SoftI2CScan.ino).

## Contributing and license

Report issues or propose changes in the [GitHub repository](https://github.com/yasir-shahzad/SoftI2C). This project is licensed under the [MIT License](LICENSE).
