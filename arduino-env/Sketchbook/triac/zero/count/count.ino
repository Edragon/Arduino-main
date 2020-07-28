const uint8_t ledPin = 13;                          // Digital output pin that has the on board LED
const uint8_t zeroPin = 2;                          // Digital input pin to which the zero crossing detector is connected

uint8_t zeroCounter = 0;
bool ledState = 0;

void setup() {
    Serial.begin(9600);
    pinMode( ledPin , OUTPUT );                       // Enable output driver for LED pin
    
    // 实验：已经上拉10K
    // pinMode( zeroPin , INPUT );

    // 实验结果。 如果外部没有上拉电阻 那么这里需要
    pinMode( zeroPin , INPUT_PULLUP );
    
}

void loop() {
    while ( digitalRead(zeroPin) != 0 ); //wait until zeroPin is low, to make sure that the same zero crossing isn't detected multiple times.
    while ( digitalRead(zeroPin) == 0 ); //wait until zeroPin is high, that's when a zero crossing occurs.
    zeroCounter++;
    if ( zeroCounter >= 50 ) {         // Every 50 zero crossings change the LED state
        zeroCounter = 0;
        Serial.println("50 zero crossings have occurred and 500ms have passed.");
        ledState = !ledState;
        digitalWrite( ledPin , ledState );
    }
}
