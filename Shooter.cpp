#include "include/hardware/Shooter.h"
#include "mbed.h"

int Shooter::count = 0;
int Shooter::time = 0;
int Shooter::prev_time = 0;
Ticker Shooter::ticker;
InterruptIn* Shooter::button;
void (*Shooter::shoot)();

Shooter::Shooter(){
    printf("Shooter constructor should not be used.\r\n");
    printf("Use ShootingButton::create_shooter.\r\n");
}

void Shooter::create_shooter(PinName button_pin, void (*shoot)()){
    if(count == 0){
        Shooter::count++;
        Shooter::ticker.attach_us(Shooter::ticker_handler, 1000);
        Shooter::shoot = shoot;
        Shooter::button = new InterruptIn(button_pin);
        Shooter::button->rise(&Shooter::button_handler);
    }
}

void Shooter::ticker_handler(){
    Shooter::time++;
}

void Shooter::button_handler(){
    if(Shooter::time - Shooter::prev_time >= DEBOUNCE_DELAY){
        Shooter::shoot();
        Shooter::prev_time = Shooter::time;
    }
}