#ifndef LEFTRIGHTCONTROLSTICK_H
#define LEFTRIGHTCONTROLSTICK_H
#include "mbed.h"
#include "Joystick.h"
#include "Button.h"

/**
 * We determined that on the x axis of a joystick, anything less
 * than 0.1 is considered a left and anything greater than 0.7 is
 * considered a right. Modify these values as neccessary for your
 * joystick.
 */
#define MIN_NEUTRAL 0.1
#define MAX_NEUTRAL 0.7

class LeftRightControlStick{
    public: 
        /**
         * Creates a joystick that takes angle movement along x and y axis 
         * and determines whether user is pushing the stick to the left/right
         * or pushing down on the stick.
         * @param xAnglePin Pin for tracking motion on the x axis.
         * @param yAnglePin Pin for tracking motion on the y axis.
         * @param buttonPin Pin for tracking state of internal button.
         * @param sleep_time Time between subsequent polls.
         * @param move_left Handler for joystick left.
         * @param move_right Handler for joystick right.
         * @param press Handler for button press
         * NOTE: Does nothing if joystick is at the origin.
         */
        LeftRightControlStick(
            PinName xAnglePin, PinName yAnglePin, PinName buttonPin, 
            void(*move_left)(), void(*move_right)(), void(*press)()
        );
        /**
         * Checks if joystick motion and calls corresponding handlers.
         */
        void checkJoystick();
    private:
        /**
         * Internal joystick object to keep track of x and y angle.
         */
        Joystick* joystick;
        /**
         * Internal button object to keep track of button press. (with debouncing)
         */
        Button* button;
        /**
         * Handler for left joystick motion.
         */ 
        void(*move_left)();
        /**
         * Handler for right joystick motion.
         */ 
        void(*move_right)();
};

#endif