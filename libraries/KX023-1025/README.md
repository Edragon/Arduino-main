# KX023-1025-IMU
Arduino library for KX023-1025 https://datasheet.lcsc.com/szlcsc/ROHM-Semicon-KX023-1025_C236128.pdf  

Simple implementation of the basic functions for reading accelerometer data.

# Constructors

## SPI
```
KX0231025Class(SPIClass& spi, int csPin);
```
## I2C
```
KX0231025Class(TwoWire& wire, uint8_t slaveAddress);
```
I2C has not been verified and tested as of yet. Refer to documentation for the correct slave address

# Functions

## begin(), begin(int powerMode, int accelerationRange, int outputDataRate)
Configures the accelerometer and changes its mode to operational mode

400,800,1600Hz mode is not possible in LOWPOWER powerMode.  
LOWPOWER mode will affect the accuracy  
With higher range it will loose accuracy by a factor of 2 for each step. 2G highest accuracy, 8G lowest accuracy  

### return
0 = Could not initialize with current settings  
1 = Success

```
#define KX0231025_LOWPOWER_MODE    0X00
#define KX0231025_HIGHPOWER_MODE   0X40

#define KX0231025_RANGE_2G       0
#define KX0231025_RANGE_4G       1
#define KX0231025_RANGE_8G       2

#define KX0231025_DATARATE_12_5HZ  0
#define KX0231025_DATARATE_25HZ    1
#define KX0231025_DATARATE_50HZ    2
#define KX0231025_DATARATE_100HZ   3
#define KX0231025_DATARATE_200HZ   4
#define KX0231025_DATARATE_400HZ   5
#define KX0231025_DATARATE_800HZ   6
#define KX0231025_DATARATE_1600HZ  7
#define KX0231025_DATARATE_0_781HZ   8
#define KX0231025_DATARATE_1_563HZ   9
#define KX0231025_DATARATE_3_125HZ   10
#define KX0231025_DATARATE_6_25HZ  11

int begin(); // Standard values KX0231025_HIGHPOWER_MODE, KX0231025_RANGE_2G, KX0231025_DATARATE_50HZ
int begin(KX0231025_HIGHPOWER_MODE, KX0231025_RANGE_2G, KX0231025_DATARATE_50HZ);
```

## end()
Sets the chip in standby mode  
### return
0 = Failed  
1 = Success

```
int end();
```

## readAcceleration(float& x, float& y, float& z);
reads the latest available values from the accelerometer  
### return
0 = Failed  
1 = Success

```
virtual int readAcceleration(float& x, float& y, float& z); // Results are in G (earth gravity).
```
