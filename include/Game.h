#ifndef GAME_H
#define GAME_H

#include <vector>
#include "mbed.h"

#include "Missile.h"
#include "Obstacle.h"
#include "Projectile.h"

class Player;

class Game {
private:
	static const int MAX_OBSTACLES_PER_SPAWN{ 3 };
	static const int NUM_ROWS{ 8 };
	static const int NUM_COLS{ 8 };
	
	//keeps track of what numbers on the board mean what
	enum class BoardToken {
		empty,
		player,
		obstacle,
		missile
	};
	
	int board[8][8];
	Player& player;
	std::vector<Projectile*> projectiles;
	
public:
	Game(Player& p): board{}, player{ p } {};
	
	~Game();
	
	void init();
	void loop();
	
	void printBoard();
	
	/*
	Spawns some obstacles at the top of the grid
	*/
	void spawnObstacles();
	
	/*
	Adjust player position so that they are within board bounds
	*/
	void adjustPlayerBound();
};

#include "Player.h"

#endif

//periodic interrupt for clock tick: use LowPowerTicker
/*
speed valueS:
1: every tick
4: every 4 ticks

everything 1 pixel
collide once, game over

queue up instructions before the next clock tick
*/