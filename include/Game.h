#ifndef GAME_H
#define GAME_H

#include <vector>
#include "mbed.h"

#include "Missile.h"
#include "Obstacle.h"
#include "Projectile.h"


#include "mbed_mem_trace.h"
void print_memory_info();


class Player;
class Missile;
class Obstacle;

class Game {
private:
	static const int MAX_OBSTACLES_PER_SPAWN{ 3 };
	
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
	
	/**
	sets all board positions to BoardToken::empty
	*/
	void clearBoard();
	
	/**
	places all appropriate tokens on the board. should be called last in loop()
	*/
	void updateBoard();
	
	/**
	writes a value representing an object onto the board
	*/
	void placeToken(int x, int y, BoardToken token);
	
	/**
	checks if projectiles are on same spot
	*/
	bool hasCollided(Projectile* a, Projectile* b);
	
	/**
	checks if a missile skipped over an obstacle (ie, row of missile < row of obstacle)
	*/
	bool isObstacleBehind(Missile* m, Obstacle* o);
	
	bool isInBounds(Entity *e);
	
	void removeOutOfBoundsEntities();
	
public:
	static const int NUM_ROWS{ 8 };
	static const int NUM_COLS{ 8 };
	
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
	
	/**
	prints the current state of the board to console
	*/
	void printBoard();
	
	/**
	Spawns some obstacles at the top of the grid
	*/
	void spawnObstacles();
	
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