#ifndef LEDMATRIX_H
#define LEDMATRIX_H
#include "mbed.h"

// Adjust the clock freq as neccessary.
#define CLOCK_FREQUENCY 1000000

/**
#include "mbed.h"
#include "include/Game.h"
#include "include/hardware/LEDMatrix.h"

int board[8][8] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 1, 1},
	{1, 0, 1, 0, 0, 1, 0, 1},
	{1, 0, 0, 1, 1, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
}; //displays smiley face

int main() {
	LEDMatrix::create_LEDMatrix(PTD2, PTD0, PTD1);
    LEDMatrix::display(board);

    while(1){
        //do stuff
    }
*/

class LEDMatrix{
    public:
        /**
         * Creates an interface for 8 by 8 LED matrix with MAX7219 driver.
         * @param mosi Pin name for Master-Out Slave-In pin on board
         * and connects to DIN on MAX7219.
         * @param cs Pin name for Chip Select Pin on board and connects
         * to CS on MAX7219.
         * @param clk Pin nam for Serial Clock on board and connects
         * to CLK on MAX7219.
         */
        static void create_LEDMatrix(PinName mosi, PinName cs, PinName clk);
        /** Displays the current state of board.
         * @param board A 2D array of ints containing 0 if LED is off and 1 if on.
         */
        static void display(int board[8][8]);
        /** Displays an empty board state.
         */
        static void reset();
    private:
        /**
         * We implement the singleton pattern, so we hide the constructor.
         */
        LEDMatrix();
        /**
         * Serial Peripheral Interface for MAX7219 (LED matrix driver).
         */
        static SPI* driver_spi;
        /**
         * Clock select as a digital output for MAX7219 (LED matrix driver).
         */
        static DigitalOut* driver_cs;
        static void SPI_Write2(unsigned char MSB, unsigned char LSB);
        static void Init_MAX7219();
        static void encodeArray(unsigned char* output, int board[8][8]);
};

#endif