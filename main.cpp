#include "mbed.h"
#include "include/Game.h"
#include "include/hardware/Shooter.h"

DigitalOut led(LED1);
void toggle(){
	led = !led;
}

int main() {
	Shooter::create_shooter(PTC3, toggle);
	
	DigitalOut out(LED_GREEN);
	
	Serial pc(USBTX, USBRX);
	
	Player p(0, 0, 1);
	Game g(p);
	
	g.init();
	g.printBoard();
	
	g.spawnObstacles();
	g.printBoard();
	
	while (1) {
		out = !out;
		
		ThisThread::sleep_for(500);
	}
	
	return 0;
}
