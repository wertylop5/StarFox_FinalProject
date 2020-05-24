#include "LeftRightControlStick.h"

LeftRightControlStick::LeftRightControlStick(
    PinName xAnglePin, PinName yAnglePin, PinName buttonPin, 
    void(*move_left)(), void(*move_right)(), void(*press)()
){
    joystick = new Joystick(xAnglePin, yAnglePin, PTB11); // note PTB11 not used
    joystick->init();
    button = new Button(buttonPin, press);
    this->move_left = move_left;
    this->move_right = move_right;
}

void LeftRightControlStick::checkJoystick(){
    float xAngle = joystick->GetXValue();
    if(xAngle <= MIN_NEUTRAL){
        move_left();
    }
    else if(xAngle >= MAX_NEUTRAL){
        move_right();
    }
}