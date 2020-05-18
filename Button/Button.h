#ifndef BUTTON_H
#define BUTTON_H

#include "mbed.h"

// this value can be tuned to alter delay between actual button clicks
#define DEBOUNCE_DELAY 15


/** Button Class
@author Rodrigo Taipe
@brief  Library for interfacing with button

Example:

@code

#include "mbed.h"
#include "Joystick.h"

Button button(PTD0);
DigitalIn led(LED1)

void toggleLED(){
    led = !led;
    printf("LED Toggled\r\n");
}

int main() {
    // set interrupt handler
    button.on_rise(toggleLED);
    while(1){
        // do stuff
        printf("Button State: %d\r\n", button.button_pressed());
    }
}

* @endcode
*/
class Button
{
public:
    /**
     * Constructor to create a debounced button/
     * @param button_pin Digital pin which button is hooked up to.
     */
    Button(PinName button_pin);
    /**
     * Sets handler on rising edge of button click.
     * 
     * @param on_rise_handler The handler to execute when button is pressed.
     * Note: No need to debounce inside handler (We do this for you.)
     */
    void on_rise(void (*on_rise_handler)());    // Sets handler on rising edge of button click.
    /**
     * Returns state of button.
     * 
     * @return True if button is pressed and false otherwise.
     */
    bool button_pressed();
private:
    InterruptIn *click;
    bool click_flag;                            // Flag set in ISR.
    void (*on_rise_handler)();                  // Function to run inside ISR when button on rising edge.
    /**
     * ISR on button rise. (Includes debouncing and sets click_flag to true.)
     */
    void rise_ISR();
    /**
     * ISR on button rise. (Sets click_flag to false.)
     */
    void fall_ISR();
    /**
     * ISR on timer. (Increment currentTime by one every second.)
     */
};


#endif