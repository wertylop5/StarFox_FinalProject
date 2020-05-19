#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "Projectile.h"

class Obstacle: public Projectile {
public:
	Obstacle(int x, int y): Projectile(x, y, 3) {};
	
	virtual ~Obstacle() {};
	
	virtual void move() { posx++; }
};

#endif