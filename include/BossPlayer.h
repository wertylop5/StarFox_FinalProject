#ifndef BOSSPLAYER_H
#define BOSSPLAYER_H

#include "Player.h"

class BossPlayer: public Player {
private:
	//the direction the boss is currently moving in
	int direction;
	
	//keeps track of how long until the next time the boss shoots
	int shootCounter;
	
	//how often the boss should shoot
	int shootFrequency;

public:
	//used in Game to keep track of update frequency of different entities
	static const std::string LABEL;
	
	/**
	f is the frequency at which the boss shoots (relative to how many times
		it has moved)
	
	for example, a frequency of 2 means the boss shoots after it moves 2 times
	
	x: the row to start at
	y: the column to start at
	s: the boss's speed (how many pixels to move by)
	h: the boss's health points
	f: the frequency at which the boss shoots (relative to how many times
		it has moved)
	*/
	BossPlayer(int x, int y, int s, int h, int f);
	
	/**
	x: the row to start at
	y: the column to start at
	s: the boss's speed (how many pixels to move by)
	h: the boss's health points
	*/
	BossPlayer(int x, int y, int s, int h): BossPlayer(x, y, s, h, 3) {}
	
	/**
	x: the row to start at
	y: the column to start ats
	*/
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
