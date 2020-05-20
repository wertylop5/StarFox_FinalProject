#ifndef MISSLE_H
#define MISSLE_H

#include <string>
#include "Projectile.h"

class Missile: public Projectile {
private:
	int durability;

public:
	static const std::string LABEL;
	
	Missile(int x, int y): Projectile(x, y, 1), durability{ 1 } {};
	
	virtual ~Missile() {};
	
	virtual void move() { posx--; }
	
	/**
	if the missile collided with an object
	*/
	void lowerDurability() { durability--; }
	
	int getDurability() { return durability; }
};

#endif
