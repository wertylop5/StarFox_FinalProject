/*
define this to enable debug print statements and disable hardware usage

comment out to disable debug print statements and enable hardware usage
*/
//#define GAME_DEBUG 

#include "mbed.h"
#include "include/Game.h"
#include "include/hardware/LEDMatrix.h"
#include "include/hardware/Button.h"
#include "include/hardware/LeftRightControlStick.h"
#include "include/hardware/SevenSegmentDisplay.h"


//change this to control the game's speed
const int FPS = 15;

const int SECOND = 1000;
const int REFRESH_TIME = static_cast<int>((1/FPS)*SECOND);

Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1, 100);
Game g(p);
LowPowerTicker tester;

void on_shoot() {
	g.handleShoot();
}

void on_joystick_left(){
	p.moveLeft();
}

void on_joystick_right(){
	p.moveRight();
}

void on_super_shoot() {
	g.handleSuper();
}

int main() {
	printf("~~~~~~~~~~~~~~~~~~~PROGRAM START~~~~~~~~~~~~~~~~~~~\n");
	
	#ifndef GAME_DEBUG
	LEDMatrix::create_LEDMatrix(PTD2, PTD0, PTD1);
	Button shooter(PTB19, on_shoot);
	LeftRightControlStick lrStick(
		PTB2, PTB3, PTD3, on_joystick_left, 
		on_joystick_right, on_super_shoot
	);
	SevenSegmentDisplay scoreDisplay(
		PTA1, PTC3, PTC0, PTC8, PTC1, PTB23, PTC7, PTC9,
		PTB9, PTA2, PTC2, PTC5, &g.score
	);
	#endif
	
	g.init(PTB10);
	//tester.attach(&interruptTest, 1.0f);
	
	while(1) {
		if (!g.loop()) {
			tester.detach();
			
			break;
		}
		
		g.clampBoard();
		
		#ifndef GAME_DEBUG
		lrstick.checkJoystick();
		LEDMatrix::display(g.clampedBoard);
		#else
		g.printBoard();
		#endif
		
		ThisThread::sleep_for(REFRESH_TIME);
	}
	
	printf("Game over!\n");
	
	return 0;
}