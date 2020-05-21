#include "mbed.h"
#include "include/Game.h"
#include "include/hardware/LEDMatrix.h"

/**
TODO: figure a different way to spawn missiles (can't do inside an interrupt)
TODO: change obstacle spawn mechanics to choose a random column
*/

/*
define this to enable debug print statements and disable LED matrix printing

comment out to disable debug print statements and enable LED matrix printing
*/
#define GAME_DEBUG

//change this to control the game's speed
const int FPS = 60;

const int SECOND = 1000;
const int REFRESH_TIME = static_cast<int>((1/FPS)*SECOND);

int main() {
	printf("~~~~~~~~~~~~~~~~~~~PROGRAM START~~~~~~~~~~~~~~~~~~~\n");
	
	#ifndef GAME_DEBUG
	LEDMatrix::create_LEDMatrix(PTD2, PTD0, PTD1);
	#endif
	
	Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1);
	Game g(p);
	g.init(PTB10);
	
	while(1) {
		if (!g.loop()) { break; }
		
		g.clampBoard();
		
		#ifndef GAME_DEBUG
		LEDMatrix::display(g.clampedBoard);
		#else
		g.printBoard();
		#endif
		
		ThisThread::sleep_for(REFRESH_TIME);
	}
	
	printf("Game over!\n");
	
	return 0;
}
