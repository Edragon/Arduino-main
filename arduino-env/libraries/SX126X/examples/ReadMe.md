## Examples

The Examples folder contains a series of test programs for the SX1261 and  SX1262. The programs are described below. Its recommended to use the **SX126X_LoRa_Register_Test** program first to test the device connections are correct. 

A common feature of the programs is the uses of a series of definitions in the Settings.h file. These defines determine the frequency used, the bandwidth the spreading factor and coding rate used, for example;


    const uint32_t Frequency = 434400000;     //frequency of transmissions
    const uint32_t Offset = 0;                //offset frequency for calibration purposes  
    const uint8_t Bandwidth = LORA_BW_125;    //LoRa bandwidth
    const uint8_t SpreadingFactor = LORA_SF7; //LoRa spreading factor
    const uint8_t CodeRate = LORA_CR_4_5;     //LoRa coding rate
    const uint8_t Optimisation = LDRO_AUTO;   //low data rate optimisation setting

The full range of possible settings is within the 'SX126XLT_Includes.h' file. A summary of the relevant LoRa modem settings is below.  

<br> 

### LoRa Modem Settings

    //LoRa bandwidths
    #define LORA_BW_500  //203khz
    #define LORA_BW_0400  //406khz
    #define LORA_BW_0800  //812khz
    #define LORA_BW_1600  //1625khz

    //LoRa spreading factors
    #define LORA_BW_500  6      //actual 500000hz
    #define LORA_BW_250  5      //actual 250000hz
    #define LORA_BW_125  4      //actual 125000hz
    #define LORA_BW_062  3      //actual  62500hz 
    #define LORA_BW_041  10     //actual  41670hz
    #define LORA_BW_031  2      //actual  31250hz 
    #define LORA_BW_020  9      //actual  20830hz
    #define LORA_BW_015  1      //actual  15630hz
    #define LORA_BW_010  8      //actual  10420hz 
    #define LORA_BW_007  0      //actual   7810hz

    //LoRa coding rates
    #define LORA_CR_4_5  0x01
    #define LORA_CR_4_6  0x02
    #define LORA_CR_4_7  0x03
    #define LORA_CR_4_8  0x04


<br>
<br>

### GFSK Modem Settings

Not yet tested

<br>
<br>
## Buffers

The data to be sent is loaded into an array called TXBUFFER, the length can be varied to suit, with a maximum of 255 bytes for LoRa packets received by the device are pulled from the FIFO and moved into the array called RXBUFFER.


## Packet Addressing

LoRa is a two way technology, each device is a transceiver. Most often on a particular frequency there will be one transmitter and one receiver. However, this may not always be the case and there could be several nodes in use on the same frequency. 

In order to keep the software simple and allow for the receipt of signals from multiple receivers or directed commands to a particular node, a basic addressing scheme has been implemented and is used by some programs. There are library routines to send and receive packets in addressed and non-addressed format so you choose which to send. When using addressed mode regardless of the data content of the actual payload (in TXBUFFER) each packet sent has 3 control bytes at the beginning of the packet. In general the control bytes have been restricted to ASCII printable characters so that they can be shown directly on a terminal monitor. The 3 bytes are;

**Packet type**. This either describes the content of the packet, which could be a GPS location payload or is a command to do something and there is no payload. Details of the packet types defined are in the library file 'SX126XLT_Program_Definitions.h'

**Packet Destination**. The node number that the packet is destined for.

**Packet Source**. The node number that the packet was sent from.

The destination and source packet bytes mean that node ‘2’ (could be your base station receiver) can send a command that only station ‘3’ will respond to. This command could be a reset command, a request to turn on and off certain transmission types etc. Node ‘3’ can be set-up so that it only accepts commands from a particular node.

In addressed mode the 3 control bytes are automatically stripped from each received packet and the rest of the data is placed in an array called RXBUFFER. 

An example of the 3 control bytes from a tracker would be;

T*2

Which means there is a test packet (T) its been sent as a broadcast (*) and its from node 2.

This simple protocol can be used to queue requests to intermittent receivers. For instance some high altitude balloon tracker software assumes that the in flight tracker does not listen for commands continuously; it only listens for incoming packets for short periods in order to minimise power consumption. At the beginning of a listen period (which may only be a few seconds) the tracker sends a ready (clear to send) packet. The receiver is normally not so power constrained so is left in permanent listen mode. The receiver has been given a queued request (could be a 'release a payload command' for instance) and is waiting for the ready packet from the tracker. When the ready packet is received, the receiver sends the queued command and listens for an acknowledge packet. If the receiver does not receive an acknowledge it will keep queuing the request.

<br>

For the example programs set the serial monitor baud rate to 115200 baud.

**1 SX126XLT LoRa RegisterTest** - Checks to see if a SX126X device can be found. Prints a list of registers read. Use this program first to check the SX126X module is wired up correctly.  

**2 SX126XLT LoRa Simple TX** - Transmits a series of LoRa packets according to the LoRa parameters in the Settings.h file. The LED will flash when a packet is transmitted. Used together with matching RX program. 

**3 SX126XLT LoRa Simple RX** - Receives LoRa packets according to the LoRa parameters in the settings.h file. Results, RSSI, SNR, errors etc are  displayed in the Arduino IDE serial monitor. The LED will flash when a packet is received, you can add and enable a buzzer too. Used together with matching TX program.  

**4 SX126XLT LoRa Link Test TX** - This used for testing the sensitivity of links or antennas etc. The transmitter sends a sequence of packets starting at a specified power (in settings.h) and decreasing the power used to send the packet by 1dBm at a time. The packet contains the ASCII representation of the packet power such a +10 for 10dBm, +01 for 1dBm and -10 for -10dBm. The receiver prints these ASCII values so you can see at what power level the link fails. Use the '3 SX126XLT_LoRa_Simple_RX' program to receive the packets. This program used addressed packets. The principles of this type of link testing are discussed, for 434Mhz LoRa in the document '[Testing and Comparing - December 2018](https://github.com/LoRaTracker/Link-Tester2/blob/master/Testing%20and%20Comparing%20-%20December%202018.pdf)' see the section on 'Test Software – Descending Power Tests' in particular. 

**5 SX126XLT LoRa FrequencyCounter Check TX**

This program generates a LoRa packet that is around 6 seconds long. The program can be used for checking the frequency programmed for the SX126X is as expected and for measuring the power output level.

**6 SX126XLT LoRa RX Frequency Error Check**

This receiver program demonstrates the use of the ability of the SX126X to measure the frequency error, in relation to the receivers frequency, of an incoming packet. Prints the frequency error over an average of 10 packets to the serial monitor. Can be used to ensure that modules are close together in frequency. 

**9 SX126XLT LoRa TX and Sleep**

The SX126X has features that allow the register state of the device to be saved internally before putting the device into low current deep sleep. When the device is woken up, by pulsing the NSS select pin, the device reloads all the internal registers and the device can continue as if it had never been asleep. The feature avoids having to reconfigure the device on wakeup. The program sends a packet 'Hello World1' goes into deep sleep for around 10 seconds, wakes up an attempts to transmit a 'Hello World2'. Packet. If the wakeup is working correctly then the '3 SX126XLT LoRa Simple RX' program should show both packets arriving.  



Enjoy. 
<br>
<br>
### Stuart Robinson
### October 2019


