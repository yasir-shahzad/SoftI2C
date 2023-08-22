/*
  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef _SoftWire_h
#define _SoftWire_h

#include <Arduino.h>
#include <inttypes.h>
#include "Stream.h"

#ifndef I2C_BUFFER_LENGTH
  #define I2C_BUFFER_LENGTH 32
#endif

#define WIRE_HAS_END 1
#define I2C_READ 1
#define I2C_WRITE 0
#define DELAY 4 // usec delay
#define I2C_MAXWAIT 5000

class SoftI2C : public Stream
{
private:
  void setPinHigh(uint8_t pin);
  void setPinLow(uint8_t pin);
  bool i2cInit(void);
  bool i2cStart(uint8_t addr);
  bool i2cStartWait(uint8_t addr);
  bool i2cRepStart(uint8_t addr);
  void i2cStop(void);
  bool i2cWrite(uint8_t value);
  uint8_t i2cRead(bool last);

  uint8_t rxBuffer[I2C_BUFFER_LENGTH];
  uint8_t rxBufferIndex;
  uint8_t rxBufferLength;
  uint8_t isTransmitting;
  uint8_t error;
  uint8_t sda;
  uint8_t scl;
  bool pullup;
public:

  SoftI2C(uint8_t dataPin, uint8_t clockPin, bool usePullup = false);

  void begin(void);
  void end(void);

  void beginTransmission(uint8_t address);
  void beginTransmission(int address);
  uint8_t endTransmission(uint8_t sendStop);
  uint8_t endTransmission(void);

  size_t write(uint8_t data);
  size_t write(const uint8_t *data, size_t quantity);

  uint8_t requestFrom(uint8_t address, uint8_t quantity,
              uint32_t iaddress, uint8_t isize, uint8_t sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop);
  uint8_t requestFrom(int address, int quantity, int sendStop);
  uint8_t requestFrom(uint8_t address, uint8_t quantity);
  uint8_t requestFrom(int address, int quantity);

  int available(void);
  int read(void);
  int peek(void);
  void flush(void);

  inline size_t write(unsigned long n) { return write((uint8_t)n); }
  inline size_t write(long n) { return write((uint8_t)n); }
  inline size_t write(unsigned int n) { return write((uint8_t)n); }
  inline size_t write(int n) { return write((uint8_t)n); }
};
#endif 
#pragma once
