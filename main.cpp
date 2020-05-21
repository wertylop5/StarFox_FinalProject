#include "mbed.h"
#include "include/Game.h"
#include "include/hardware/LEDMatrix.h"

//#define GAME_DEBUG

const int SECOND = 1000;
const int FPS = 60;
const int REFRESH_TIME = static_cast<int>((1/FPS)*SECOND);

//Mutex loop_lock;

/*
void refreshLEDMatrix(Game& g) {
	while(1) {
		loop_lock.lock();
		
		g.clampBoard();
		LEDMatrix::display(g.clampedBoard);
		
		loop_lock.unlock();
		ThisThread::sleep_for(REFRESH_TIME);
	}
}
*/

int main() {
	printf("~~~~~~~~~~~~~~~~~~~PROGRAM START~~~~~~~~~~~~~~~~~~~\n");
	
	#ifndef GAME_DEBUG
	LEDMatrix::createLEDMatrix(PTD2, PTD0, PTD1);
	#endif
	
	Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1);
	Game g(p);
	g.init(PTB10);
	
	//Thread matrixUpdater;
	//matrixUpdater.start(callback(g, refreshLEDMatrix));
	
	while(1) {
		if (!g.loop()) {
			//matrixUpdater.terminate();
			
			//loop_lock.unlock();
			break;
		}
		
		g.clampBoard();
		
		#ifndef GAME_DEBUG
		LEDMatrix::display(g.clampedBoard);
		#else
		g.printBoard();
		#endif
		
		ThisThread::sleep_for(REFRESH_TIME);
	}
	
	return 0;
}