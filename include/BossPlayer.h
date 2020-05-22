#ifndef BOSSPLAYER_H
#define BOSSPLAYER_H

#include "Player.h"

class BossPlayer: public Player {
private:
	int direction;

public:
	BossPlayer(int x, int y, int s, int h): Player(x, y, s, h) {}
	
	BossPlayer(int x, int y): BossPlayer(x, y, 2, 5) {}
	
	virtual ~BossPlayer() {}
	
	virtual void moveRight() { posy += speed; }
	virtual void moveLeft() { posy -= speed; }
};

#endif
