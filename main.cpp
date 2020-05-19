#include "mbed.h"
#include "include/Game.h"


int main() {
	DigitalOut out(LED_GREEN);
	//AnalogIn seed(PTB2);
	
	Serial pc(USBTX, USBRX);
	
	Player p(0, 0, 1);
	Game g(p);
	
	g.init(PTB10);
	
	g.spawnObstacles();
	g.spawnMissiles();
	g.printBoard();
	
	g.loop();
	g.printBoard();
	
	while (1) {
		out = !out;
		
		ThisThread::sleep_for(500);
	}
	
	return 0;
}
