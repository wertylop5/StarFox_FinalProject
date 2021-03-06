#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <unordered_map>
#include "mbed.h"

#include "Missile.h"
#include "Obstacle.h"
#include "Projectile.h"
#include "BossPlayer.h"

#include "mbed_mem_trace.h"
void print_memory_info();

class Player;
class Missile;
class Obstacle;

class Game {
private:
	static const int MAX_OBSTACLES_PER_SPAWN{ 1 };
	static const int MISSILE_BUFFER_SIZE{ 30 };
	
	//chance out of 100 for a critical missile (hits 2 obstacles)
	static const int CRIT_MISSILE_CHANCE{ 20 };
	
	//boss will spawn after score % BOSS_SPAWN_CONDITION == 0, score != 0
	static const int BOSS_SPAWN_CONDITION{ 10 };
	
	//chance out of 100 for obstacles to spawn on a row
	static const int OBSTACLE_SPAWN_CHANCE{ 70 };
	
	//keeps track of what numbers on the board mean what
	enum class BoardToken {
		empty,
		player,
		obstacle,
		missile,
		boss
	};
	
	int board[8][8];
	Player& player;
	BossPlayer* boss;
	std::vector<Missile*> missiles;
	std::vector<Obstacle*> obstacles;
	
	/*
	stores the location of missiles to be spawned
	column 0 is x, column 1 is y
	*/
	int missileBuffer[MISSILE_BUFFER_SIZE][2];
	
	//stores next available position in missile buffer
	int missileBufferPos;
	
	//whether game should end
	bool endGameFlag;
	
	//controls the boss level
	bool bossSpawnFlag;
	bool bossDestroyedFlag;
	
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
	
	//keeps player and boss within bounds
	void adjustShipBounds();
	
	bool isInBounds(Entity *e);
	bool isInBounds(int x, int y);
	
	//decrease counter for next entity refresh
	void decrementCounters();
	
	//moves projectiles if they're ready
	void moveProjectiles();
	
	void moveMissiles();
	void moveObstacles();
	
	/*
	if missile requests exists, calls the Player.shoot() function and keeps track of
	the new missile
	*/
	void addMissiles();
	
	//helper function to write to the missile buffer
	void addToMissileBuffer(int x, int y);
	
	/*
	runs checks on all projectiles to see if any have collided
	*/
	void checkProjectileCollision();
	void removeOutOfBoundsProjectiles();
	void checkPlayerCollision(Player* p);
	
	//check collisions for player and boss
	void checkShipCollisions();
	
	void spawnBoss();
	
	//removes the boss ship
	void destroyBoss();
	
	//moves the boss and has it shoot if it should
	void updateBoss();
	
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
	
	/**
	keeps track of the current score
	*/
	int score;
	
	/**
	p is a Player object for the game to keep track of
	*/
	Game(Player& p): board{ }, player{ p }, boss{ 0 }, missileBuffer{ }, missileBufferPos{ 0 },
		endGameFlag{ false }, bossSpawnFlag{ false }, bossDestroyedFlag{ false },
		clampedBoard{ }, score{ 0 } {}
	
	~Game();
	
	/**
	seed_in is a floating analog pin to be used to seed the
	random number generator
	*/
	void init(PinName seed_pin);
	
	/**
	update all game variables
	
	returns whether or not the game should continue running
	*/
	bool loop();
	
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
	notifies the game to create a missile on the next refresh
	*/
	void handleShoot();
	
	/**
	handles the super move, which spawns missiles along a whole row
	
	super moves can only be used once the score has reached a certain threshold
	(currently, when it is >= BOSS_SPAWN_CONDITION). score is then decreased by
	that amount
	*/
	void handleSuper();
	
	/**
	spawns a bunch of missiles along a whole row
	*/
	void spawnMissiles();
};

#endif
