#ifndef TEMPERATURE_LM75_DERIVED_H
#define TEMPERATURE_LM75_DERIVED_H

#include <Wire.h>

class Temperature_LM75_Derived {
public:

  // The layout of registers accessed through the I2C protocol.
  struct RegisterLayout {
    uint8_t temperature;
    uint8_t configuration;
    uint8_t temperature_low;
    uint8_t temperature_high;
  };

  // Attributes about a device or family of devices (if the attributes are shared).
  struct Attributes {
    uint8_t temperature_width;
    uint8_t default_temperature_resolution;
    uint8_t default_temperature_frac_width;
    uint8_t max_temperature_resolution;
    RegisterLayout *registers;
  };

  // The typical I2C address for any device.
  static const uint8_t DEFAULT_I2C_ADDRESS = 0x48;

  inline static float convertCtoF(float c) {
    return c * 1.8 + 32;
  }

  inline static float convertFtoC(float f) {
    return (f - 32) / 1.8;
  }

protected:

  TwoWire *bus;
  uint8_t i2c_address;
  uint8_t resolution;
  uint16_t resolution_mask;
  uint8_t temperature_frac_width;
  float temperature_frac_factor;
  Attributes *attributes;

  // Set the internal resolution of the temperature sensor, which affects
  // conversions and which bits are discarded.
  void setInternalResolution(uint8_t resolution) {
    this->resolution = resolution;
    this->resolution_mask = ~(uint16_t)((1 << (attributes->temperature_width - resolution)) - 1);
  }

  void setInternalTemperatureFracWidth(uint8_t temperature_frac_width) {
    this->temperature_frac_width = temperature_frac_width;
    temperature_frac_factor = 1.0 / (float)(1 << temperature_frac_width);
  }

  int16_t readIntegerTemperatureRegister(uint8_t register_index);
  void writeIntegerTemperatureRegister(uint8_t register_index, int16_t value);

public:

  Temperature_LM75_Derived(TwoWire *bus, uint8_t i2c_address, Attributes *attributes) {
    this->bus = bus;
    this->i2c_address = i2c_address;
    this->attributes = attributes;

    setInternalResolution(attributes->default_temperature_resolution);
    setInternalTemperatureFracWidth(attributes->default_temperature_frac_width);
  }

  inline float convertIntegerTemperature(int16_t value) {
    return (float)value * temperature_frac_factor;
  }

  inline int16_t convertFloatTemperature(float value) {
    return (int16_t)(value / temperature_frac_factor);
  }

  float readTemperatureC() {
    return convertIntegerTemperature(readIntegerTemperatureRegister(attributes->registers->temperature));
  }

  float readTemperatureF() {
    return convertCtoF(readTemperatureC());
  }

  float readTemperatureLowC() {
    return convertIntegerTemperature(readIntegerTemperatureRegister(attributes->registers->temperature_low));
  }

  float readTemperatureLowF() {
    return convertCtoF(readTemperatureLowC());
  }

  void setTemperatureLowC(float value) {
    writeIntegerTemperatureRegister(attributes->registers->temperature_low, convertFloatTemperature(value));
  }

  void setTemperatureLowF(float value) {
    setTemperatureLowC(convertFtoC(value));
  }

  float readTemperatureHighC() {
    return convertIntegerTemperature(readIntegerTemperatureRegister(attributes->registers->temperature_high));
  }

  float readTemperatureHighF() {
    return convertCtoF(readTemperatureHighC());
  }

  void setTemperatureHighC(float value) {
    writeIntegerTemperatureRegister(attributes->registers->temperature_high, convertFloatTemperature(value));
  }

  void setTemperatureHighF(float value) {
    setTemperatureHighC(convertFtoC(value));
  }
};

extern Temperature_LM75_Derived::Attributes Generic_LM75_Attributes;

class Generic_LM75_Compatible : public Temperature_LM75_Derived {
protected:

  enum ConfigurationBits {
    FaultQueueLength  = 3, // mask 0x18, length 2 bits
    AlertPolarity     = 2, // mask 0x04, length 1 bit
    ThermostatMode    = 1, // mask 0x02, length 1 bit
    Shutdown          = 0, // mask 0x01, length 1 bit
  };

  uint8_t readConfigurationRegister();
  void writeConfigurationRegister(uint8_t configuration);

  uint8_t readConfigurationBits(uint8_t bits) {
    return readConfigurationRegister() & bits;
  }

  bool checkConfigurationBits(uint8_t bits) {
    return (readConfigurationRegister() & bits) == bits;
  }

  void setConfigurationBits(uint8_t bits);
  void clearConfigurationBits(uint8_t bits);
  void setConfigurationBitValue(uint8_t value, uint8_t start, uint8_t width);

public:

  enum FaultQueueLength {
    FaultQueueLength_1_fault   = 0,
    FaultQueueLength_2_faults  = 1,
    FaultQueueLength_4_faults  = 2,
    FaultQueueLength_6_faults  = 3,
  };

