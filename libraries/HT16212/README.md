# HT1621 7 segment LCD Arduino library
Arduino library for 7-segment lcds based on the HT1621 driver. Very often marked as PDC-6X1.

<img src="extras/photo.jpg" alt="photo" width="500">

Based on the bitbanging efforts by [anxzhu](https://github.com/anxzhu) (2016-2018).  
APIs rewritten in 2018 to follow the LiquidCrystal format by [valerio\new](https://github.com/5N44P).

## APIs reference

* `void begin(int cs_p, int wr_p, int data_p,[int backlight_p])`  
Starts the lcd with the pin assignement declared. The backlight pin is optional

* `void clear()`  
Clears the display

* `void backlight()`  
Turns on the backlight 

* `void noBacklight()`  
Turns off the backlight

* `void setBatteryLevel(int level)`  
Accepts values from 0 to 3. Smaller values will be treated like 0, bigger values will be treated as 3. 0 turns off the battery symbol. Values from 1 to 3 will be represented by the rectangles above the battery symbol. 

* `void print(long num)`  
Prints a signed integer between -99999 and 999999. Larger and smaller values will be displayed as -99999 and 999999

* `void print(float num)`  
Prints a float with 3 decimals. 

* `void print(float num, int precision)`  
Prints a float with 0 to 3 decimals, based on the `precision` parameter. 

* `void noDisplay()`  
Turns off the display (doesn't turn off the backlight) 

* `void display()`  
Turns the display back on after it has been disabled by `noDisplay()`


## Internal functioning

° -> 0x33 = 10 + 20 + 01 + 02  
C -> 0x1D = 10 + 01 + 04 + 08

```
  ___10___
 |        |
 01       20
 |___02___|
 |        |
 04       40
 |___08___|

```

