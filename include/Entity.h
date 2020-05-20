#ifndef ENTITY_H
#define ENTITY_H

class Missile;

class Entity {
protected:
	int posx;
	int posy;
	int speed;

public:
	Entity(int x, int y, int s)
		: posx{ x }, posy{ y }, speed{ s }  {};
	
	Entity(): Entity(0, 0, 0) {};
	
	virtual ~Entity() {};
	
	int getPosx() { return posx; }
	int getPosy() { return posy; }
};

#endif