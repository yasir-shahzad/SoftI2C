
#include "SoftI2C.h"

SoftI2C::SoftI2C(uint8_t dataPin, uint8_t clockPin, bool usePullup /*=false*/) {
  sda = dataPin;
  scl = clockPin;
  pullup = usePullup;
}

void SoftI2C::begin(void) {
  rxBufferIndex = 0;
  rxBufferLength = 0;
  error = 0;
  isTransmitting = false;

  i2cInit();
}

void SoftI2C::end(void) {
}

void SoftI2C::beginTransmission(uint8_t address) {
  if (isTransmitting) {
    error = (i2cRepStart((address<<1)|I2C_WRITE) ? 0 : 2);
  } else {
    error = (i2cStart((address<<1)|I2C_WRITE) ? 0 : 2);
  }
    // indicate that we are isTransmitting
  isTransmitting = 1;
}

void SoftI2C::beginTransmission(int address) {
  beginTransmission((uint8_t)address);
}

uint8_t SoftI2C::endTransmission(uint8_t sendStop)
{
  uint8_t transferError = error;
  if (sendStop) {
    i2cStop();
    isTransmitting = 0;
  }
  error = 0;
  return transferError;
}

uint8_t SoftI2C::endTransmission(void)
{
  return endTransmission(true);
}

size_t SoftI2C::write(uint8_t data) {
  if (i2cWrite(data)) {
    return 1;
  } else {
    if (error == 0) error = 3;
    return 0;
  }
}

size_t SoftI2C::write(const uint8_t *data, size_t quantity) {
  size_t progress = 0;
  for(size_t i = 0; i < quantity; ++i){
    progress += write(data[i]);
  }
  return progress;
}

uint8_t SoftI2C::requestFrom(uint8_t address, uint8_t quantity,
            uint32_t iaddress, uint8_t isize, uint8_t sendStop) {
  error = 0;
  uint8_t localerror = 0;
    if (isize > 0) {
      beginTransmission(address);
      // the maximum size of internal address is 3 bytes
      if (isize > 3){
        isize = 3;
      }
      // write internal register address - most significant byte first
      while (isize-- > 0) {
        write((uint8_t)(iaddress >> (isize*8)));
      }
      endTransmission(false);
    }
    // clamp to buffer length
    if(quantity > I2C_BUFFER_LENGTH){
      quantity = I2C_BUFFER_LENGTH;
    }
    if (isTransmitting) {
      localerror = !i2cRepStart((address<<1) | I2C_READ);
    } else {
      localerror = !i2cStart((address<<1) | I2C_READ);
    }
    if (error == 0 && localerror) error = 2;
    // perform blocking read into buffer
    for (uint8_t count=0; count < quantity; count++) {
      rxBuffer[count] = i2cRead(count == quantity-1);
    }
    // set rx buffer iterator vars
    rxBufferIndex = 0;
    rxBufferLength = error ? 0 : quantity;
    if (sendStop) {
      isTransmitting = 0;
      i2cStop();
    }
    return rxBufferLength;
}

uint8_t SoftI2C::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint32_t)0, (uint8_t)0, (uint8_t)sendStop);
}

uint8_t SoftI2C::requestFrom(int address, int quantity, int sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

uint8_t SoftI2C::requestFrom(uint8_t address, uint8_t quantity) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

uint8_t SoftI2C::requestFrom(int address, int quantity) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)true);
}

int SoftI2C::available(void) {
  return rxBufferLength - rxBufferIndex;
}

int SoftI2C::read(void) {
  int value = -1;
  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  return value;
}

int SoftI2C::peek(void) {
  int value = -1;

  if(rxBufferIndex < rxBufferLength){
    value = rxBuffer[rxBufferIndex];
  }
  return value;
}

void SoftI2C::flush(void) {
}

/**
 * @brief Initializes the SoftI2C module.
 *
 * This function needs to be called once in the beginning to initialize the SoftI2C module.
 * It returns false if either the SDA or SCL lines are low, which may indicate an I2C bus lockup
 * or that the lines are not properly pulled up.
 *
 * @return True if initialization is successful, false otherwise.
 */
bool SoftI2C::i2cInit(void) {
  digitalWrite(sda, LOW);
  digitalWrite(scl, LOW);
  setPinHigh(sda);
  setPinHigh(scl);
  if (digitalRead(sda) == LOW || digitalRead(scl) == LOW) return false;
  return true;
}

/**
 * @brief Start an I2C transfer.
 *
 * This function starts an I2C transfer by pulling the data line (SDA) low followed by the clock line (SCL).
 * The 8-bit I2C address (including the R/W bit) is passed as a parameter.
 *
 * @param addr The 8-bit I2C address to communicate with.
 * @return Returns true if the slave responds with an "acknowledge" signal, false otherwise.
 */
