#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include "Entity.h"

class Projectile: public Entity {
public:
	static const std::string LABEL;
	
	Projectile(int x, int y, int s): Entity(x, y, s) {};
	
	Projectile(int x, int y): Projectile(x, y, 1) {};
	
	virtual ~Projectile() {};
	
	virtual void move() {};
};

#endif
