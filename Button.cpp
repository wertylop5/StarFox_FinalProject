#include "include/hardware/Button.h"
#include "mbed.h"

Button::Button(PinName button_pin, void(*shoot)()){
    //use timer for debouncing
    time = 0;
    prev_time = 0;
    ticker.attach_us(callback(this, &Button::ticker_handler), 1000);
    this->shoot = shoot;

    // set button pin interrupts
    button = new InterruptIn(button_pin);
    button->rise(callback(this, &Button::button_rise_handler));
    button->fall(callback(this, &Button::button_fall_handler));
    is_clicked = false;
}

void Button::ticker_handler(){
    time++;
}

void Button::button_rise_handler(){
    if(time - prev_time >= DEBOUNCE_DELAY){
        shoot();
        prev_time = time;
        is_clicked = true;
    }
}

void Button::button_fall_handler(){
    is_clicked = false;
}

bool Button::is_shooting(){
    return is_clicked;
}