bool SoftI2C::i2cStart(uint8_t addr) {
  setPinLow(sda);
  delayMicroseconds(DELAY);
  setPinLow(scl);
  return i2cWrite(addr);
}

/**
 * @brief Try to start transfer until an ACK is returned.
 *
 * This function attempts to start an I2C transfer to the specified address and
 * waits for an acknowledgment (ACK) to be returned. If the ACK is not received
 * within a certain number of retries, the function gives up and returns false.
 *
 * @param addr The 7-bit I2C address of the target device.
 * @return Returns true if the I2C start and ACK were successful, false otherwise.
 */
bool SoftI2C::i2cStartWait(uint8_t addr) {
  long retry = I2C_MAXWAIT;
  while (!i2cStart(addr)) {
    i2cStop();
    if (--retry == 0) return false;
  }
  return true;
}

/**
 * @brief Repeated start function.
 *
 * After having claimed the bus with a start condition,
 * you can address another or the same chip again without an intervening 
 * stop condition.
 *
 * @param addr The 7-bit address of the slave device.
 *
 * @return True if the slave replies with an "acknowledge", false otherwise.
 */
bool SoftI2C::i2cRepStart(uint8_t addr) {
  setPinHigh(sda);
  setPinHigh(scl);
  delayMicroseconds(DELAY);
  return i2cStart(addr);
}

/**
 * @brief Issue a stop condition, freeing the bus.
 *
 * This function generates a stop condition on the I2C bus,
 * releasing the bus for other devices to use.
 */
void SoftI2C::i2cStop(void) {
  setPinLow(sda);
  delayMicroseconds(DELAY);
  setPinHigh(scl);
  delayMicroseconds(DELAY);
  setPinHigh(sda);
  delayMicroseconds(DELAY);
}

/**
 * @brief Write one byte to the slave chip that had been addressed by the previous start call.
 *
 * @param value The byte to be sent.
 * @return True if the slave replies with an "acknowledge," false otherwise.
 */
bool SoftI2C::i2cWrite(uint8_t value) {
  for (uint8_t curr = 0X80; curr != 0; curr >>= 1) {
    if (curr & value) setPinHigh(sda); else  setPinLow(sda); 
    setPinHigh(scl);
    delayMicroseconds(DELAY);
    setPinLow(scl);
  }

  setPinHigh(sda);
  setPinHigh(scl);
  delayMicroseconds(DELAY/2);
  uint8_t ack = digitalRead(sda);
  setPinLow(scl);
  delayMicroseconds(DELAY/2);  
  setPinLow(sda);
  return ack == 0;
}

/**
 * @brief Read one byte from the I2C bus.
 * 
 * This function reads a single byte from the I2C bus. If the parameter `last` is true,
 * a NAK (Not Acknowledge) signal is sent after receiving the byte to terminate the
 * read sequence.
 * 
 * @param last Set to true if NAK should be sent after the byte read.
 * @return The byte read from the I2C bus.
 */
uint8_t SoftI2C::i2cRead(bool last) {
  uint8_t receivedByte  = 0;
  setPinHigh(sda);
  for (uint8_t i = 0; i < 8; i++) {
    receivedByte <<= 1;
    delayMicroseconds(DELAY);
    setPinHigh(scl);
    if (digitalRead(sda)) receivedByte  |= 1;
    setPinLow(scl);
  }
  if (last) setPinHigh(sda); else setPinLow(sda);
  setPinHigh(scl);
  delayMicroseconds(DELAY/2);
  setPinLow(scl);
  delayMicroseconds(DELAY/2);  
  setPinLow(sda);
  return receivedByte ;
}
/**
 * @brief Set the specified pin to a low state.
 *
 * This function sets the specified pin to a low (logic level 0) state. If pull-up
 * is enabled, it first ensures that the pin is set to a low state and then sets it
 * as an output pin. Interrupts are temporarily disabled during this process.
 *
 * @param pin The pin to be set to a low state.
 */
void SoftI2C::setPinLow(uint8_t pin) {
  noInterrupts();
  if (pullup) 
    digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  interrupts();
}

/**
 * @brief Set the specified pin to a high level and configure its mode.
 * 
 * This function sets the specified pin to a high level and configures its mode
 * according to the pull-up setting.
 * 
 * @param pin The pin number to set high and configure.
 */
void SoftI2C::setPinHigh(uint8_t pin) {
  noInterrupts();
  if (pullup) 
    pinMode(pin, INPUT_PULLUP);
  else
    pinMode(pin, INPUT);
  interrupts();
}
