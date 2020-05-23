#ifndef BUTTON_H
#define BUTTON_H

#include "mbed.h"

/**
#include "mbed.h"
#include "include/Game.h"
#include "include/hardware/Button.h"

DigitalOut x(LED1);
void toggle(){
	x = !x;
}
int main() {
	Button b(PTC6, toggle);
    while(1){
        //do stuff
	}
	return 0;
}  
*/

// This delay sets minimum time between clicks in millis.
#define DEBOUNCE_DELAY 15

class Button {
    public:
        /**
         * Create a debounced button for shooting.
         * Uses singleton design pattern.
         * @param button_pin Button pin on board.
         * @param shoot Handles shooting when button pressed.
         */
        Button(PinName button_pin, void (*shoot)());
        /**
         * Returns true if user presses button, else false.
         */
        bool is_shooting();
    private:
        /**
         * Shooting handler.
         */
        void (*shoot)();
        /**
         * Keeps track of current time. (in millis)
         */
        int time;
        /**
         * Keeps track of last time button was pressed. (in millis)
         */
        int prev_time;
        /**
         * mBed object to increment time.
         */
        Ticker ticker;
        /**
         * Ticker handler.
         */
        void ticker_handler();
        /**
         * Interrupt object for button.
         */
        InterruptIn *button;
        /**
         *  Button handler for rising edge.
         */
        void button_rise_handler();
        /**
         * Button handler for falling edge.
         */
        void button_fall_handler();
        /**
         * Button state.
         */
        bool is_clicked;
};

#endif