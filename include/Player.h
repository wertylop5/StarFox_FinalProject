#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include "Entity.h"

class Missile;

class Player: public Entity {
private:
	int health;

public:
	static const std::string LABEL;
	
	Player(int x, int y, int s, int h);
	
	Player(int x, int y, int s): Player(x, y, s, 3) {}
	
	virtual ~Player() {}
	
	/**
	increments/decrements posy
	*/
	void moveRight() { posy += speed; }
	void moveLeft() { posy -= speed; }
	
	/**
	spawns a new missile
	*/
	virtual Missile* shoot();
	
	void lowerHealth() { health--; }
	void increaseHealth() { health++; }
	
	bool isAlive() { return health > 0; }
	
	//friend void Game::adjustPlayerBound(Player* p);
	void adjustPlayerBound(int yBoundLo, int yBoundHi);
};

#endif
