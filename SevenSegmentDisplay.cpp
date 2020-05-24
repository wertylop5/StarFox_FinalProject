#include "include/hardware/SevenSegmentDisplay.h"

SevenSegmentDisplay::SevenSegmentDisplay(
    PinName a,PinName b,PinName c,PinName d,
    PinName e,PinName f,PinName g,PinName dec,
    PinName d1,PinName d2, PinName d3, PinName d4,
    int *value
){
    sevenSegmentDisplay = new Sseg(
        a, b, c, d, e, f, g, dec, d1, d2, d3, d4
    );
    sevenSegmentDisplay->begin();
    sevenSegmentDisplay->setKcommon();
    this->value = value;
    displayThread.start(callback(this, &SevenSegmentDisplay::displayValue));
}

void SevenSegmentDisplay::displayValue(){
    while(1){
        sevenSegmentDisplay->writeNum4(*value);
        sevenSegmentDisplay->updateOnce();
    }
}

