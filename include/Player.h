#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Entity.h"

class Missile;

class Player: public Entity {
private:
	int health;

public:
	static const std::string LABEL;
	
	//pixels in the hitbox
	static const int HITBOX_SIZE = 4;
	
	//hitbox points relative to actual center position
	const int hitbox[4][2];
	
	Player(int x, int y, int s, int h): Entity(x, y, s), health{ h },
		hitbox{ {0, 0}, {-1, 0}, {0, -1}, {0, 1} } {};
	
	Player(int x, int y, int s): Player(x, y, s, 3) {};
	
	/**
	increments/decrements posy
	*/
	void moveRight() { posy += 1; }
	void moveLeft() { posy -= 1; }
	
	/**
	spawns a new missile
	*/
	Missile* shoot();
	
	void lowerHealth() { health--; }
	void increaseHealth() { health++; }
	
	bool isAlive() { return health > 0; }
	
	friend void Game::adjustPlayerBound();
};

#endif
