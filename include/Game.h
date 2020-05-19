#ifndef GAME_H
#define GAME_H

#include <vector>
#include "mbed.h"

#include "Missile.h"
#include "Obstacle.h"
#include "Projectile.h"

class Player;
class Missile;
class Obstacle;

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
	std::vector<Missile*> missiles;
	std::vector<Obstacle*> obstacles;
	
public:
	Game(Player& p): board{}, player{ p } {};
	
	~Game();
	
	/**
	seed_in is a floating analog pin to be used to seed the
	random number generator
	*/
	void init(PinName seed_pin);
	
	/**
	update all game variables
	*/
	void loop();
	
	
	void clearBoard();
	void printBoard();
	
	/**
	writes a value representing an object onto the board
	*/
	void placeToken(int x, int y, BoardToken token);
	
	/**
	Spawns some obstacles at the top of the grid
	*/
	void spawnObstacles();
	
	/**
	checks if projectiles are on same spot
	*/
	bool hasCollided(Projectile* a, Projectile* b);
	
	/**
	calls the Player.shoot() function and keeps track of the new missile
	*/
	void handleShoot();
	
	/**
	test function, spawns a bunch of missiles
	*/
	void spawnMissiles();
	
	/**
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