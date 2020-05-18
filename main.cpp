#include "mbed.h"
#include "include/Game.h"


int main() {
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
