/***********************************************************************************************//**
 *  \brief      LM75 Family Device Driver Library - CPP Source file
 *  \details    Based on the LM75 Family Data Sheet 3901090614 Rev 004 09jun2008.
 *  \li         The current implementation does not manage PWM (only digital data by I2C).
 *  \li         Sleep mode is not implemented yet.
 *
 *  \note       THIS IS ONLY A PARTIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING
 *              ACTIVE DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP 
 *              THIS IN MIND IF YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.
 *
 *  \file       LM75.CPP
 *  \author     J. F. Fitter <jfitter@eagleairaust.com.au>
 *  \version    1.0
 *  \date       jan2016
 *  \copyright  Copyright (c) 2016-2017 John Fitter.  All right reserved.
 *
 *  \par License
 *              GNU Public License. Permission is hereby granted, free of charge, to any
 *              person obtaining a copy of this software and associated documentation files
 *              (the "Software"), to deal in the Software without restriction, including
 *              without limitation the rights to use, copy, modify, merge, publish, distribute,
 *              sublicense, and/or sell copies of the Software, and to permit persons to whom
 *              the Software is furnished to do so, subject to the following conditions:
 *  \par
 *              The above copyright notice and this permission notice shall be included in
 *              all copies or substantial portions of the Software.
 *  \par
 *              THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *              IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *              FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *              AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *              LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *              OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *              THE SOFTWARE.
 *
 **************************************************************************************************/

#include "LM75.h"

/**************************************************************************************************/
/*  LM75 Device class functions.                                                                  */
/**************************************************************************************************/

/**
 *  \brief               LM75 Device class constructor.
 *  \param [in] i2caddr  Device address (default: published value).
 */
LM75::LM75(uint8_t i2caddr) {

    busAddr.Set_Class(this);
    busAddr.Set_Get(&LM75::getAddr);

    _addr  = i2caddr;
    _ready = false;
}

/**
 *  \brief  Initialize the device and the i2c interface.
 */
boolean LM75::begin(void) {

    return _ready = true;
}

/**
 *  \brief             Return the measured temperature in specified units.
 *  \remarks           Temperature is returned as a 2's complement value in the most significant
 *                     11 bits of a 16 bit field expressed in degrees C to a resolution of 0.125C.
 *  \param [in] tunit  Temperature units to convert raw data to.
 *  \return            Temperature.
 */
double LM75::readTemp(tempUnit_t tunit) {
    double temp;

    temp = read16(LM75_TEMP) * 0.125/32.0;
    switch(tunit) {
        case    LM75_TK : return convCtoK(temp);
        case    LM75_TF : return convCtoF(temp);
        default : return temp;
    }
}

/**
 *  \brief             Return the specified setpoint in specified units.
 *  \remarks           Setpoint is returned as a 2's complement value in the most significant
 *                     9 bits of a 16 bit field expressed in degrees C to a resolution of 0.125C.
 *  \param [in] tunit  Temperature units to convert raw data to.
 *  \param [in] spt    Setpoint type (overtemp or hysteresis)
 *  \return            Temperature.
 */
double LM75::readSetPoint(tempUnit_t tunit, setPointType spt) { 
    double val;
    
    val = read16(spt == SPT_OVERTEMP ? LM75_TOS : LM75_THYST) * 0.125/128.0;
    switch(tunit) {
        case    LM75_TK : return convCtoK(val);
        case    LM75_TF : return convCtoF(val);
        default : return val;
    }
}

/**
 *  \brief             Write the specified setpoint supplied in the specified units.
 *  \remarks           Setpoint is written as a 2's complement value in the most significant
 *                     9 bits of a 16 bit field expressed in degrees C to a resolution of 0.125C.
 *  \param [in] val    Setpoint temperature.
 *  \param [in] tunit  Temperature units of the setpoint.
 *  \param [in] spt    Setpoint type (overtemp or hysteresis)
 */
void LM75::writeSetPoint(double val, tempUnit_t tunit, setPointType spt) {
    uint16_t data;

    switch(tunit) {
        case    LM75_TK : val = convKtoC(val); break;
        case    LM75_TF : val = convFtoC(val);
    }
    data = (uint16_t)(val * 128.0/0.125 + 0.5);
    write16(spt == SPT_OVERTEMP ? LM75_TOS : LM75_THYST, data);
}

/**
 *  \brief             Set the OS polarity.
 *  \param [in] pol    Enumerated OS polarity selection.
 */
void LM75::setOutputPolarity(outputPolarity pol) {

    write8(LM75_CONF, read8(LM75_CONF) & 
        (pol == POL_LOW ? LM75_CONF_OSPOL_AL : LM75_CONF_OSPOL_AH));
}

