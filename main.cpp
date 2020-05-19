#include "mbed.h"
#include "include/Game.h"

int main() {
	printf("~~~~~~~~~~~~~~~~~~~PROGRAM START~~~~~~~~~~~~~~~~~~~\n");
	print_memory_info();
	
	DigitalOut out(LED_GREEN);
	//AnalogIn seed(PTB2);
	
	Serial pc(USBTX, USBRX);
	
	Player p(Game::NUM_ROWS, Game::NUM_COLS/2, 1);
	Game g(p);
	
	g.init(PTB10);
	
	g.spawnMissiles();
	
	print_memory_info();
	
	g.spawnObstacles();
	g.loop();
	
	print_memory_info();
	
	//crashing here
	g.spawnObstacles();
	/*
	g.loop();
	g.loop();
	g.printBoard();
	
	g.loop();
	g.printBoard();
	g.loop();
	g.printBoard();
	*/
	while (1) {
		out = !out;
		
		ThisThread::sleep_for(500);
	}
	
	return 0;
}
