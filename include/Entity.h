#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <utility>

class Entity {
protected:
	int posx;
	int posy;
	int speed;
	
	/**
	hitbox entries are relative to the entity's center (ie, posx, posy)
	*/
	std::vector<std::pair<int, int>> hitbox;

public:
	Entity(int x, int y, int s)
		: posx{ x }, posy{ y }, speed{ s }  {};
	
	Entity(): Entity(0, 0, 1) {};
	
	virtual ~Entity() {};
	
	int getPosx() { return posx; }
	int getPosy() { return posy; }
	int getSpeed() { return speed; }
	
	/**
	returns a const iterator to the start of the hitbox vector
	*/
	std::vector<std::pair<int, int>>::const_iterator getHitboxIterator() {
		return hitbox.begin();
	}
	
	bool isHitboxIteratorAtEnd(std::vector<std::pair<int, int>>::const_iterator it) {
		return it == hitbox.end();
	}
};

#endif
