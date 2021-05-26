/* 
 * Arduino Library for I2C EEPROM
 * @author: Hugo Arganda 
 * @date: 2/12/2021
 */

#include "Arduino.h"
#include <Wire.h>

#define EEPROM_DUMP_EN 0 /* Enable(1)/Disabled(0) EEPROM Dump method, Disabled by default */

class serialEEPROM {
  public:
    /**
     * Class constructor.
     *
     * @param deviceAddress - Device I2C address.
     * @param memSize - Memory size in bytes.
     * @param pageSize - Page size in bytes.
     */
    serialEEPROM(uint8_t deviceAddress, uint16_t memSize, uint8_t pageSize);

    /**
     * Erase full memory (fill with 0xFF).
     */
    void erase(void);
    
    /**
     * Write a single byte to address.
     *
     * @param memaddress - Memory address.
     * @param data - Byte to write.
     */
    void write(uint16_t memaddress, uint8_t data);

    /**
     * Write a buffer to address with defined length.
     *
     * @param memaddress - Memory address.
     * @param src - Buffer to write.
     * @param len - Buffer length.
     */
    void write(uint16_t memaddress, uint8_t* src, int len);

    /**
     * Read a single byte from address.
     *
     * @param memaddress - Memory address.
     * @retval - Byte read.
     */
    uint8_t read(uint16_t memaddress);

    /**
     * Read a buffer from address with defined length.
     *
     * @param memaddress - Memory address.
     * @param dest - Buffer to store data read.
     * @param len - Buffer length.
     */
    void read(uint16_t memaddress, uint8_t* dest, int len);

#if (EEPROM_DUMP_EN == 1)
    /**
     * Dump memory.
     *
     * @param port - HardwareSerial port to print.
     * @param memaddress - Starting address.
     * @param len - Length of data to print in bytes.
     */
    void dump(HardwareSerial &port, uint16_t memaddress, int len);
    void PrintHex8(HardwareSerial &port, uint8_t *data, uint8_t length);
    void PrintHex16(HardwareSerial &port, uint16_t *data, uint8_t length);
#endif

  private:
    uint8_t _address;  /* Device Address */
    uint16_t _memSize; /* Device Memory Size */
    uint8_t _pageSize; /* Device Memory Page Size */
};