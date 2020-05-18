#include "mbed.h"
#include "include/Game.h"


Serial pc(USBTX, USBRX); // tx, rx

//Joystick joystick(PTB2,PTB3,PTC11);
                    //x  y
Joystick joystick(PTB2, PTB3, PTC0);
DigitalOut r_led(LED1);
InterruptIn button(PTD1);
Button b = Button(PTD0);

Ticker ms_tick;
volatile unsigned long currentTime = 0;
volatile unsigned long lastDebounceTime;
volatile unsigned long debounceDelay = 15;

// not using ptd3
SPI spi(PTD2, PTD3, PTD1);          // Arduino compatible MOSI, MISO, SCLK
DigitalOut cs(PTD4);                // Chip select
 
const unsigned char led1[]= {
    0xFF,0x18,0x18,0x18,0x18,0x18,0x18,0xFF
};  //H
const unsigned char led2[]= {
    0x1F,0x60,0x80,0x40,0x40,0x80,0x60,0x1F
};  //W

void onMillisecondTicker(){
    currentTime++;
}

/// Send two bytes to SPI bus
void SPI_Write2(unsigned char MSB, unsigned char LSB)
{
    cs = 0;                         // Set CS Low
    spi.write(MSB);                 // Send two bytes
    spi.write(LSB);
    cs = 1;                         // Set CS High
}
 
/// MAX7219 initialisation
void Init_MAX7219(void)
{
    SPI_Write2(0x09, 0x00);         // Decoding off
    SPI_Write2(0x0A, 0x08);         // Brightness to intermediate
    SPI_Write2(0x0B, 0x07);         // Scan limit = 7
    SPI_Write2(0x0C, 0x01);         // Normal operation mode
    SPI_Write2(0x0F, 0x0F);         // Enable display test
    ThisThread::sleep_for(500);     // 500 ms delay
    SPI_Write2(0x01, 0x00);         // Clear row 0.
    SPI_Write2(0x02, 0x00);         // Clear row 1.
    SPI_Write2(0x03, 0x00);         // Clear row 2.
    SPI_Write2(0x04, 0x00);         // Clear row 3.
    SPI_Write2(0x05, 0x00);         // Clear row 4.
    SPI_Write2(0x06, 0x00);         // Clear row 5.
    SPI_Write2(0x07, 0x00);         // Clear row 6.
    SPI_Write2(0x08, 0x00);         // Clear row 7.
    SPI_Write2(0x0F, 0x00);         // Disable display test
    ThisThread::sleep_for(500);
}

void button_interrupt(){
    if(currentTime - lastDebounceTime >= debounceDelay){
        r_led = !r_led;
        lastDebounceTime=currentTime;
    }
}

void toggle(){
    r_led = !r_led;
}
 
int main()
{
    // cs = 1;                         // CS initially High
    // spi.format(8,0);                // 8-bit format, mode 0,0
    // spi.frequency(1000000);         // SCLK = 1 MHz
    // Init_MAX7219();                 // Initialize the LED controller
    // while (1) {
    //     for(int i=1; i<9; i++)      // Write first character (8 rows)
    //         SPI_Write2(i,led1[i-1]);
    //     ThisThread::sleep_for(BLINKING_RATE_MS);
    //     for(int i=1; i<9; i++)      // Write second character
    //         SPI_Write2(i,led2[i-1]);
    //     ThisThread::sleep_for(BLINKING_RATE_MS);
    // }
    
    // joystick.init();
 
    // while(1) {
    //     printf("digin: %d\r\n", button.read());
    
    //     Vector2D coord = joystick.get_coord();
    //     printf("Coord = %f,%f\r\n",coord.x,coord.y);
        
    //     Vector2D mapped_coord = joystick.get_mapped_coord(); 
    //     printf("Mapped coord = %f,%f\r\n",mapped_coord.x,mapped_coord.y); 
        
    //     float mag = joystick.get_mag();
    //     float angle = joystick.get_angle();
    //     printf("Mag = %f Angle = %f\r\n",mag,angle);
        
    //     Direction d = joystick.get_direction();
    //     printf("Direction = %i\r\n",d);
        
    //     if (joystick.button_pressed() ) {
    //         printf("Button Pressed\r\n");  
    //     }
          
    //     ThisThread::sleep_for(500);
    // }
    // button.rise(&button_interrupt);
    // ms_tick.attach_us(onMillisecondTicker, 1000);
    b.on_rise(toggle);
    
    while(1){
        // do stuff
    }
    // ms_tick.detach();
}
