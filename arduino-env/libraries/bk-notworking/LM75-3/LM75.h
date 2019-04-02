#ifndef _LM75_H_
#define _LM75_H_

/***********************************************************************************************//**
 *  \brief      LM75 Family Device Driver Library - CPP Header file
 *  \details    Based on the Melexis LM75 Family Data Sheet 3901090614 Rev 004 09jun2008.
 *  \li         The current implementation does not manage PWM (only digital data by I2C).
 *  \li         Sleep mode is not implemented yet.
 *
 *  \note       THIS IS ONLY A PARTIAL RELEASE. THIS DEVICE CLASS IS CURRENTLY UNDERGOING
 *              ACTIVE DEVELOPMENT AND IS STILL MISSING SOME IMPORTANT FEATURES. PLEASE KEEP 
 *              THIS IN MIND IF YOU DECIDE TO USE THIS PARTICULAR CODE FOR ANYTHING.
 *
 *  \file       LM75.H
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

#if (ARDUINO >= 100)
    #include "Arduino.h"
#else
    #include "WProgram.h"
#endif
#include <Wire.h>
#include "Property.h"

/**************************************************************************************************/
/* Definitions                                                                                    */
/**************************************************************************************************/

#define LM75_I2CDEFAULTADDR     0x48    /**< Device default slave address */
#define LM75_BROADCASTADDR      0       /**< Device broadcast slave address */

/** REGISTER addresses. */
#define LM75_CONF               0x01    /**< RAM reg - Configuration */
#define LM75_TEMP               0x00    /**< RAM reg - Temperature */
#define LM75_TOS                0x03    /**< RAM reg - Overtemperature shutdown threshold */
#define LM75_THYST              0x02    /**< RAM reg - Hysteresis */

/** CONFIGURATION bits masks. */
#define LM75_CONF_RES           0x00    /**< Manufacturer reserved bits */
#define LM75_CONF_OSFQUE_1      0x00    /**< OS fault queue programming value = 1 */
#define LM75_CONF_OSFQUE_2      0x08    /**< OS fault queue programming value = 2 */
#define LM75_CONF_OSFQUE_4      0x10    /**< OS fault queue programming value = 4 */
#define LM75_CONF_OSFQUE_6      0x18    /**< OS fault queue programming value = 6 */
#define LM75_CONF_OSPOL_AL      0x00    /**< OS polarity selection active LOW */
#define LM75_CONF_OSPOL_AH      0x04    /**< OS polarity selection active HIGH */
#define LM75_CONF_OSOM_COMP     0x00    /**< OS operation mode - comparator */
#define LM75_CONF_OSOM_INT      0x02    /**< OS operation mode - interrupt */
#define LM75_CONF_DOM_NORMAL    0x00    /**< Device operation mode - normal */
#define LM75_CONF_DOM_SHUTDOWN  0x01    /**< Device operation mode - shutdown */

/**************************************************************************************************/
/* LM75 Device class.                                                                             */
/**************************************************************************************************/

class LM75 {
public:
    LM75(uint8_t = LM75_I2CDEFAULTADDR);                    /**< constructor */

    boolean  begin();

    Property<uint8_t, LM75> busAddr;                        /**< I2C Bus address property */

    /** Enumerations for temperature units. */
    enum tempUnit_t {LM75_TK,                               /**< degrees Kelvin */
                     LM75_TC,                               /**< degrees Centigrade */
                     LM75_TF                                /**< degrees Fahrenheit */
    };                                                      
    /** Enumerations for setpoint registers. */
    enum setPointType {SPT_OVERTEMP,                        /**< overtemp shutdown register */
                       SPT_HYSTERESIS                       /**< hysteresis register */
    };
    /** Enumerations for output polarity. */
    enum outputPolarity {POL_LOW,                           /**< OS output active low */
                         POL_HIGH                           /**< OS output active high */
    };
    /** Enumerations for output mode. */
    enum outputMode {MODE_COMP,                             /**< OS comparator mode */
                     MODE_INT                               /**< OS interrupt mode */
    };
    /** Enumerations for operating mode. */
    enum opMode {OPM_NORMAL,                                /**< normal operation mode */
                 OPM_SHDN                                   /**< shutdown mode */
    };
    /** Enumerations for fault queue length. */
    enum faultQueue {FQ_1 = 1,                              /**< fault queue value = 1 */
                     FQ_2 = 2,                              /**< fault queue value = 2 */
                     FQ_4 = 4,                              /**< fault queue value = 4 */
                     FQ_6 = 6                               /**< fault queue value = 6 */
    };

    boolean isReady(void) { return _ready; };
    double readTemp(tempUnit_t = LM75_TC);
    double readSetPoint(tempUnit_t = LM75_TC, setPointType = SPT_OVERTEMP);
    void writeSetPoint(double, tempUnit_t = LM75_TC, setPointType = SPT_OVERTEMP);
    void setOutputPolarity(outputPolarity = POL_LOW);
    void setOutputMode(outputMode = MODE_COMP);
    void setOperationMode(opMode = MODE_COMP);
    void setFaultQueue(faultQueue);

    double convCtoK(double degC);
    double convCtoF(double degC);
    double convKtoC(double degK);
    double convFtoC(double degF);

private:
    boolean _ready;
    uint8_t _addr;                                          /**< Slave address */
    uint8_t getAddr(void);

    uint8_t read8(uint8_t);                            
    uint16_t read16(uint8_t);                          
    void write8(uint8_t, uint8_t);                
    void write16(uint8_t, uint16_t);              
};

#endif /* _LM75_H_ */
