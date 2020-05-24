#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H

#include "mbed.h"
#include "Sseg.h"

class SevenSegmentDisplay{
    public:
        /**
         * Creates a 4 digit 7 segment display with common cathode.
         * @param a @param b @param c @param d @param e @param f
         * @param g @param dec These params represent the pins 
         * controlling the 7 segments and decimal point of each digit.
         * @param d1 @param d2 @param d3 @param d4 These params
         * represent the pins to select a digit, 1 being the leftmost.
         * @param value The point of the value to display and update.
         */
        SevenSegmentDisplay(
            PinName a,PinName b,PinName c,PinName d,
            PinName e,PinName f,PinName g,PinName dec,
            PinName d1,PinName d2, PinName d3, PinName d4,
            int *value
        );
    private:
        /**
         * The internal object to display value.
         */
        Sseg* sevenSegmentDisplay;
        /**
         * Thread that constantly displays value.
         */
        Thread displayThread;
        /**
         * Pointer to value to be displayed.
         */
        int* value;
        /**
         * Function that thread runs.
         */
        void displayValue();
};

#endif