#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
private:
	int speed{ 1 };
	int posx;
	int posy;

public:
	Projectile() {};
	
	void move();
};

#endif