# SX126x - Semtech UHF LoRa Transceivers

##**24/10/19**
<br>

This is a a repository for my Arduino library for the Semtech SX1261 and SX1262 LoRa devices. 


**The Semtech SX1261 and SX1262 are 3.3V logic level device, do not use with 5V logic level Arduinos.** The programs have only been tested on 3.3V 8Mhz ATMega328P and ATMega1284P processors. 

For all example programs you will need to define the following pins as a minimum for the LoRa module in the Settings.h file; NSS, RFBUSY, NRESET, DIO1. 

Most programs use a LED as an indicator and this is defined as pin LED1. Some programs will also turn on a buzzer as an indicator this is connected to pin BUZZER. DIO2 and DIO3 are currently not used directly by the library although they are used internally in the modules. Unused pins in the Settings.h file should be defined as -1.
 
The SX126x operates in the sub-GHz UHF bands, 150 MHz to 960 MHz. In addition to having a LoRa modem the SX126x can send GFSK packets. This library only supports LoRa. 

The SX1262 module from NiceRF uses a TCXO instead of a plain crystal and this makes operation at the narrowest bandwidth of 7.8Khz possible, see here for some details of testing; [SX1262 - improved LoRa device](https://www.loratracker.uk/sx1262-improved-lora-device/).  The NiceRF SX1262 also implements a switching DCDC converter to power the internals of the LoRa receiver, this cuts receive current to about half of the value of SX127X based devices. The SX1261 can also implement the DCDC converter for the transmit side, which promises to significantly improve battery life. As yet there are no readily available small SX1261 modules.

Because of the different ways the SX1261 and SX1262 handle transmissions it is necessary to define which part is being used. This can be done by a specific call when setting the PA config function;

SX126XLT.setPaConfig(0x04, HPMAXAUTO, DEVICE_SX1262);

or in the all in one setup function, for example;

SX126XLT.setupLoRaTX(Frequency, Offset, SpreadingFactor, Bandwidth, CodeRate, Optimisation, DEVICE_SX1262);

The library is in its first revision, there are still some issues to attend to and changes to be made, see the section 'Changes Required to Library' at the bottom of this document. 

Please do not expect basic level support on how to connect the SX126x devices to a particular Arduino or other device, I just don't have the time to do this. The examples do work, so if for you they do not, assume there is a problem with your Arduino setup or how you have wired the modules or that they are faulty.

If the example programs are working, but you consider the reception distance is poor at maximum power (22dBm!) there is no magic voodoo configuration that will make reception 'much better'. Reception distance can vary tremendously depending on environment, what range you get with good line of sight can be 1000 (or more) times the reception distances in urban areas.


### Program examples

The \examples folder of the library contains basic transmitter and receiver programs. There is a link test transmitter programs as well, these programs allows the performance of a link to be measured.

### SX126x connections  

The SX126x operates an SPI based interface. The SX126x will need pin connections for NSS (select) NRESET (reset) RFBUSY (busy) and one of the interrupt out pins, DIO1 is used in the examples. The SPI connections for the SPI interface, SCK, MOSI and MISO are needed also. Most of the testing and evaluation of the SX126x was carried out using Mikrobus compatible boards. Their are breadboard friendly boards for using the modules to be found here; 


### Library installation

To install the library select the 'Clone or download' button on the main Github page, then select 'Download Zip'. In the Arduino IDE select 'Sketch' then 'Include Library'. Next select 'Add .ZIP library' and browse to and select the ZIP file you downloaded, it's called 'SX126x-master.zip'.

### Compatibility

Tested on 3.3V 8Mhz ATMega328P and ATMega1284P only. 


<br>


### Changes Required to Library

  Investigate use of clearDeviceErrors()
  Check recovery from busy timeout error.
  Check rxEnable and txenable are working.
  Add packet implicit mode support - Added readPacketLoRaImplicit and examples
  Review compatibility with SX127x devices.
<br>
<br>


### Stuart Robinson
### August 2019