#include "mbed.h"
#include "include/Game.h"

int main() {
	printf("~~~~~~~~~~~~~~~~~~~PROGRAM START~~~~~~~~~~~~~~~~~~~\n");
	
	DigitalOut out(LED_GREEN);
	//AnalogIn seed(PTB2);
	
	Serial pc(USBTX, USBRX);
	
	Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1);
	Game g(p);
	
	g.init(PTB10);
	
	g.spawnMissiles();
	for (int x = 0; x < 7; x++) {
		g.loop();
		g.printBoard();
	}
	
	g.loop();
	g.printBoard();
	g.loop();
	g.printBoard();
	g.loop();
	g.printBoard();
	
	while (1) {
		out = !out;
		
		ThisThread::sleep_for(500);
	}
	
	return 0;
}
