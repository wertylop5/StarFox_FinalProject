#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include "Entity.h"

/**

*/

class Projectile: public Entity {
protected:
	//how much to move by in the x direction
	int directionx{ 0 };
	
	//how much to move by in the y direction
	int directiony{ 0 };

public:
	//used in Game to keep track of update frequency of different entities
	static const std::string LABEL;
	
	/**
	x: the row to start at
	y: the column to start at
	s: the projectile's speed (how many pixels to move by)
	dx: how much to move by in the x direction (ex: 1 means move
		1 pixel down the screen)
	dy: how much to move by in the y direction (ex: 1 means move
		1 pixel to the right)
	*/
	Projectile(int x, int y, int s, int dx, int dy): Entity(x, y, s),
		directionx{ dx }, directiony{ dy } {}
	
	/**
	x: the row to start at
	y: the column to start at
	s: the missile's speed (how many pixels to move by)
	*/
	Projectile(int x, int y, int s): Entity(x, y, s) {}
	
	/**
	x: the row to start at
	y: the column to start at
	*/
	Projectile(int x, int y): Projectile(x, y, 1) {}
	
	virtual ~Projectile() {}
	
	/**
	move the projectile
	*/
	void move() { posx += directionx; }
};

#endif
