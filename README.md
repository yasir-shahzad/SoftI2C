# SoftI2C — Arduino Software I2C over GPIO

[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Commits since latest](https://img.shields.io/github/commits-since/yasir-shahzad/SoftI2C/latest)](https://github.com/yasir-shahzad/SoftI2C/commits/master)
[![GitHub issues](https://img.shields.io/github/issues/yasir-shahzad/SoftI2C.svg)](https://github.com/yasir-shahzad/SoftI2C/issues)

**SoftI2C** is an Arduino-compatible software I2C (bit-banged I2C) library that uses GPIO pins selected by the sketch. It provides I2C-style write/read operations through a class that implements Arduino's `Stream` interface.

## Identify this library

- Library name: **SoftI2C**
- Maintainer: **Yasir Shahzad**
- Repository: <https://github.com/yasir-shahzad/SoftI2C>
- Header: `#include <SoftI2C.h>`

This is the `SoftI2C` project at the repository URL above, not the separately named `SoftI2CMaster` project. Check the repository URL and header when choosing between similarly named libraries.

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

The public API takes **7-bit I2C addresses** (for example, `0x48`); do not shift the address or add the read/write bit yourself. This example writes a register pointer and then reads two bytes using a repeated start:

```cpp
#include <SoftI2C.h>

const uint8_t SDA_PIN = 10;
const uint8_t SCL_PIN = 5;
const uint8_t DEVICE_ADDRESS = 0x48;
SoftI2C i2c(SDA_PIN, SCL_PIN); // SDA, SCL; external pull-ups assumed

void setup() {
  Serial.begin(9600);
  i2c.begin();
}

void loop() {
  i2c.beginTransmission(DEVICE_ADDRESS);
  i2c.write(0x00); // Replace with the register required by your device.

  uint8_t status = i2c.endTransmission(false);
  if (status == 0) { // Keep the bus for a repeated start.
    uint8_t count = i2c.requestFrom(DEVICE_ADDRESS, 2);
    if (count == 2 && i2c.available() >= 2) {
      int first = i2c.read();
      int second = i2c.read();
      Serial.println(first);
      Serial.println(second);
    }
  } else {
    i2c.endTransmission(); // Send a stop after a failed register-pointer write.
  }

  delay(1000);
}
```

The example assumes the addressed device supports that register-pointer/read sequence; consult its data sheet and adjust the address, register, and byte count. The library's address scan sketch is [`SoftI2CScan.ino`](examples/SoftI2CScan/SoftI2CScan.ino).

## Contributing and license

Report issues or propose changes in the [GitHub repository](https://github.com/yasir-shahzad/SoftI2C). This project is licensed under the [MIT License](LICENSE).
