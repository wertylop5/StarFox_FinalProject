#ifndef ENTITY_H
#define ENTITY_H

class Missile;

class Entity {
protected:
	int posx;
	int posy;
	int speed;

public:
	Entity(): posx{ 0 }, posy{ 0 }, speed{ 0 } {};
	
	Entity(int x, int y, int s)
		: posx{ x }, posy{ y }, speed{ s }  {};
	
	virtual ~Entity() {};
	
	int getPosx() { return posx; }
	int getPosy() { return posy; }
};

#endif