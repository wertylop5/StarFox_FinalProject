/*
define this to enable debug print statements and disable hardware usage

comment out to disable debug print statements and enable hardware usage
*/
//#define GAME_DEBUG 

#include "mbed.h"
#include "include/Game.h"
#include "Sseg.h"
#include "include/hardware/LEDMatrix.h"
#include "include/hardware/Button.h"
#include "Joystick.h"


//change this to control the game's speed
const int FPS = 15;

const int SECOND = 1000;
const int REFRESH_TIME = static_cast<int>((1/FPS)*SECOND);

Player p(Game::NUM_ROWS-1, Game::NUM_COLS/2, 1, 100);
//Player p(Game::NUM_ROWS-1, 7, 1, 1);
Game g(p);
LowPowerTicker tester;

Sseg score(PTA1, PTC3, PTC0, PTC8,
		PTC1, PTB23, PTC7, PTC9,
		PTB9, PTA2, PTC2, PTC5);
Thread scoreThread;

void displayScore(){
	while(1){
		score.writeNum4(g.score);
		score.updateOnce();
	}
}

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
	Button supershooter(PTD3, on_super_shoot);
	Joystick joystick(PTB2, PTB3, PTB11); // note PTB11 not used
    joystick.init();
	score.begin();
	score.setKcommon();
	#endif
	
	g.init(PTB10);
	scoreThread.start(displayScore);
	//tester.attach(&interruptTest, 1.0f);
	
	while(1) {
		// x idrectoin rangs from 0 to 0.8
		
		#ifndef GAME_DEBUG
		float x = joystick.GetXValue();
		if(x <= 0.1){
			on_joystick_left();
		}
		else if(x >= 0.7){
			on_joystick_right();
		}
		#endif

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