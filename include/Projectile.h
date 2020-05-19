#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
protected:
	int posx;
	int posy;
	int speed{ 1 };

public:
	Projectile(int x, int y): posx{ x }, posy{ y } {};
	
	Projectile(int x, int y, int s)
		: posx{ x }, posy{ y }, speed{ s } {};
	
	virtual ~Projectile() {};
	
	virtual void move() {};
	
	int getPosx() { return posx; };
	int getPosy() { return posy; };
};

#endif