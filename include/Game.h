#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <unordered_map>
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
	bool endGameFlag;
	
	/*
	keeps track of where each class is in its refresh cycle
	*/
	std::unordered_map<std::string, int> refreshCounters;
	
	/*
	sets all board positions to BoardToken::empty
	*/
	void clearBoard();
	
	/*
	places all appropriate tokens on the board. should be called last in loop()
	*/
	void updateBoard();
	
	/*
	writes a value representing an object onto the board
	*/
	void placeToken(int x, int y, BoardToken token);
	
	/*
	checks if entities are on same spot
	*/
	bool hasCollided(Entity* a, Entity* b);
	
	/*
	checks if a missile skipped over an obstacle (ie, row of missile < row of obstacle)
	*/
	bool isObstacleBehind(Missile* m, Obstacle* o);
	
	bool isInBounds(Entity *e);
	
	void decrementCounters();
	
	void moveMissiles();
	void moveObstacles();
	
	/*
	runs checks on all projectiles to see if any have collided
	*/
	void checkProjectileCollision();
	void removeOutOfBoundsProjectiles();
	void checkPlayerCollision();
	
public:
	static const int NUM_ROWS{ 8 };
	static const int NUM_COLS{ 8 };
	
	/**
	stores how many cycles each game element should take to update and/or move
	*/
	std::unordered_map<std::string, const int> refreshSpeeds;
	
	/**
	a version of the internal game board with values clamped to 0 and 1
	*/
	int clampedBoard[8][8];
	
	Game(Player& p): board{}, player{ p }, endGameFlag{ false } {};
	
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
	sets clampedBoard to the internal board values clamped to 0 and 1
	*/
	void clampBoard();
	
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

/*
speed valueS:
1: every tick
4: every 4 ticks

everything 1 pixel
collide once, game over

two threads: game and graphics update
use mutex and condition variables
*/
