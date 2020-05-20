#include "mbed.h"
#include "include/hardware/LEDMatrix.h"

/** Note: The SPI code (specifically SPI_Write2 and Init_MAX7219) is from the example on the following link: 
 * https://os.mbed.com/teams/Megtestesules-Plebania-hobbielektronika-/code/06_spi_max7219_led8x8/file/bd34a367f642/main.cpp/
 */

/// Send two bytes to SPI bus
void LEDMatrix::SPI_Write2(unsigned char MSB, unsigned char LSB)
{
    LEDMatrix::driver_cs->write(0);
    LEDMatrix::driver_spi->write(MSB);                 // Send two bytes
    LEDMatrix::driver_spi->write(LSB);
    LEDMatrix::driver_cs->write(1);
}
 
/// MAX7219 initialisation
void LEDMatrix::Init_MAX7219()
{
    LEDMatrix::SPI_Write2(0x09, 0x00);         // Decoding off
    LEDMatrix::SPI_Write2(0x0A, 0x08);         // Brightness to intermediate
    LEDMatrix::SPI_Write2(0x0B, 0x07);         // Scan limit = 7
    LEDMatrix::SPI_Write2(0x0C, 0x01);         // Normal operation mode
    LEDMatrix::SPI_Write2(0x0F, 0x0F);         // Enable display test
    wait_ms(500);                   // 500 ms delay
    LEDMatrix::SPI_Write2(0x01, 0x00);         // Clear row 0.
    LEDMatrix::SPI_Write2(0x02, 0x00);         // Clear row 1.
    LEDMatrix::SPI_Write2(0x03, 0x00);         // Clear row 2.
    LEDMatrix::SPI_Write2(0x04, 0x00);         // Clear row 3.
    LEDMatrix::SPI_Write2(0x05, 0x00);         // Clear row 4.
    LEDMatrix::SPI_Write2(0x06, 0x00);         // Clear row 5.
    LEDMatrix::SPI_Write2(0x07, 0x00);         // Clear row 6.
    LEDMatrix::SPI_Write2(0x08, 0x00);         // Clear row 7.
    LEDMatrix::SPI_Write2(0x0F, 0x00);         // Disable display test
    wait_ms(500);                   // 500 ms delay
}
 
SPI* LEDMatrix::driver_spi;
DigitalOut* LEDMatrix::driver_cs;

void LEDMatrix::create_LEDMatrix(PinName mosi, PinName cs, PinName clk){
    LEDMatrix::driver_spi = new SPI(mosi, PTD3, clk);
    LEDMatrix::driver_cs = new DigitalOut(cs, 1);
    LEDMatrix::driver_spi->format(8, 0);
    LEDMatrix::driver_spi->frequency(CLOCK_FREQUENCY);
    LEDMatrix::Init_MAX7219();
}

LEDMatrix::LEDMatrix(){
    printf("LEDMatrix constructor should not be used.\r\n");
    printf("Use LEDMatrix::create_LEDMatrix.\r\n");
}

void LEDMatrix::convertToHexArray(unsigned char* output, int board[8][8]){
    for(int i=0; i < 8; i++){
        for(int j=0; j < 8; j++){
            if(board[i][j] >= 1){
                output[i] |= 1 << j;
            }
            else{
                output[i] &= ~(1 << j);
            }
        }
    }
}

void LEDMatrix::display(int board[8][8]){
    // unsigned char led1[]= {
    //     0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0xFF
    // };  //H
    unsigned char led1[8];
    LEDMatrix::convertToHexArray(led1, board);
    for(int i=1; i<9; i++){      // Write first character (8 rows)
        LEDMatrix::SPI_Write2(i,led1[i-1]);
    }
}