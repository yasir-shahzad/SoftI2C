# SoftI2C Arduino Library
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](https://opensource.org/licenses/MIT)
[![Installation instructions](https://www.ardu-badge.com/badge/SoftI2C.svg?)](https://www.ardu-badge.com/SoftI2C)
[![Commits since latest](https://img.shields.io/github/commits-since/yasir-shahzad/SoftI2C/latest)](https://github.com/yasir-shahzad/SoftI2C/commits/master)
[![Build Status](https://github.com/yasir-shahzad/SoftI2C/workflows/LibraryBuild/badge.svg)](https://github.com/yasir-shahzad/SoftI2C/actions)
![Hit Counter](https://visitor-badge.laobi.icu/badge?page_id=yasir-shahzad_SoftI2C)

The SoftI2C library provides an Arduino-compatible implementation of the I2C communication protocol using software-based methods. It allows you to perform I2C communication without relying on hardware I2C pins, which can be useful in situations where hardware I2C is unavailable or needs to be emulated.

## Features

- Software-based I2C communication.
- Emulates I2C communication over GPIO pins.
- Provides read and write functionality.
- Easy-to-use API for I2C operations.

## Installation

1. Download the library ZIP file from the [GitHub repository](https://github.com/yasir-shahzad/SoftI2C).
2. In the Arduino IDE, go to `Sketch` > `Include Library` > `Add .ZIP Library...`.
3. Select the downloaded ZIP file and click `Open`.

## Usage

1. Include the library at the beginning of your sketch:

   ```cpp
   #include <SoftI2C.h>

   
## Contributing
Contributions to the SoftI2C library are welcome! If you find any issues or have suggestions for improvements, please feel free to submit a pull request or open an issue on the GitHub repository.

## License
This project is licensed under the MIT License.