/**
 *  \brief             Set the OS operation mode.
 *  \param [in] mode   Enumerated OS operation mode.
 */
void LM75::setOutputMode(outputMode mode) {

    write8(LM75_CONF, read8(LM75_CONF) & 
        (mode == MODE_COMP ? LM75_CONF_OSOM_COMP : LM75_CONF_OSOM_INT));
}

/**
 *  \brief             Set the device operation mode.
 *  \param [in] mode   Enumerated operation mode.
 */
void LM75::setOperationMode(opMode mode) {

    write8(LM75_CONF, read8(LM75_CONF) & 
        (mode == OPM_NORMAL ? LM75_CONF_DOM_NORMAL : LM75_CONF_DOM_SHUTDOWN));
}

/**
 *  \brief             Set the value of the OS fault queue.
 *  \param [in] fqueue Enumerated OS fault queue programming value.
 */
void LM75::setFaultQueue(faultQueue fqueue) {
    uint8_t fq;

    switch(fqueue) {
        case    FQ_1 : fq = LM75_CONF_OSFQUE_1; break;
        case    FQ_2 : fq = LM75_CONF_OSFQUE_1; break;
        case    FQ_4 : fq = LM75_CONF_OSFQUE_1; break;
        case    FQ_6 : fq = LM75_CONF_OSFQUE_1;
    }
    write8(LM75_CONF, read8(LM75_CONF) & fq);
}

/**
 *  \brief            Return the device I2C Bus address.
 *  \return           Device address.
 */
uint8_t LM75::getAddr(void) {return _addr;}

/**
 *  \brief            Return an 8 bit register value from the device.
 *  \param [in] reg   8b Register to read from.
 *  \return           Value read from register.
 */
uint8_t LM75::read8(uint8_t reg) {

    // send the device address then the register pointer byte
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    
    // resend device address then get the returned byte
    Wire.requestFrom(_addr, (uint8_t)1);

    // data is returned as 1 byte
    return Wire.read();
}

/**
 *  \brief            Return a 16 bit register value from the device.
 *  \param [in] reg   16b Register to read from.
 *  \return           Value read from register.
 */
uint16_t LM75::read16(uint8_t reg) {
    uint16_t val;

    // send the device address then the register pointer byte
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);

    // resend device address then get the 2 returned bytes
    Wire.requestFrom(_addr, (uint8_t)2);

    // data is returned as 2 bytes big endian
    val = Wire.read() << 8;
    val |= Wire.read();

    return val;
}

/**
 *  \brief            Write a value to an 8 bit register in the device.
 *  \param [in] reg   Register to write to.
 *  \param [in] data  Value to write.
 */
void LM75::write8(uint8_t reg, uint8_t data) {

    // send the device address then the register pointer byte
    Wire.beginTransmission(_addr);
    Wire.write(reg);

    // write the data
    Wire.write(data);
    Wire.endTransmission(true);
}

/**
 *  \brief            Write a value to a 16 bit register in the device.
 *  \param [in] reg   Register to write to.
 *  \param [in] data  Value to write.
 */
void LM75::write16(uint8_t reg, uint16_t data) {

    // send the device address then the register pointer byte
    Wire.beginTransmission(_addr);
    Wire.write(reg);

    // write the data high byte first
    Wire.write(highByte(data));
    Wire.write(lowByte(data));
    Wire.endTransmission(true);
}

/**
 *  \brief            Convert temperature in degrees C to degrees K.
 *  \param [in] degC  Temperature in degrees Centigrade.
 *  \return           Temperature in degrees Kelvin.
 */
double LM75::convCtoK(double degC) {return degC + 273.15;}

/**
 *  \brief            Convert temperature in degrees C to degrees F.
 *  \param [in] degC  Temperature in degrees Centigrade.
 *  \return           Temperature in degrees Fahrenheit.
 */
double LM75::convCtoF(double degC) {return (degC * 1.8) + 32.0;}

/**
 *  \brief            Convert temperature in degrees K to degrees C.
 *  \param [in] degK  Temperature in degrees Kelvin.
 *  \return           Temperature in degrees Centigrade.
 */
double LM75::convKtoC(double degK) {return degK - 273.15;}

/**
 *  \brief            Convert temperature in degrees F to degrees C.
 *  \param [in] degF  Temperature in degrees Fahrenheit.
 *  \return           Temperature in degrees Centigrade.
 */
double LM75::convFtoC(double degF) {return (degF - 32.0) / 1.8;}


