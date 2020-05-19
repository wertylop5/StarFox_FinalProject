#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "Entity.h"

class Projectile: public Entity {
public:
	Projectile(int x, int y): Entity(x, y, 1) {};
	
	Projectile(int x, int y, int s): Entity(x, y, s) {};
	
	virtual ~Projectile() {};
	
	virtual void move() {};
};

#endif