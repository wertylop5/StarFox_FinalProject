#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include "Projectile.h"

class Obstacle: public Projectile {
public:
	//used in Game to keep track of update frequency of different entities
	static const std::string LABEL;
	
	/**
	x: the row to start at
	y: the column to start at
	*/
	Obstacle(int x, int y);
	
	virtual ~Obstacle() {}
};

#endif
