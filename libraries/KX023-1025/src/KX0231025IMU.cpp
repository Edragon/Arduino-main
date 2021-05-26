/*
  This file is part of the KX023-1025-IMU library.
  Copyright (c) 2021 Good Solutions Sweden AB. All rights reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "KX0231025IMU.h"

#define KX0231025_ADDRESS 0x18

#define KX0231025_WHO_AM_I_REG 0X0F
#define KX0231025_CTRL1 0X18
#define KX0231025_CTRL2 0X19
#define KX0231025_CTRL3 0X20
#define KX0231025_ODCNTL 0X1B

#define KX0231025_STATUS_REG 0X15

#define KX0231025_OUTX_L_XL 0X06
#define KX0231025_OUTX_H_XL 0X07
#define KX0231025_OUTY_L_XL 0X08
#define KX0231025_OUTY_H_XL 0X09
#define KX0231025_OUTZ_L_XL 0X10
#define KX0231025_OUTZ_H_XL 0X11

#define KX0231025_RANGE_2G 0
#define KX0231025_RANGE_4G 1
#define KX0231025_RANGE_8G 2

#define KX0231025_DATARATE_12_5HZ 0
#define KX0231025_DATARATE_25HZ 1
#define KX0231025_DATARATE_50HZ 2
#define KX0231025_DATARATE_100HZ 3
#define KX0231025_DATARATE_200HZ 4
#define KX0231025_DATARATE_400HZ 5
#define KX0231025_DATARATE_800HZ 6
#define KX0231025_DATARATE_1600HZ 7
#define KX0231025_DATARATE_0_781HZ 8
#define KX0231025_DATARATE_1_563HZ 9
#define KX0231025_DATARATE_3_125HZ 10
#define KX0231025_DATARATE_6_25HZ 11

#define KX0231025_LOWPOWER_MODE 0X00
#define KX0231025_HIGHPOWER_MODE 0X40

KX0231025Class::KX0231025Class(TwoWire &wire, uint8_t slaveAddress) : _wire(&wire),
																	  _spi(NULL),
																	  _slaveAddress(slaveAddress),
																	  _sda(0),
																	  _scl(0)
{
}
KX0231025Class::KX0231025Class(TwoWire &wire, uint8_t slaveAddress, int sda, int scl) : _wire(&wire),
																						_spi(NULL),
																						_slaveAddress(slaveAddress),
																						_sda(sda),
																						_scl(scl)
{
}
KX0231025Class::KX0231025Class(SPIClass &spi, int csPin) : _wire(NULL),
														   _spi(&spi),
														   _csPin(csPin),
														   _spiSettings(10E6, MSBFIRST, SPI_MODE0)
{
}

KX0231025Class::~KX0231025Class()
{
}
int KX0231025Class::begin(int powerMode, int accelerationRange, int outputDataRate)
{
	this->_accelerationRange = accelerationRange;

	if (_spi != NULL)
	{
		pinMode(_csPin, OUTPUT);
		digitalWrite(_csPin, HIGH);
		_spi->begin();
	}
	else if (_sda != 0)
	{
		_wire->begin(_sda, _scl);
	}
	else
	{
		_wire->begin();
	}

	if (readRegister(KX0231025_WHO_AM_I_REG) != 0x15)
	{
		end();
		return 0;
	}

	if (powerMode == 0 && (outputDataRate == KX0231025_DATARATE_400HZ || outputDataRate == KX0231025_DATARATE_400HZ || outputDataRate == KX0231025_DATARATE_400HZ))
	{
		//Data rate not supported in low power mode
		return 0;
	}

	writeRegister(KX0231025_ODCNTL, outputDataRate);

	int activateValue = 0x80 | powerMode;

	if (this->_accelerationRange == KX0231025_RANGE_4G)
	{
		activateValue = activateValue | 0x08;
	}
	if (this->_accelerationRange == KX0231025_RANGE_8G)
	{
		activateValue = activateValue | 0x10;
	}

	writeRegister(KX0231025_CTRL1, activateValue);

	return 1;
}

void KX0231025Class::end()
{
	if (_spi != NULL)
	{
		_spi->end();
		digitalWrite(_csPin, LOW);
		pinMode(_csPin, INPUT);
	}
	else
	{
		writeRegister(KX0231025_CTRL1, 0x18);
		_wire->endTransmission();
	}
}
int KX0231025Class::getSda(){
	return _sda;
}
int KX0231025Class::getScl(){
	return _scl;
}
int KX0231025Class::readRawData(int16_t &x, int16_t &y, int16_t &z){
	int16_t data[3];

	if (!readRegisters(KX0231025_OUTX_L_XL, (uint8_t *)data, sizeof(data)))
	{
		x = 0;
		y = 0;
		z = 0;

		return 0;
	}
	if (this->_accelerationRange == KX0231025_RANGE_2G)
	{
		x = data[0] * 2;
		y = data[1] * 2;
		z = data[2] * 2;
	}
	if (this->_accelerationRange == KX0231025_RANGE_4G)
	{
		x = data[0] * 4;
		y = data[1] * 4;
		z = data[2] * 4;
	}
	if (this->_accelerationRange == KX0231025_RANGE_8G)
	{
		x = data[0] * 8;
		y = data[1] * 8;
		z = data[2] * 8;
	}

	return 1;
}
int KX0231025Class::readAcceleration(float &x, float &y, float &z)
{
	int16_t data[3];

	if (!readRegisters(KX0231025_OUTX_L_XL, (uint8_t *)data, sizeof(data)))
	{
		x = NAN;
		y = NAN;
		z = NAN;

		return 0;
	}
	if (this->_accelerationRange == KX0231025_RANGE_2G)
	{
		x = data[0] * 2.0 / 32768.0;
		y = data[1] * 2.0 / 32768.0;
		z = data[2] * 2.0 / 32768.0;
	}
	if (this->_accelerationRange == KX0231025_RANGE_4G)
	{
		x = data[0] * 4.0 / 32768.0;
		y = data[1] * 4.0 / 32768.0;
		z = data[2] * 4.0 / 32768.0;
	}
	if (this->_accelerationRange == KX0231025_RANGE_8G)
	{
		x = data[0] * 8.0 / 32768.0;
		y = data[1] * 8.0 / 32768.0;
		z = data[2] * 8.0 / 32768.0;
	}

	return 1;
}

int KX0231025Class::readRegister(uint8_t address)
{
	uint8_t value;

	if (readRegisters(address, &value, sizeof(value)) != 1)
	{
		return -1;
	}

	return value;
}

int KX0231025Class::readRegisters(uint8_t address, uint8_t *data, size_t length)
{
	if (_spi != NULL)
	{
		_spi->beginTransaction(_spiSettings);
		digitalWrite(_csPin, LOW);
		_spi->transfer(0x80 | address);
		_spi->transfer(data, length);
		digitalWrite(_csPin, HIGH);
		_spi->endTransaction();
	}
	else
	{
		_wire->beginTransmission(_slaveAddress);
		_wire->write(address);

		if (_wire->endTransmission(false) != 0)
		{
			return -1;
		}

		if (_wire->requestFrom(_slaveAddress, length) != length)
		{
			return 0;
		}

		for (size_t i = 0; i < length; i++)
		{
			*data++ = _wire->read();
		}
	}
	return 1;
}

int KX0231025Class::writeRegister(uint8_t address, uint8_t value)
{
	if (_spi != NULL)
	{
		_spi->beginTransaction(_spiSettings);
		digitalWrite(_csPin, LOW);
		_spi->transfer(address);
		_spi->transfer(value);
		digitalWrite(_csPin, HIGH);
		_spi->endTransaction();
	}
	else
	{
		_wire->beginTransmission(_slaveAddress);
		_wire->write(address);
		_wire->write(value);
		if (_wire->endTransmission() != 0)
		{
			return 0;
		}
	}
	return 1;
}
