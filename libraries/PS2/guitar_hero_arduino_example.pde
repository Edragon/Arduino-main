#include <PS2X_lib.h>

#define RED 0
#define GREEN 1
#define BLUE 2
#define YELLOW 3
#define ORANGE 4


PS2X ps2x; // create PS2 Controller Class
byte colors; //byte to hold color statuses. 

void setup(){
  
 ps2x.config_gamepad(13,11,10,12);
  
 Serial.begin(115200);
 
}

void loop(){
  
    ps2x.read_gamepad();          //read controller 
    
    //cheack for color frets released
    if(ps2x.ButtonReleased(RED_FRET)) bitClear(colors, RED);
    if(ps2x.ButtonReleased(GREEN_FRET)) bitClear(colors, GREEN);
    if(ps2x.ButtonReleased(BLUE_FRET)) bitClear(colors, BLUE);
    if(ps2x.ButtonReleased(YELLOW_FRET)) bitClear(colors, YELLOW);
    if(ps2x.ButtonReleased(ORANGE_FRET)) bitClear(colors, ORANGE);
    
    //only turn a color on it if strummed
    if(ps2x.ButtonPressed(UP_STRUM) || ps2x.ButtonPressed(DOWN_STRUM)) {
      
      if(ps2x.Button(RED_FRET)) bitSet(colors, RED);
      if(ps2x.Button(GREEN_FRET)) bitSet(colors, GREEN);
      if(ps2x.Button(BLUE_FRET)) bitSet(colors, BLUE);
      if(ps2x.Button(YELLOW_FRET)) bitSet(colors, YELLOW);
      if(ps2x.Button(ORANGE_FRET)) bitSet(colors, ORANGE);

    }
    
    //send fret statues to PC
    for(int i=0; i<=4; i++){
      if(bitRead(colors, i)) Serial.print(1);
      else Serial.print(0);
    }
    
    //send whammy bar value to PC, 0 to 100 range
    Serial.print(map(ps2x.Analog(WHAMMY_BAR), 0,128,0,100), DEC);
    Serial.println('n');//end of message
   
 delay(100);
}
