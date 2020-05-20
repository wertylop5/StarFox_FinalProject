#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include "Projectile.h"

class Obstacle: public Projectile {
public:
	static const std::string LABEL;
	
	Obstacle(int x, int y): Projectile(x, y, 1) {};
	
	virtual ~Obstacle() {};
	
	virtual void move() { posx++; }
};

#endif
