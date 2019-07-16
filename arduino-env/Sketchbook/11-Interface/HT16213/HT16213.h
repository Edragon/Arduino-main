/**
 * \mainpage Holtek HT1621
 *
 * \author Enrico Formenti
 * \date 31 january 2015
 * \version 1.0
 * \tableofcontents
 * \section sec_intro Introduction
 * The Holtek HT1621 is multi-function LCD driver with a 32x4 memory mapping capability. It has a simple
 * interface with microprocessors (3-wire serial communication) and a built-in tone generator with two
 * frequences (2kHz and 4kHz). It supports both internal oscillator (256kHz) or external (256kHz). With
 * 1/2, 1/3 bias and 1/2, 1/3, 1/4 duty cycle is suitable for many general purpose LCD applications.
 *
 * \section sec_dispmem Display memory
 * The RAM is organized into an array of 32 cells of 4 bits each as illustrated by Figure 1.
 * The content of the RAM is directly mapped to the content of the LCD driver and may be affected by the
 * commands \c READ, \c WRITE, \c READ-MODIFY-WRITE.
 *
 * \image latex HT1621mem.png "Figure 1. Memory structure of the HT1621. Remark that we have restructure the adresses so to be able to take care of a full digit."
 * \image html HT1621mem.png "Figure 1. Memory structure of the HT1621. Remark that we have restructure the adresses so to be able to take care of a full digit."
 *
 * \section sec_osc System oscillator
 * The system clock is used in a variety of tasks included LCD driving clock and tone frequency generation.
 * HT1621 has a built-in RC oscillator (256kHZ) and a crystal oscillator (32768Hz). An external 256KHz
 * oscillator can also be used. The type of oscillator used can be selecting by issuing \c XTAL32K,
 * \c RC256K or \c EXT256K commands.
 *
 * Issuing a \c SYS_DIS command stops the system clock, the LCD bias generator and the time base/WDT.
 * The LCD screen will appear blank.
 *
 * An \c LCD_OFF command will turn off the bias generator but not the system clock. Issuing a
 * \c SYS_DIS command will power down the system reducing power consumption.
 *
 * \warning The \c SYS_DIS command affects the system only when the internal RC 256kHZ or the
 * 32.768kHz crystal clocks are used.
 *
 * \section sec_tone Tone output
 * The HT1621 can provide a tone output at the pins \c BZ and \c \f$\bar{BZ}\f$. There are only two tone
 * frequencies, namely: 2kHz and 4kHz that can be selected using the \c TONE2K or \c TONE4K, respectively.
 * Then, the buzzer (if connected to \c BZ and \c \f$\bar{BZ}\f$) start playing at the
 * \c TONE_ON command. Sound is stopped issuing the \c TONE_OFF command.
 *
 * \section sec_powon Power-on
 * At power-on the system is in \c SYS_DIS state. A generic LCD initialization sequence will consist of the following steps:
 * - System Enable
 * - Oscillator configuration
 * - Bias/com configuration
 * - Bias generator start (LCD_ON)
 *
 * \section sec_history History
 * \subsection subsec_v1_0 Version 1.0
 * This the first public version.
 *
 * \section sec_todo Todo list
 * - Improve the overall documentation.
 * - Optimize delays in both writing and reading functions
 * - Overload \c HT1621::sendCommand() function to send several commands in a row
 * - Test reading functions which use the internal RAM of HT1621 not the simulated RAM.
 */

/**
 * \file HT1621.h
 * \brief A class for dealing with the Holtek HT1621 chip.
 * \author Enrico Formenti
 * \date 31 january 2015
 * \version 1.0
 * \copyright BSD license, check the License page on the blog for more information. All this text must be
 *  included in any redistribution.
 *  <br><br>
 *  See macduino.blogspot.com for more details.
 *
 */


#ifndef _HT1621_h
#define _HT1621_h

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define TAKE_CS()    digitalWrite(_CS_pin, LOW)
#define RELEASE_CS() digitalWrite(_CS_pin, HIGH)


// Uncomment the line below if you can read from the HT1621 directly
// #define __HT1621_READ


class HT1621 {
private:
    uint8_t _CS_pin;
    uint8_t _DATA_pin;
    uint8_t _RW_pin;

protected:
    
#ifndef __HT1621_READ
    /**
     * \var ram[16]
     * This array is used to simulate the HT1621 internal ram whenever the read operations are not possible.
     * \warning Define the label __HT1621 to disable this feature and use standard read procedures.
     */
    uint8_t ram[16];
#endif

public:
    /*!
     * \enum Modes
     * Operating modes for the HT1621.
     */
    enum Modes : uint8_t {
        COMMAND_MODE = 0b10000000, /*!< This is used for sending standard commands. */
        READ_MODE = 0b11000000, /*!< This instructs the HT1621 to prepare for reading the internal RAM. */
        WRITE_MODE = 0b10100000, /*!< This instructs the HT1621 to prepare for writing the internal RAM. */
        READ_MODIFY_WRITE_MODE = 0b10100000, /*!< This instructs the HT1621 to prepare for reading/modifying batch of internal RAM adresses. */
        SPECIAL_MODE = 0b10010000 /*!< This instructs the HT1621 to prepare for executing a special command. */
    };
    
