#ifndef BOSSPLAYER_H
#define BOSSPLAYER_H

#include "Player.h"

class BossPlayer: public Player {
private:
	int direction;
	int shootCounter;
	int shootFrequency;

public:
	static const std::string LABEL;
	
	BossPlayer(int x, int y, int s, int h, int f);
	
	BossPlayer(int x, int y, int s, int h): BossPlayer(x, y, s, h, 3) {}
	
	BossPlayer(int x, int y): BossPlayer(x, y, 2, 5) {}
	
	virtual ~BossPlayer() {}
	
	/**
	should move in a back and forth manner
	*/
	void move();
	
	/**
	whether or not the boss should shoot a missile
	*/
	bool shouldShoot();
	
	/**
	spawn a missile
	*/
	virtual Missile* shoot();
};

#endif
