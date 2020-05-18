#include "Button.h"
#include "mbed.h"

Ticker millis;
volatile unsigned long current_time = 0;
volatile unsigned long last_debounce_time;
void time_ISR(){
    current_time++;
}

Button::Button(PinName button_pin)
{
    millis.attach_us(time_ISR, 1000); // run every 1000 us = 1 ms
    click = new InterruptIn(button_pin);
    click_flag = false;
}

void Button::on_rise(void (*on_rise_handler)())
{
    this->on_rise_handler = on_rise_handler;
}

bool Button::button_pressed()
{
    return click_flag;
}

void Button::rise_ISR(){
    if(current_time - last_debounce_time >= DEBOUNCE_DELAY){
        on_rise_handler();
        last_debounce_time = current_time;
        click_flag = true;
    }
}

void Button::fall_ISR(){
    if(click_flag){
        click_flag = false;
    }
}