#include "Temperature_LM75_Derived.h"

// The standard register layout for most devices, based on LM75.
Temperature_LM75_Derived::RegisterLayout LM75_Compatible_Registers = {
  .temperature      = 0x00,
  .configuration    = 0x01,
  .temperature_low  = 0x02,
  .temperature_high = 0x03,
};

Temperature_LM75_Derived::Attributes Generic_LM75_Attributes = {
  .temperature_width              = 16,
  .default_temperature_resolution = 9,
  .default_temperature_frac_width = 8,
  .max_temperature_resolution     = 9,
  .registers                      = &LM75_Compatible_Registers,
};

Temperature_LM75_Derived::Attributes Generic_LM75_10Bit_Attributes = {
  .temperature_width              = 16,
  .default_temperature_resolution = 10,
  .default_temperature_frac_width = 8,
  .max_temperature_resolution     = 10,
  .registers                      = &LM75_Compatible_Registers,
};

Temperature_LM75_Derived::Attributes Generic_LM75_11Bit_Attributes = {
  .temperature_width              = 16,
  .default_temperature_resolution = 11,
  .default_temperature_frac_width = 8,
  .max_temperature_resolution     = 11,
  .registers                      = &LM75_Compatible_Registers,
};

Temperature_LM75_Derived::Attributes Generic_LM75_12Bit_Attributes = {
  .temperature_width              = 16,
  .default_temperature_resolution = 12,
  .default_temperature_frac_width = 8,
  .max_temperature_resolution     = 12,
  .registers                      = &LM75_Compatible_Registers,
};

Temperature_LM75_Derived::Attributes TI_TMP102_Attributes = {
  .temperature_width              = 16,
  .default_temperature_resolution = 12,
  .default_temperature_frac_width = 8,
  .max_temperature_resolution     = 13,
  .registers                      = &LM75_Compatible_Registers,
};

int16_t Temperature_LM75_Derived::readIntegerTemperatureRegister(uint8_t register_index) {
  // Select the temperature register at register_index.
  bus->beginTransmission(i2c_address);
  bus->write(register_index);
  bus->endTransmission();

  // Start a transaction to read the register data.
  bus->requestFrom(i2c_address, (uint8_t) (resolution <= 8 ? 1 : 2));

  // Read the most significant byte of the temperature data.
  uint16_t t = bus->read() << 8;
  
  // Read the least significant byte of the temperature data, if requested.
  if (resolution > 8) {
    t |= bus->read();
  }

  // Finished reading the register data.
  bus->endTransmission();

  // Mask out unused/reserved bit from the full 16-bit register.
  t &= resolution_mask;

  // Read the raw memory as a 16-bit signed integer and return.
  return *(int16_t *)(&t);
}

void Temperature_LM75_Derived::writeIntegerTemperatureRegister(uint8_t register_index, int16_t value) {
  bus->beginTransmission(i2c_address);

  bus->write(register_index);

  uint16_t *value_ptr = (uint16_t *)&value;

  bus->write((uint8_t)((*value_ptr & 0xff00) >> 8));
  bus->write((uint8_t)(*value_ptr & 0x00ff));

  bus->endTransmission();
}

uint8_t Generic_LM75_Compatible::readConfigurationRegister() {
  bus->beginTransmission(i2c_address);
  bus->write(attributes->registers->configuration);
  bus->endTransmission();

  bus->requestFrom(i2c_address, (uint8_t) 1);
  uint8_t c = bus->read();
  bus->endTransmission();

  return c;
}

void Generic_LM75_Compatible::writeConfigurationRegister(uint8_t configuration) {
  bus->beginTransmission(i2c_address);

  bus->write(attributes->registers->configuration);
  bus->write(configuration);

  bus->endTransmission();
}

void Generic_LM75_Compatible::setConfigurationBits(uint8_t bits) {
  uint8_t configuration = readConfigurationRegister();

  configuration |= bits;

  writeConfigurationRegister(configuration);
}

void Generic_LM75_Compatible::clearConfigurationBits(uint8_t bits) {
  uint8_t configuration = readConfigurationRegister();

  configuration &= ~bits;

  writeConfigurationRegister(configuration);
}

void Generic_LM75_Compatible::setConfigurationBitValue(uint8_t value, uint8_t start, uint8_t width) {
  uint8_t configuration = readConfigurationRegister();

  uint8_t mask = ((1 << width) - 1) << start;

  configuration &= ~mask;
  configuration |= value << start;

  writeConfigurationRegister(configuration);
}

uint16_t TI_TMP102_Compatible::readExtendedConfigurationRegister() {
  bus->beginTransmission(i2c_address);
  bus->write(attributes->registers->configuration);
  bus->endTransmission();

  bus->requestFrom(i2c_address, (uint8_t) 2);
  uint16_t c = bus->read() << 8;
  c |= bus->read();
  bus->endTransmission();

  return c;
}

void TI_TMP102_Compatible::writeExtendedConfigurationRegister(uint16_t configuration) {
  bus->beginTransmission(i2c_address);

  bus->write(attributes->registers->configuration);
  bus->write((uint8_t)((configuration & 0xff00) >> 8));
  bus->write((uint8_t)(configuration & 0x00ff));

  bus->endTransmission();
}

void TI_TMP102_Compatible::setExtendedConfigurationBits(uint16_t bits) {
  uint16_t configuration = readExtendedConfigurationRegister();

  configuration |= bits;

  writeExtendedConfigurationRegister(configuration);
}

void TI_TMP102_Compatible::clearExtendedConfigurationBits(uint16_t bits) {
  uint16_t configuration = readExtendedConfigurationRegister();

  configuration &= ~bits;

  writeExtendedConfigurationRegister(configuration);
}

void TI_TMP102_Compatible::setExtendedConfigurationBitValue(uint16_t value, uint8_t start, uint8_t width) {
  uint16_t configuration = readExtendedConfigurationRegister();

  uint16_t mask = ((1 << width) - 1) << start;

  configuration &= ~mask;
  configuration |= value << start;

  writeExtendedConfigurationRegister(configuration);
}

void TI_TMP102_Compatible::enableExtendedMode() {
  setExtendedConfigurationBits(_BV(ExtendedConfigurationBits::ExtendedMode));
  setInternalResolution(13);
  setInternalTemperatureFracWidth(7);
}

void TI_TMP102_Compatible::disableExtendedMode() {
  clearExtendedConfigurationBits(_BV(ExtendedConfigurationBits::ExtendedMode));
  setInternalResolution(12);
  setInternalTemperatureFracWidth(8);
}
