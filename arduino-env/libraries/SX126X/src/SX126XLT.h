#ifndef SX126XLT_h
#define SX126XLT_h

#include "Arduino.h"
#include <SX126XLT_Includes.h>

/**************************************************************************

  ToDO
  
  DONE - Tidy Order of routines
  DONE - Move spiInit out of class instance constructor
  DONE - Change GetFreqInt() to getFreqInt()
  DONE - Check for packet reception overwriting TXbuffer and RXbuffer
  DONE - Check for packet reception overwriting TXbuffer and RXbuffer
  DONE - Trim register defines to only those used 
  DONE - Check SX126XLT.readPacketAddressed(RXBUFFER, RXBUFFER_SIZE) is not reading uneccssary data - RXpacketL
  DONE - Check SX126XLT.readPacketAddressed(RXBUFFER, RXBUFFER_SIZE) for packet length 256
  REJECTED - Check for passing TXbuffer and RX buffer size allows for 256 - Cannot have packet size of 256
  DONE - Is _SavedLoRaBandwidth needed ? - No
  DONE - Check for bool sendPacket - Returns status of send
  DONE - Change set frequency to offset varient
  DONE - Change readPacketAddressed to readPacketAddressedLoRa
  DONE - SendPacketAddressedLoRa remove startmS, timemS
  DONE - Change sendBufferLoRa to sendFIFOLoRa
  DONE - Change function of setSleep() to setSleep(uint8_t config);
  DONE - Why is setSleep(uint8_t config) commented out - Corrected added appropriate setting defines
  DONE - sendPacketAddressedLoRa/FLRC and sendPacketLoRa/FLRC - is it necessary to trap zero length packets ? - Yes
  DONE - Does printASCIIPacket(uint8_t *buff, uint8_t tsize) need size--; - No
  DONE - printASCIIPacket( printing one byte too many ? - No prints null character present at end of buffer
  DONE - Check for packet reception overwriting TXbuffer and RXbuffer ends
  DONE - Remove definitions for TXBUFFER_SIZE, RXBUFFER_SIZE
  REJECTED - Remove need for readPacketReceptionLoRa() - cannot as packet reception flagged outside of library
  DONE - Check for uint32_t txtimeout - all changed from unit16_t to uint32_t
  DONE - Put  clearIrqStatus(IRQ_RADIO_ALL) in setTX() and SetRX()
  DONE - Save settings to allow for a SX126X_Config recovery from busy timeout error
  DONE - Check that checkBusy() only used in functions where there is SPI access
  DONE - Use LoRa_Frequency_and_Power_Check_TX program to check output frequency
  DONE - Use LoRa_Frequency_and_Power_Check_TX program to check +\- frequency offsets
  DONE - Check power limits correct,  -17dBm to +14dBm for SX1261, -9dBm to +22dBm for SX1262
  DONE - Add routines for auto calculation of low data rate optimisation setting
  DONE - Check frequency error output, needs calculation same as SX1280 ?
  DONE - Change all example program settings to BW125000, SF7, 434400000
  DONE - Check setSleep() - Total board sleep current is 7uA. 
  DONE - Add SX126XLT.setHighSensitivity(); to RX setup.
  DONE - Investigate use of calibrateDevice(uint8_t devices) - Add to setup routines
  DONE - Review compatibility with SX127x devices. SX127x TX may have issues with short preamble
  
  Investigate use of clearDeviceErrors()
  Check recovery from busy timeout error.
  Check rxEnable and txenable are working.
  For FIFO TX & RX Check writeUint8 readUint8 works with characters
  For FIFO TX & RX Check bytes sent for writeInt16 vs writeUInt16
  For FIFO TX & RX Check bytes sent for writeInt32 vs writeUInt32
  Add packet implicit mode support - Added readPacketLoRaImplicit and examples
  Review compatibility with SX127x devices.
  
**************************************************************************/

class SX126XClass  {
  public:

    SX126XClass();

