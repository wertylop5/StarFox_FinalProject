#ifndef SHOOTER_H
#define SHOOTER_H

#include "mbed.h"

// This delay sets minimum time between clicks in millis.
#define DEBOUNCE_DELAY 15

class Shooter {
    public:
        /**
         * Create a debounced button for shooting.
         * Uses singleton design pattern.
         * @param button_pin Button pin on board.
         * @param shoot Handles shooting when button pressed.
         */
        static void create_shooter(PinName button_pin, void (*shoot)());
    private:
        /**
         * Hide default constructor so that it cannot be instantiated
         * using it.
         */
        Shooter();
        /**
         * Keeps track of number of shooters created.
         * Note: can only be zero or one.
         */
        static int count;
        /**
         * Shooting handler.
         */
        static void (*shoot)();
        /**
         * Keeps track of current time. (in millis)
         */
        static int time;
        /**
         * Keeps track of last time button was pressed. (in millis)
         */
        static int prev_time;
        /**
         * mBed object to increment time.
         */
        static Ticker ticker;
        /**
         * Ticker handler.
         */
        static void ticker_handler();
        static InterruptIn *button;
        /**
         *  Button handler.
         */
        static void button_handler();
};

#endif