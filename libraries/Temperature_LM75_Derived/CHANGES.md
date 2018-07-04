# Changes #

## Release 1.0.1 (unreleased) ##

* Restructure vanity-named classes to use `#define` instead of subclassing so that they work consistently with regard to constructors.

## Release 1.0.0 (2018-02-03) ##

* Tested with 14 different makes/models of temperature sensors.
* Improve examples for clarity.
* Improve descriptions in the `library.properties` file.
* Add `TI_LM75` alias.

## Release 0.9.1 (2018-01-31) ##

* Replaced I²C with I2C in the name based on request from the Arduino team.
* Add `TI_TMP112` alias.

## Release 0.9.0 (2018-01-29) ##

* Initial version tested only with the Texas Instruments TMP102.