#ifndef MISSLE_H
#define MISSLE_H

#include "Projectile.h"

class Missile: public Projectile {
public:
	Missile(int x, int y): Projectile(x, y, 1) {};
	
	virtual ~Missile() {};
	
	virtual void move() { posx--; }
};

#endif