#include <SoftI2C.h>

SoftI2C SoftWire =SoftI2C(10, 5); //sda, scl

void setup()
{
  SoftWire.begin();
  
  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}

void loop()
{
  byte error, address;
  int nDevices;
  
  Serial.println(F("Scanning I2C bus (7-bit addresses) ..."));
  
  nDevices = 0;
  for(address = 1; address < 127; address++ )
    {
      // The i2c_scanner uses the return value of
      // the Write.endTransmisstion to see if
      // a device did acknowledge to the address.
      SoftWire.beginTransmission(address);
      error = SoftWire.endTransmission();
      
      if (error == 0)
        {
          Serial.print(F("I2C device found at address 0x"));
          if (address<16)
            Serial.print(F("0"));
          Serial.print(address,HEX);
          Serial.println(F("  !"));
	  
          nDevices++;
        }
      else if (error==4)
        {
          Serial.print(F("Unknow error at address 0x"));
          if (address<16)
            Serial.print("0");
          Serial.println(address,HEX);
        }    
    }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else 
    Serial.println("done\n");
  
  delay(5000);           // wait 5 seconds for next scan
}
