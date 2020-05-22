#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <string>
#include "Entity.h"

/**

*/

class Projectile: public Entity {
protected:
	int directionx = 0;
	int directiony = 0;

public:
	static const std::string LABEL;
	
	Projectile(int x, int y, int s, int dx, int dy): Entity(x, y, s),
		directionx{ dx }, directiony{ dy } {}
	
	Projectile(int x, int y, int s): Entity(x, y, s) {}
	
	Projectile(int x, int y): Projectile(x, y, 1) {}
	
	virtual ~Projectile() {}
	
	virtual void move() {}
};

#endif
