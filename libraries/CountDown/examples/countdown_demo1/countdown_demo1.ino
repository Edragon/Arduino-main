//
//    FILE: countdown_demo.ino
//  AUTHOR: Rob Tillaart
// VERSION: 0.1.0
// PURPOSE: demo
//    DATE: 2015-10-28
//     URL: http://forum.arduino.cc/index.php?topic=356253
//
// Released to the public domain
//

#include "CountDown.h"

CountDown CD(CountDown::SECONDS);

void setup()
{
    Serial.begin(115200);
    Serial.println(__FILE__);
    Serial.print("COUNTDOWN_LIB_VERSION: ");
    Serial.println(COUNTDOWN_LIB_VERSION);

    CD.start(5);

    while (CD.remaining() > 0 && CD.isRunning() )
    {
        Serial.println(CD.remaining() );
        if (CD.remaining() < 2) CD.stop();
    }
    Serial.println(CD.remaining());
    Serial.println("done...");
}

void loop()
{
}
