#include "mbed.h"
#include "include/Game.h"
#include "Joystick.h"
#include "include/hardware/LEDMatrix.h"
#include "include/hardware/Shooter.h"

/**
TODO: change obstacle spawn mechanics to choose a random column
*/

/*
define this to enable debug print statements and disable LED matrix printing

comment out to disable debug print statements and enable LED matrix printing
*/
// #define GAME_DEBUG 

//change this to control the game's speed
const int FPS = 30;

const int SECOND = 1000;
const int REFRESH_TIME = static_cast<int>((1/FPS)*SECOND);

Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1, 3);
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

int main() {
	printf("~~~~~~~~~~~~~~~~~~~PROGRAM START~~~~~~~~~~~~~~~~~~~\n");
	
	#ifndef GAME_DEBUG
	LEDMatrix::create_LEDMatrix(PTD2, PTD0, PTD1);
	Shooter::create_shooter(PTC3, on_shoot);
	Joystick joystick(PTB3,PTB2,PTB10); // note PTB10 not used
    joystick.init();
	#endif
	
	g.init(PTB10);
	//tester.attach(&interruptTest, 1.0f);
	
	while(1) {
		Direction d = joystick.get_direction();
		// if d is NE, E, or SE (2, 3, or 4)
		if(d >= 2 && d <= 4){
			on_joystick_right();
		}
		// if d is SW, W, or NW (6, 7, or 8)
		else if(d >= 6 && d <= 8){
			on_joystick_left();
		}

		if (!g.loop()) {
			tester.detach();
			
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
	
	
	
	printf("Game over!\n");
	
	return 0;
}