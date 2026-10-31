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