  Generic_LM75_Compatible(TwoWire *bus, uint8_t i2c_address, Attributes *attributes)
    : Temperature_LM75_Derived(bus, i2c_address, attributes) { };

  void setFaultQueueLength(enum FaultQueueLength faults) {
    setConfigurationBitValue(faults, ConfigurationBits::FaultQueueLength, 2);
  }

  void setAlertActiveLow() {
    clearConfigurationBits(_BV(ConfigurationBits::AlertPolarity));
  }

  void setAlertActiveHigh() {
    setConfigurationBits(_BV(ConfigurationBits::AlertPolarity));
  }

  void setThermostatComparatorMode() {
    clearConfigurationBits(_BV(ConfigurationBits::ThermostatMode));
  }

  void setThermostatInterruptMode() {
    setConfigurationBits(_BV(ConfigurationBits::ThermostatMode));
  }

  void enableShutdownMode() {
    setConfigurationBits(_BV(ConfigurationBits::Shutdown));
  }

  void disableShutdownMode() {
    clearConfigurationBits(_BV(ConfigurationBits::Shutdown));
  }
};

class Generic_LM75 : public Generic_LM75_Compatible {
public:
  Generic_LM75(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_Compatible(bus, i2c_address, &Generic_LM75_Attributes) { };

  Generic_LM75(uint8_t i2c_address)
    : Generic_LM75_Compatible(&Wire, i2c_address, &Generic_LM75_Attributes) { };
};

extern Temperature_LM75_Derived::Attributes Generic_LM75_10Bit_Attributes;

class Generic_LM75_10Bit : public Generic_LM75_Compatible {
public:
  Generic_LM75_10Bit(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_Compatible(bus, i2c_address, &Generic_LM75_10Bit_Attributes) { };

  Generic_LM75_10Bit(uint8_t i2c_address)
    : Generic_LM75_Compatible(&Wire, i2c_address, &Generic_LM75_10Bit_Attributes) { };
};

extern Temperature_LM75_Derived::Attributes Generic_LM75_11Bit_Attributes;

class Generic_LM75_11Bit : public Generic_LM75_Compatible {
public:
  Generic_LM75_11Bit(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_Compatible(bus, i2c_address, &Generic_LM75_11Bit_Attributes) { };

  Generic_LM75_11Bit(uint8_t i2c_address)
    : Generic_LM75_Compatible(&Wire, i2c_address, &Generic_LM75_11Bit_Attributes) { };
};

extern Temperature_LM75_Derived::Attributes Generic_LM75_12Bit_Attributes;

class Generic_LM75_12Bit : public Generic_LM75_Compatible {
public:
  Generic_LM75_12Bit(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_Compatible(bus, i2c_address, &Generic_LM75_12Bit_Attributes) { };

  Generic_LM75_12Bit(uint8_t i2c_address)
    : Generic_LM75_Compatible(&Wire, i2c_address, &Generic_LM75_12Bit_Attributes) { };
};

class Generic_LM75_9_to_12Bit_Compatible : public Generic_LM75_Compatible {
protected:

  enum ConfigurationBits {
    Resolution        = 5, // mask 0x60, length 2 bits
  };

public:

  enum Resolution {
    Resolution_9_bits   = 0,
    Resolution_10_bits  = 1,
    Resolution_11_bits  = 2,
    Resolution_12_bits  = 3,
  };

  Generic_LM75_9_to_12Bit_Compatible(TwoWire *bus, uint8_t i2c_address, Attributes *attributes)
    : Generic_LM75_Compatible(bus, i2c_address, attributes) { };

  void setResolution(enum Resolution resolution) {
    setConfigurationBitValue(resolution, ConfigurationBits::Resolution, 2);
  }
};

extern Temperature_LM75_Derived::Attributes Generic_LM75_12Bit_Attributes;

class Generic_LM75_9_to_12Bit : public Generic_LM75_9_to_12Bit_Compatible {
public:
  Generic_LM75_9_to_12Bit(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_9_to_12Bit_Compatible(bus, i2c_address, &Generic_LM75_12Bit_Attributes) { };

  Generic_LM75_9_to_12Bit(uint8_t i2c_address)
    : Generic_LM75_9_to_12Bit_Compatible(&Wire, i2c_address, &Generic_LM75_12Bit_Attributes) { };
};


class Generic_LM75_9_to_12Bit_OneShot_Compatible : public Generic_LM75_9_to_12Bit_Compatible {
private:

  enum ConfigurationBits {
    OneShot           = 7, // mask 0x80, length 1 bit
  };

public:

  Generic_LM75_9_to_12Bit_OneShot_Compatible(TwoWire *bus, uint8_t i2c_address, Attributes *attributes)
    : Generic_LM75_9_to_12Bit_Compatible(bus, i2c_address, attributes) { };

