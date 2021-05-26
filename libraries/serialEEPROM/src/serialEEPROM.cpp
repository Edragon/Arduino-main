#include <serialEEPROM.h>

#define EEPROM_BUFF_LEN 30 /* Arduino Wire Buffer is 32 bytes, 2 are used for address */

/* Constructor for serialEEPROM class */
serialEEPROM::serialEEPROM(uint8_t deviceAddress, uint16_t memSize, uint8_t pageSize) : 
  _address(deviceAddress),
  _memSize(memSize),
  _pageSize(pageSize)
{
  
}

void serialEEPROM::erase(void)
{
  uint8_t data[EEPROM_BUFF_LEN];
  int i = 0;
  
  for(i = 0; i < sizeof(data); i++)
  {
    data[i] = 0xFF;
  }

  for(i = 0; i < (_memSize/sizeof(data)); i++)
  {
    write((uint16_t)(sizeof(data)*i), (uint8_t*)data, sizeof(data));
  }
}

void serialEEPROM::write(uint16_t memaddress, uint8_t data)
{
  write(memaddress, &data, 1);
}

void serialEEPROM::write(uint16_t memaddress, uint8_t* src, int len)
{
  /* Memory protection, don't write beyond memory limits */
  if((memaddress + len) >= _memSize)
  {
    len = (_memSize - memaddress);
  }

#if 1
  uint8_t i;

  for(i = 0; i < len; i++)
  {
    Wire.beginTransmission(_address);
    Wire.write((int)((memaddress >> 8) & 0xFF));   // MSB
    Wire.write((int)(memaddress & 0xFF)); // LSB
    Wire.write(*src++);
    Wire.endTransmission();
    delay(5);

    memaddress++;
  }
  
#else
  int index = 0;          /* Sent bytes */
  uint8_t pageSpace  = 0; /* Available page space */
  uint8_t pageOffset = 0; /* Current page offset */
  uint8_t packLength = 0; /* Length of package to send */
  uint16_t mAddress = 0;  /* Address to write */


  for(index = 0; index < len; index += packLength)
  {
    mAddress = (memaddress + index);
    /* Check available page space */
    pageOffset = (uint8_t)(mAddress % _pageSize);
    pageSpace = (_pageSize - pageOffset); 

    if(pageSpace <= EEPROM_BUFF_LEN) 
    {
      /* If Page has less or equal space than we want to send */
      packLength = pageSpace;
    }
    else 
    {
      /* Else, send the max allowed amount of data */
      packLength = EEPROM_BUFF_LEN;
    }
    
    /* Use only needed space, check for pending data to send */
    if((len - index) < packLength) 
    {
      packLength = (uint8_t)(len - index);
    }

    Wire.beginTransmission(_address);
    if(0xFF < _memSize)
    {
      Wire.write((uint8_t)(mAddress >> 8)); /* ADDR MSB */
    }
    Wire.write((uint8_t)(mAddress & 0xFF)); /* ADDR LSB */
    for (i = 0; i < packLength; i++)
    {
      Wire.write(*src++);
    }
    Wire.endTransmission();

    if(packLength > 1)
    {
      /* Delay 6 ms to ensure Page write */
      delay(6);
    }
    else
    {
      /* Delay ~3.5 ms to ensure byte write */
      delay(5);
    }
  }
#endif


}

uint8_t serialEEPROM::read(uint16_t memaddress)
{
  uint8_t rdata = 0xFF;
  read(memaddress, &rdata, 1);
  return rdata;
}

void serialEEPROM::read(uint16_t memaddress, uint8_t* dest, int len)
{
  int c;

  /* Read protection, don't read beyond memory limits */
  if((memaddress + len) >= _memSize)
  {
    len = (_memSize - memaddress);
  }

  Wire.beginTransmission(_address);
  if(0xFF < _memSize)
  {
    Wire.write((uint8_t)(memaddress >> 8));   // MSB
  }
  Wire.write((uint8_t)(memaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom((int)_address, len);

  for (c = 0; c < len; c++)
  {
    if (Wire.available())
    {
      *dest++ = Wire.read();
    }
  }
}


#if (EEPROM_DUMP_EN == 1)
void serialEEPROM::dump(HardwareSerial &port, uint16_t memaddress, int len)
{
  uint8_t i;

  uint8_t data[EEPROM_BUFF_LEN] = {0};
  
  int index = 0;          /* Sent bytes */
  uint8_t pageSpace  = 0; /* Available page space */
  uint8_t pageOffset = 0; /* Current page offset */
  uint8_t packLength = 0; /* Length of package to send */
  uint16_t addr = 0;      /* Address to print */

  /* Memory protection, don't write beyond memory limits */
  if((memaddress + len) >= _memSize)
  {
    len = (_memSize - memaddress);
  }

  for(index = 0; index < len; index += packLength)
  {
    /* Print memory address */
    addr = (memaddress + index);
    PrintHex16(port, &addr, 1);
    port.print(": ");
    
    /* Check available page space */
    pageOffset = (uint8_t)(addr % _pageSize);
    pageSpace = (_pageSize - pageOffset); 

    if(pageSpace <= EEPROM_BUFF_LEN) 
    {
      /* If Page has less or equal space than we want to send */
      packLength = pageSpace;
    }
    else 
    {
      /* Else, send the max allowed amount of data */
      packLength = EEPROM_BUFF_LEN;
    }
    
    /* Use only needed space, check for pending data to send */
    if((len - index) < packLength) 
    {
      packLength = (uint8_t)(len - index);
    }

    read(addr, (uint8_t*)data, sizeof(data));

    PrintHex8(port, (uint8_t*)data, packLength);

    port.print("| ");

    for (i = 0; i < packLength; i++)
    {
      if(isprint(data[i]))
      {
        port.write(data[i]);
      }
      else
      {
        port.write('=');
      }
    }

    port.println();
  }
}

void serialEEPROM::PrintHex8(HardwareSerial &port, uint8_t *data, uint8_t length) // prints 8-bit data in hex with leading zeroes
{
  for (int i = 0; i < length; i++) 
  { 
    // ort.print("0x"); 
    if (data[i] < 0x10) 
    {
      port.print("0");
    } 
    port.print(data[i],HEX); 
    port.print(" "); 
  }
}

void serialEEPROM::PrintHex16(HardwareSerial &port, uint16_t *data, uint8_t length) // prints 16-bit data in hex with leading zeroes
{
  for (int i = 0; i < length; i++)
  { 
    // port.print("0x"); 
    uint8_t MSB = byte(data[i] >> 8);
    uint8_t LSB = byte(data[i]);
    if (MSB < 0x10) 
    {
      port.print("0");
    } 
    port.print(MSB,HEX); 
    if (LSB < 0x10)
    {
      port.print("0");
    }
    port.print(LSB,HEX); 
    port.print(" "); 
  }
}
#endif