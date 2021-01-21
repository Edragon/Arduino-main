#include "Cursor.h"

void Cursor::setCursor(position x, position y){
    _x = x;
    _y = y;
}

/**
 * Moves cursor in x axis by a number sepcified in method's parameter
 */
void Cursor::moveXAxis(position by){
    if(by == 0)
        return;
    
    _x++;

    if(_x > 83){
        moveYAxis(1);
        _x = 0;
    }

    moveXAxis(--by);
}

/**
 * Moves cursor in y axis by a number sepcified in method's parameter
 */
void Cursor::moveYAxis(position by){
    if(by == 0)
        return;

    _y++;
    _x = 0; // for each y incrementation, reset the x axis :D

    if(_y > 5){
        _y = 0;
    }

    moveYAxis(--by);
}

/**
 * Returns cursor's current position.
 */
Position Cursor::getPosition(){
    Position currentPosition;
    currentPosition.y = _y;
    currentPosition.x = _x;

    return currentPosition;
}
