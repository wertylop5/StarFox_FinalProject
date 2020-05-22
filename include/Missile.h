#ifndef MISSILE_H
#define MISSILE_H

#include <string>
#include "Projectile.h"

class Missile: public Projectile {
private:
	int durability;

public:
	static const std::string LABEL;
	
	Missile(int x, int y, int d, int dx, int dy);
	
	Missile(int x, int y, int d): Missile(x, y, d, -1, 0) {}
	
	Missile(int x, int y): Missile(x, y, 1) {}
	
	virtual ~Missile() {}
	
	virtual void move() { posx += directionx; }
	
	/**
	if the missile collided with an object
	*/
	void lowerDurability() { durability--; }
	
	int getDurability() { return durability; }
};

#endif