    /*!
     * \enum Commands
     *
     * This is an enum of available commands for the HT1621.
     *
     */

    enum Commands : uint8_t {
        SYS_DIS   = 0b00000000, /*!< System disable. It stops the bias generator and the system oscillator. */
        SYS_EN    = 0b00000010, /*!< System enable. It starts the bias generator and the system oscillator. */
        LCD_OFF   = 0b00000100, /*!< Turn off the bias generator. */
        LCD_ON    = 0b00000110, /*!< Turn on the bias generator. */
        TIMER_DIS = 0b00001000, /*!< Disable time base output. */
        WDT_DIS   = 0b00001010, /*!< Watch-dog timer disable. */
        TIMER_EN  = 0b00001100, /*!< Enable time base output. */
        WDT_EN    = 0b00001110, /*!< Watch-dog timer enable. The timer is reset. */
        CLR_TIMER = 0b00011000, /*!< Clear the contents of the time base generator. */
        CLR_WDT   = 0b00011100, /*!< Clear the contents of the watch-dog stage. */

        TONE_OFF  = 0b00010000, /*!< Stop emitting the tone signal at the tone pin. \sa TONE2K, TONE4K */
        TONE_ON   = 0b00010010, /*!< Start emitting tone signal at the tone pin. Tone frequency is selected using commands TONE2K or TONE4K. \sa TONE2K, TONE4K */
        TONE2K    = 0b11000000, /*!< Output tone is at 2kHz. */
        TONE4K    = 0b10000000, /*!< Output tone is at 4kHz. */
        
        RC256K    = 0b00110000, /*!< System oscillator is the internal RC oscillator at 256kHz. */
        XTAL32K   = 0b00101000, /*!< System oscillator is the crystal oscillator at 32768Hz. */
        EXT256K   = 0b00111000, /*!< System oscillator is an external oscillator at 256kHz. */
        
        //Set bias to 1/2 or 1/3 cycle
        //Set to 2,3 or 4 connected COM lines
        BIAS_HALF_2_COM  = 0b01000000, /*!< Use 1/2 bias and 2 commons. */
        BIAS_HALF_3_COM  = 0b01001000, /*!< Use 1/2 bias and 3 commons. */
        BIAS_HALF_4_COM  = 0b01010000, /*!< Use 1/2 bias and 4 commons. */
        BIAS_THIRD_2_COM = 0b01000010, /*!< Use 1/3 bias and 2 commons. */
        BIAS_THIRD_3_COM = 0b01001010, /*!< Use 1/3 bias and 3 commons. */
        BIAS_THIRD_4_COM = 0b01010010, /*!< Use 1/3 bias and 4 commons. */
        
        IRQ_EN    = 0b00010000, /*!< Enables IRQ output. This needs to be excuted in SPECIAL_MODE. */
        IRQ_DIS   = 0b00010000, /*!< Disables IRQ output. This needs to be excuted in SPECIAL_MODE. */
        
        // WDT configuration commands
        F1 = 0b01000000, /*!< Time base/WDT clock. Output = 1Hz. Time-out = 4s. This needs to be excuted in SPECIAL_MODE. */
        F2 = 0b01000010, /*!< Time base/WDT clock. Output = 2Hz. Time-out = 2s. This needs to be excuted in SPECIAL_MODE. */
        F4 = 0b01000100, /*!< Time base/WDT clock. Output = 4Hz. Time-out = 1s. This needs to be excuted in SPECIAL_MODE. */
        F8 = 0b01000110, /*!< Time base/WDT clock. Output = 8Hz. Time-out = .5s. This needs to be excuted in SPECIAL_MODE. */
        F16 = 0b01001000, /*!< Time base/WDT clock. Output = 16Hz. Time-out = .25s. This needs to be excuted in SPECIAL_MODE. */
        F32 = 0b01001010, /*!< Time base/WDT clock. Output = 32Hz. Time-out = .125s. This needs to be excuted in SPECIAL_MODE. */
        F64 = 0b01001100, /*!< Time base/WDT clock. Output = 64Hz. Time-out = .0625s. This needs to be excuted in SPECIAL_MODE. */
        F128 = 0b01001110, /*!< Time base/WDT clock. Output = 128Hz. Time-out = .03125s. This needs to be excuted in SPECIAL_MODE. */
        
