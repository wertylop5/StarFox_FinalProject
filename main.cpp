#include "mbed.h"
#include "include/hardware/LEDMatrix.h"

const unsigned char led1[]= {
    0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0xFF
};  //H
const unsigned char led2[]= {
    0x1F,0x60,0x80,0x40,0x40,0x80,0x60,0x1F
};  //W
 
int main()
{
	LEDMatrix::create_LEDMatrix(PTD2, PTD0, PTD1);
	LEDMatrix::display(NULL);
	while(1){

	}
	return 0;
}