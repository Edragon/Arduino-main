#include "Arduino.h"
#include "Nokia_5110.h"
#include "Font.h"

Nokia_5110::Nokia_5110(){}

Nokia_5110::Nokia_5110(unsigned short RST, unsigned short CE, unsigned short DC, unsigned short DIN, unsigned short CLK){
    _RST = RST;
    _CE = CE;
    _DC = DC;
    _DIN = DIN;
    _CLK = CLK;

    pinMode(RST, OUTPUT);
    pinMode(CE, OUTPUT);
    pinMode(DC, OUTPUT);
    pinMode(DIN, OUTPUT);
    pinMode(CLK, OUTPUT);

    _cursor = Cursor();

    reset();

    clear();
    setDisplayMode(Display_Mode::NORMAL);
    setBiasSystem(Mux_Rate::FORTY);
    setContrast(60);
}

void Nokia_5110::startTransmission(){
    digitalWrite(_CE, LOW);
}

void Nokia_5110::endTransmission(){
    digitalWrite(_CE, HIGH);
}

void Nokia_5110::transmitInformation(byte information){
    startTransmission();
    shiftOut(_DIN, _CLK, MSBFIRST, information);
    endTransmission();
}

void Nokia_5110::execute(byte command){
    initializeForSendingCommand();
    transmitInformation(command);
}

void Nokia_5110::initializeForSendingCommand(){
    digitalWrite(_DC, LOW);
}

/**
 * @param contrast could be 0 to 127
 */
void Nokia_5110::setContrast(unsigned short value){
    if(value > 127)
        return;

    extendedInstruction();
    
    const unsigned short leastValue = 128;
    execute(byte(leastValue + value));
}

void Nokia_5110::extendedInstruction(){
    execute(0x21);
}

void Nokia_5110::basicInstruction(){
    execute(0x20);
}

/**
 * Temperature Coefficient value could be one of 0, 1, 2 or 3;
 */
void Nokia_5110::setTemperatureCoefficient(unsigned short value){

    if(value > 3)
        return;

    extendedInstruction();

    const unsigned short leastValue = 4;
    execute(byte(leastValue + value));
}

void Nokia_5110::reset(){
    digitalWrite(_RST, LOW);
    digitalWrite(_RST, HIGH);
}

void Nokia_5110::initializeForSendingData(){
    digitalWrite(_DC, HIGH);
}

void Nokia_5110::makeEnoughSpaceForPrinting(unsigned short int newCharacterLength){
    if((newCharacterLength + _cursor.getPosition().x) < 83)
        return;

    _cursor.moveYAxis(1);
    setCursor(_cursor.getPosition().x, _cursor.getPosition().y);
    initializeForSendingData();
}

void Nokia_5110::_print(uint8_t charAsciiCode){

    if (charAsciiCode == 0x0a){ // \n for jumping to the beginning of a new line.
        _cursor.moveYAxis(1);
        return;
    }

    setCursor(_cursor.getPosition().x, _cursor.getPosition().y);

    initializeForSendingData();
    
    character fontData = findCorrespondingByte(charAsciiCode);
    makeEnoughSpaceForPrinting(fontData.definition_total_bytes);

    for(unsigned int i = 0; i < fontData.definition_total_bytes; i++){
        transmitInformation(fontData.definition[i]);
    }

    transmitInformation(0x0); // add an empty line after each chars
    _cursor.moveXAxis(fontData.definition_total_bytes + 1);
}

void Nokia_5110::setCursor(position x, position y){
    _cursor.setCursor(x, y);

    basicInstruction();

    //set x position
    unsigned short int leastXPositionValue = 128;
    execute(byte(leastXPositionValue + x));


    //set y position
    unsigned short int leastYPositionValue = 64;
    execute(byte(leastYPositionValue + y));
}

void Nokia_5110::clear(){
    initializeForSendingData();

    int i = 504;
    while(i >= 0){
        transmitInformation(0x0);
        i--;
    }

    setCursor(0, 0);
}

void Nokia_5110::clear(position inRow, position fromColumn, position toColumn){
    // toColumn has to be more than from Column, otherwise flip the values :D
    position temp;
    if(fromColumn > toColumn){
        temp       = fromColumn;
        fromColumn = toColumn;
        toColumn   = temp;
    }

    position counter = fromColumn;
    while(counter <= toColumn){
        setCursor(counter, inRow);
        initializeForSendingData();
        transmitInformation(0x0);
        counter++;
    }

    setCursor(fromColumn, inRow);
}


void Nokia_5110::setDisplayMode(display_mode value){
    basicInstruction();
    execute(value);
}


void Nokia_5110::setBiasSystem(mux_rate rate){
    extendedInstruction();
    execute(rate);
}

size_t Nokia_5110::write(uint8_t character) {
    _print(character);
    return 1;
}