    bool begin(int8_t pinNSS, int8_t pinNRESET, int8_t pinRFBUSY, int8_t pinDIO1, int8_t pinDIO2, int8_t pinDIO3);
    void pinInit(int8_t _NSS, int8_t _NRESET, int8_t _RFBUSY, int8_t _DIO1, int8_t _DIO2, int8_t _DIO3);
    void spiInit(uint8_t MSBOrder, uint8_t ClockDiv, uint8_t Mode);
    void resetDevice();
    void setStandby(uint8_t StdbyConfig);
    void checkBusy();
    void setSleep(uint8_t sleepconfig);
    void setRegulatorMode(uint8_t mode);
    bool checkDevice();
    bool config();
    uint8_t readsavedRegulatorMode();
    void setDIO2AsRfSwitchCtrl();
    void setDIO3AsTCXOCtrl(uint8_t tcxoVoltage);
    void clearDeviceErrors();
    void calibrateDevice(uint8_t devices);
    void printDeviceErrors();
    void wake();
    void calibrateImage(uint32_t freq);

    void setupLoRaRX(uint32_t frequency, int32_t offset, uint8_t modParam1, uint8_t modParam2, uint8_t  modParam3, uint8_t  modParam4, uint8_t device);
    void setupLoRaTX(uint32_t frequency, int32_t offset, uint8_t modParam1, uint8_t modParam2, uint8_t  modParam3, uint8_t  modParam4, uint8_t device);

    uint8_t getOperatingMode();

    void writeCommand(uint8_t Opcode, uint8_t *buffer, uint16_t size );
    void readCommand( uint8_t Opcode, uint8_t *buffer, uint16_t size );
    void writeRegisters( uint16_t address, uint8_t *buffer, uint16_t size );
    void writeRegister( uint16_t address, uint8_t value );
    void readRegisters( uint16_t address, uint8_t *buffer, uint16_t size );
    uint8_t readRegister( uint16_t address );
    void printRegisters(uint16_t Start, uint16_t End);
    void setPacketParams(uint16_t packetParam1, uint8_t  packetParam2, uint8_t packetParam3, uint8_t packetParam4, uint8_t packetParam5);
    void setModulationParams(uint8_t modParam1, uint8_t modParam2, uint8_t  modParam3, uint8_t  modParam4);
    void printSavedModulationParams();


    void setRfFrequency( uint32_t frequency, int32_t offset );
    uint32_t getFreqInt();
    int32_t getFrequencyErrorRegValue();
    int32_t getFrequencyErrorHz();
    void setTxParams(int8_t TXpower, uint8_t RampTime);
    void setTx(uint32_t timeout);
    bool readTXDone();
    void setRx(uint32_t timeout);
    void setLowPowerRX();
    void setHighSensitivity();
    bool readRXDone();
    void readRXBufferStatus();
    uint8_t readRXPacketL();
    uint8_t readPacketRSSI();
    uint8_t readPacketSNR();
    void setPaConfig(uint8_t dutycycle, uint8_t hpMax, uint8_t device);
    void setRx2(uint32_t timeout);
    void setRxDutyCycle( uint32_t rxTime, uint32_t sleepTime );

    uint8_t readRXBufferPointer();
    uint32_t readsavedFrequency();
    uint32_t readsavedOffset();


    void setBufferBaseAddress(uint8_t txBaseAddress, uint8_t rxBaseAddress);
    void setPacketType(uint8_t PacketType);
    uint8_t readsavedPacketType();


    void clearIrqStatus( uint16_t irq );
    uint16_t readIrqStatus();
    void setDioIrqParams(uint16_t irqMask, uint16_t dio1Mask, uint16_t dio2Mask, uint16_t dio3Mask );
    void printIrqStatus();

    bool readPacketCRCError();
    bool readPacketHeaderValid();
    bool readPacketHeaderError();
    uint8_t readsavedPower();

    void printASCIIorHEX(uint8_t temp);
    void printHEXByte(uint8_t temp);
    void printHEXByte0x(uint8_t temp);
    bool packetOK(uint16_t mask);
    void printASCIIPacket(uint8_t *buff, uint8_t tsize);
    void printHEXPacket(uint8_t *buff, uint8_t tsize);

