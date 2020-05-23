#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Entity.h"

class Missile;

class Player: public Entity {
private:
	int health;

public:
	//used in Game to keep track of update frequency of different entities
	static const std::string LABEL;
	
	/**
	x: the row to start at
	y: the column to start at
	s: the player's speed (how many pixels to move by)
	h: the player's health points
	*/
	Player(int x, int y, int s, int h);
	
	/**
	x: the row to start at
	y: the column to start at
	s: the player's speed (how many pixels to move by)
	*/
	Player(int x, int y, int s): Player(x, y, s, 3) {}
	
	virtual ~Player() {}
	
	/**
	increments/decrements posy
	*/
	void moveRight() { posy += speed; }
	void moveLeft() { posy -= speed; }
	
	/**
	spawns a new missile
	*/
	virtual Missile* shoot();
	
	void lowerHealth() { health--; }
	void increaseHealth() { health++; }
	int getHealth() { return health; }
	
	bool isAlive() { return health > 0; }
	
	/**
	modify player's position to stay within specified bounds
	
	yBoundLo: the lower bound
	yBoundHi: the upper bound
	*/
	void adjustPlayerBound(int yBoundLo, int yBoundHi);
};

#endif