        //Don't use
        TEST_ON   = 0b11000000, /*!< Don't use! Only for manifacturers. This needs SPECIAL_MODE. */
        TEST_OFF  = 0b11000110  /*!< Don't use! Only for manifacturers. This needs SPECIAL_MODE. */
    };
    
    /**
     * \fn HT1621(uint8_t CSpin, uint8_t RWpin, uint8_t DATApin)
     * \brief Constructor. Use begin() to complete the initialization of the chip.
     * @param \c CSpin Channel select pin.
     * @param \c RWpin Read/Write signal pin
     * @param \c DATApin Data pin both for reading or writing data.
     */
    HT1621(uint8_t CSpin, uint8_t RWpin, uint8_t DATApin) : _CS_pin(CSpin), _DATA_pin(DATApin), _RW_pin(RWpin) {};
    /**
     *  \fn void begin(void)
     *  \brief Init the HT1621. It inits the control bus. Moreover, it clears the (simulated) ram if \c __HT1621_READ is defined.
     */
    void begin(void);
    /**
     * \fn void writeBits(uint8_t data, uint8_t cnt)
     * \brief Send bits to the HT1621.
     * @param data Data to be sent to the HT1621 seen as an array of bits.
     * @param cnt Number of bits to send to the HT1621.
     * \warning In order to allow series of data to be sent CS is not taken by this function, so several writes can be issued in a row. You need to control CS by your own.
     * \warning There is no check on the size of cnt. Hence avoid ask to send more than 7 bits per time.
     * \sa readBits()
     */
    void writeBits(uint8_t data, uint8_t cnt);
    /**
     * \fn uint8_t readBits(uint8_t cnt)
     * \brief Reads bits from the HT1621.
     * @param cnt Number of bits to read. Maximal number of bits that can be read is 8.
     * \return uint8_t A byte containing the bits read. Last bit is the leftmost one.
     * \warning There is no check if too much bits are read. 0nly the last batch of 8 will be sent back.
     * \sa writeBits()
     */
    uint8_t readBits(uint8_t cnt);
    /**
     * \fn void sendCommand(uint8_t cmd, bool first = true, bool last = true)
     * \brief Sends a command to the HT1621.
     * @param cmd Id of the command to send.
     * @param first If true CS is taken.
     * @param last  If true CS is relased.
     * \warning There is no check on the command id.
     */
    void sendCommand(uint8_t cmd, bool first = true, bool last = true);
    /** 
     * \fn void write(uint8_t address, uint8_t data)
     * \brief Write \c data at the given address.
     * @param address Address to which write the data. Max address is 128.
     * @param data Data to be written. Remark that only the 4 less significative bits are used.
     * \warning There is no check to verify if the address is valid.
     */
    void write(uint8_t address, uint8_t data);
    /** 
     * \fn void write(uint8_t address, uint8_t data, uint8_t cnt)
     * \brief Write \c data at the given address and at the \c cnt successive addresses.
     * @param address Address to which write the data. Max address is 128.
     * @param data Data to be written. Remark that only the 4 less significative bits are used.
     * @param cnt Number of times that \c data has to be written
     * \warning There is no check to verify if the address is valid. Moreover, pay attention that 
     * the address \c (address+cnt) has also to be valid.
     */
    void write(uint8_t address, uint8_t data, uint8_t cnt);
    /**
     * \fn void write(uint8_t address, uint8_t *data, uint8_t cnt)
     * \brief Write \c cnt bytes starting at \c address and take data from buffer \c data.
     * @param address Address to which start writing data. Max address is 128.
     * @param data Buffer to be written.
     * @param cnt Length of the buffer.
     * \warning The buffer is byte aligned, so it is not very efficient. Indeed, only the 4 less significant
     * bits are written.
     * \warning There is no check that the buffer is of suitable length.
     */
    void write(uint8_t address, uint8_t *data, uint8_t cnt);
    /** 
     * \fn read(uint8_t address)
     * \brief Read memory content at address \c address
     * @param address Memory address to read from.
     * \return uint8_t A byte contained the date read.
     * \warning There is no check to verify if the address is valid.
     */
    uint8_t read(uint8_t address);
    /**
     * \fn void read(uint8_t address, uint8_t *data, uint8_t cnt)
     * \brief Read \c cnt bytes starting from \c address into buffer \c data.
     * @param address Memory address to read from.
     * @param *data Buffer in which to store data read.
     * @param cnt Number of bytes to read.
     * \warning There is no check to verify if the address is valid.
     * \warning There is no check that the buffer is of suitable length.
     */
    void read(uint8_t address, uint8_t *data, uint8_t cnt);
};

#endif
