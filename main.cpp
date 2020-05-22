#include "mbed.h"
#include "include/Game.h"
#include "Joystick.h"
#include "include/hardware/LEDMatrix.h"
#include "include/hardware/Shooter.h"

/*
define this to enable debug print statements and disable LED matrix printing

comment out to disable debug print statements and enable LED matrix printing
*/
// #define GAME_DEBUG 

//change this to control the game's speed
const int FPS = 30;

const int SECOND = 1000;
const int REFRESH_TIME = static_cast<int>((1/FPS)*SECOND);

//Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1, 3);
Player p(Game::NUM_ROWS-1, 7, 1, 1);
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
	Joystick joystick(PTB2, PTB3, PTB11); // note PTB11 not used
    joystick.init();
	#endif
	
	g.init(PTB10);
	//tester.attach(&interruptTest, 1.0f);
	
	while(1) {
		// x idrectoin rangs from 0 to 0.8
		float x = joystick.GetXValue();
		if(x <= 0.1){
			on_joystick_left();
		}
		else if(x >= 0.7){
			on_joystick_right();
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