  void startOneShotConversion() {
    setConfigurationBits(_BV(ConfigurationBits::OneShot));
  }

  bool checkConversionReady() {
    return checkConfigurationBits(_BV(ConfigurationBits::OneShot));
  }
};

class Generic_LM75_9_to_12Bit_OneShot : public Generic_LM75_9_to_12Bit_OneShot_Compatible {
public:
  Generic_LM75_9_to_12Bit_OneShot(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_9_to_12Bit_OneShot_Compatible(bus, i2c_address, &Generic_LM75_12Bit_Attributes) { };

  Generic_LM75_9_to_12Bit_OneShot(uint8_t i2c_address)
    : Generic_LM75_9_to_12Bit_OneShot_Compatible(&Wire, i2c_address, &Generic_LM75_12Bit_Attributes) { };
};


extern Temperature_LM75_Derived::Attributes TI_TMP102_Attributes;

class TI_TMP102_Compatible : public Generic_LM75_9_to_12Bit_OneShot_Compatible {
private:

  enum ExtendedConfigurationBits {
    ConversionRate  = 6, // mask 0x00c0, length 2 bits
    Alert           = 5, // mask 0x0020, length 1 bit
    ExtendedMode    = 4, // mask 0x0010, length 1 bit
  };

  enum ConversionRate {
    ConversionRate_0_25Hz = 0,
    ConversionRate_1_Hz   = 1,
    ConversionRate_4_Hz   = 2,
    ConversionRate_8_Hz   = 3,
  };

  uint16_t readExtendedConfigurationRegister();
  void writeExtendedConfigurationRegister(uint16_t configuration);

  uint16_t readExtendedConfigurationBits(uint16_t bits) {
    return readExtendedConfigurationRegister() & bits;
  }

  bool checkExtendedConfigurationBits(uint16_t bits) {
    return (readExtendedConfigurationRegister() & bits) == bits;
  }

  void setExtendedConfigurationBits(uint16_t bits);
  void clearExtendedConfigurationBits(uint16_t bits);
  void setExtendedConfigurationBitValue(uint16_t value, uint8_t start, uint8_t width);

public:

  TI_TMP102_Compatible(TwoWire *bus = &Wire, uint8_t i2c_address = DEFAULT_I2C_ADDRESS)
    : Generic_LM75_9_to_12Bit_OneShot_Compatible(bus, i2c_address, &TI_TMP102_Attributes) { };

  TI_TMP102_Compatible(uint8_t i2c_address)
    : Generic_LM75_9_to_12Bit_OneShot_Compatible(&Wire, i2c_address, &TI_TMP102_Attributes) { };

  void setConversionRate(enum ConversionRate rate) {
    setExtendedConfigurationBitValue(rate, ExtendedConfigurationBits::ConversionRate, 2);
  }

  bool checkAlert() {
    return checkExtendedConfigurationBits(_BV(ExtendedConfigurationBits::Alert));
  }

  void enableExtendedMode();
  void disableExtendedMode();
};

#define Maxim_DS1775            Generic_LM75_9_to_12Bit
#define Maxim_DS7505            Generic_LM75_9_to_12Bit
#define Maxim_DS75LV            Generic_LM75_9_to_12Bit
#define Maxim_DS75LX            Generic_LM75_9_to_12Bit
#define Microchip_AT30TS750A    Generic_LM75_9_to_12Bit_OneShot
#define Microchip_MCP9800       Generic_LM75_9_to_12Bit_OneShot
#define Microchip_MCP9801       Generic_LM75_9_to_12Bit_OneShot
#define Microchip_MCP9802       Generic_LM75_9_to_12Bit_OneShot
#define Microchip_MCP9803       Generic_LM75_9_to_12Bit_OneShot
#define NXP_LM75A               Generic_LM75_11Bit
#define NXP_LM75B               Generic_LM75_11Bit
#define NXP_PCT2075             Generic_LM75_11Bit
#define NXP_SE95                Generic_LM75_12Bit
#define ON_NCT75                Generic_LM75_12Bit
#define ST_STCN75               Generic_LM75
#define ST_STLM75               Generic_LM75
#define ST_STTS75               Generic_LM75_9_to_12Bit_OneShot
#define TI_LM75                 Generic_LM75
#define TI_LM75A                Generic_LM75
#define TI_LM75B                Generic_LM75
#define TI_LM75C                Generic_LM75
#define TI_TMP100               Generic_LM75_9_to_12Bit_OneShot
#define TI_TMP101               Generic_LM75_9_to_12Bit_OneShot
#define TI_TMP102               TI_TMP102_Compatible
#define TI_TMP112               TI_TMP102_Compatible
#define TI_TMP175               Generic_LM75_9_to_12Bit_OneShot
#define TI_TMP275               Generic_LM75_9_to_12Bit_OneShot
#define TI_TMP75                Generic_LM75_9_to_12Bit_OneShot

#endif // TEMPERATURE_LM75_DERIVED_H