    uint8_t readsavedModParam1();
    uint8_t readsavedModParam2();
    uint8_t readsavedModParam3();
    uint8_t readsavedModParam4();


    //*******************************************************************************
    //Packet addressing routines
    //*******************************************************************************

    uint8_t readRXPacketType();
    uint8_t readRXDestination();
    uint8_t readRXSource();
    void printAddressInfo();

    //*******************************************************************************
    //LoRa specific Routines
    //*******************************************************************************

    bool sendPacketAddressedLoRa(uint8_t *txbuffer, uint8_t size, char txpackettype, char txdestination, char txsource, uint32_t txtimeout, int8_t txpower, uint8_t _DIO);
    bool sendPacketLoRa(uint8_t *txbuffer, uint8_t size, uint32_t txtimeout, int8_t txPower, uint8_t _DIO);
    uint8_t readPacketLoRa(uint8_t *rxbuffer, uint8_t size);
    void readPacketReceptionLoRa();
    void printReceptionInfoLoRa();
    uint32_t getLoRaBandwidth();
    uint8_t returnSF(uint8_t data);
    uint32_t returnbandwidth(uint8_t BWregvalue);
    bool sendFIFOLoRa(int32_t txtimeout, int8_t txpower, uint8_t _DIO);
    uint8_t readPacketAddressedLoRa(uint8_t *rxbuffer, uint8_t size);
    uint8_t getOptimisation(uint8_t SpreadingFactor, uint8_t Bandwidth);
    float calcSymbolTime(float Bandwidth, uint8_t SpreadingFactor);
    void printLoraSettings();
    void setSyncWord(uint16_t syncword);

    //*******************************************************************************
    //Read Write Buffer commands
    //*******************************************************************************

    void startWriteFIFO();
    void endWriteFIFO();
    void startReadFIFO();
    void endReadFIFO();

    void writeUint8(uint8_t x);
    uint8_t readUint8();

    void writeInt8(int8_t x);
    int8_t readInt8();

    void writeInt16(int16_t x);
    int16_t readInt16();

    void writeUint16(uint16_t x);
    uint16_t readUint16();

    void writeInt32(int32_t x);
    int32_t readInt32();

    void writeUint32(uint32_t x);
    uint32_t readUint32();

    void writeFloat(float x);
    float readFloat();

    //*******************************************************************************
    //RXTX Switch routines - Not yet tested 22/10/19
    //*******************************************************************************  

    void rxtxInit(int8_t pinRXEN, int8_t pinTXEN);
    void rxEnable();
    void txEnable();

    private:

    int8_t _NSS, _NRESET, _RFBUSY, _DIO1, _DIO2, _DIO3;
    int8_t _RXEN, _TXEN;
    uint8_t _RXPacketL;             //length of packet received
    uint8_t _RXPacketType;          //type number of received packet
    uint8_t _RXDestination;         //destination address of received packet
    uint8_t _RXSource;              //source address of received packet
    int8_t  _PacketRSSI;            //RSSI of received packet
    int8_t  _PacketSNR;             //signal to noise ratio of received packet
    int8_t  _TXPacketL;
    uint8_t _RXcount;               //used to keep track of the bytes read from SX126X buffer during readFloat() etc
    uint8_t _TXcount;               //used to keep track of the bytes written to SX126X buffer during writeFloat() etc
    uint8_t _RXBufferPointer;       //pointer to first byte of packet in buffer
    uint8_t _OperatingMode;         //current operating mode
    bool _rxtxpinmode = false;      //set to true if RX and TX pin mode is used.

    //config variables 36 bytes, allows for device to be reset and reconfigured via confg();
    uint32_t savedFrequency;
    uint32_t savedOffset;
    uint8_t  savedPacketType;
    uint8_t  savedRegulatorMode;


    uint8_t  savedModParam1, savedModParam2, savedModParam3, savedModParam4;
    uint16_t savedPacketParam1;
    uint8_t  savedPacketParam2, savedPacketParam3, savedPacketParam4, savedPacketParam5;
    uint16_t savedIrqMask, savedDio1Mask, savedDio2Mask, savedDio3Mask;
    int8_t   savedTXPower;

};
#endif




