#ifndef PLAYER_H
#define PLAYER_H

#include "Entity.h"

class Missile;

class Player: public Entity {
public:
	Player(int x, int y, int s): Entity(x, y, s) {};
	
	/**
	increments/decrements posy
	*/
	void moveRight() { posy += 1; }
	void moveLeft() { posy -= 1; }
	
	/**
	spawns a new missile
	*/
	Missile* shoot();
	
	friend void Game::adjustPlayerBound();
};

#endif