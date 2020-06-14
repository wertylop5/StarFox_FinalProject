# CS 3420/ECE 3140 Final Project: Star Fox
### Created by: Stanley Lin, Rodrigo Taipe, Michael Zhou

This is a top down pixel shooter inspired by the game Star Fox.

More extensive documentation can be found [here](https://docs.google.com/document/d/15hE4Jnv0u663gtfDqWQkiszYQpph_0zsd1vFm9mV_4g/edit?usp=sharing)

## Installation
In terms of hardware, we used:
* FRDM K64F board
* 8x8 LED matrix
* MAX7219 driver
* joystick
* pushbutton
* 4 digit seven segment display

You will need to install mbed middleware for this project. Our group utilized mbed-cli. There are two ways to install it:
* Instructions from here: https://os.mbed.com/docs/mbed-os/v5.15/quick-start/offline-with-mbed-cli.html
* Install mbed-cli from python's pip package manager. Note that our group used python 3.7 when installing through pip.

You'll also need to install a few libraries. Go into the project root and run the following commands:
* `mbed add mbed-os`
* `mbed add http://os.mbed.com/users/eencae/code/Joystick/`
* `mbed add http://os.mbed.com/users/morecat_lab/code/sseg/`
	
## Compiling and Running
Run `mbed compile --target K64F --toolchain GCC_ARM --flash` (Note that `GCC_ARM` can be replaced with some other toolchain if necessary)

This project was compiled using compiler version 6.3.1 and mbed-cli version 1.10.3 on python 3.7
