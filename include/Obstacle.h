#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <string>
#include "Projectile.h"

class Obstacle: public Projectile {
public:
	static const std::string LABEL;
	
	Obstacle(int x, int y): Projectile(x, y, 1) {
		hitbox.push_back(std::pair<int, int>(0, 0));
	}
	
	virtual ~Obstacle() {}
	
	virtual void move() { posx++; }
};

#endif
