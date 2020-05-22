#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include "Projectile.h"

class Obstacle: public Projectile {
public:
	static const std::string LABEL;
	
	Obstacle(int x, int y);
	
	virtual ~Obstacle() {}
	
	virtual void move() { posx += directionx; }
};

#endif
