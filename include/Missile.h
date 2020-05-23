#ifndef MISSILE_H
#define MISSILE_H

#include <string>
#include "Projectile.h"

class Missile: public Projectile {
private:
	//how many obstacles missile can hit before being destroyed
	int durability;

public:
	//used in Game to keep track of update frequency of different entities
	static const std::string LABEL;
	
	/**
	x: the row to start at
	y: the column to start at
	d: the missile's durability (how many obstacles it can hit before
		being destroyed)
	dx: how much to move by in the x direction (ex: 1 means move
		1 pixel down the screen)
	dy: how much to move by in the y direction (ex: 1 means move
		1 pixel to the right)
	*/
	Missile(int x, int y, int d, int dx, int dy);
	
	/**
	x: the row to start at
	y: the column to start at
	d: the missile's durability (how many obstacles it can hit before
		being destroyed)
	*/
	Missile(int x, int y, int d): Missile(x, y, d, -1, 0) {}
	
	/**
	x: the row to start at
	y: the column to start at
	*/
	Missile(int x, int y): Missile(x, y, 1) {}
	
	virtual ~Missile() {}
	
	/**
	if the missile collided with an object
	*/
	void lowerDurability() { durability--; }
	
	int getDurability() { return durability; }
};

#endif
