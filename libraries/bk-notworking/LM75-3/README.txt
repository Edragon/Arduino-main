****************************************************************************************************
* This is an Arduino library for the LM75A temperature sensor                                      *
****************************************************************************************************

This library was written to enable remote sensing of the temperature of batteries and motor 
controllers used in remotely piloted aircraft, for the purpose of real time data logging and 
air to ground telemetry.

This sensor uses the I2C bus protocol to communicate allowing the Arduino standard Wire library 
to communicate with the device. 2 pins are required to interface the device to an Arduino - 
the SDA and SCL lines.

Written by John Fitter, Eagle Air Australia p/l.

BSD license, all text above must be included in any redistribution

Download the distribution package and decompress it. Rename the uncompressed folder LM75. 
Check that the LM75 folder contains the following files;

LM75.cpp
LM75.h
LM75.chm
property.h

Place the LM75 library folder your arduino sketchfolder/libraries/ folder. You may need to create 
the libraries subfolder if its your first library.
Restart the IDE.

LM75.chm contains the documentation for the class.
