#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
	int posx;
	int posy;
	int speed;

public:
	Player(int x, int y, int s)
		: posx{ x }, posy{ y }, speed{ s }  {};
	
	//increments/decrements posy
	void moveRight();
	void moveLeft();
	
	void shoot();
	
	friend void Game::adjustPlayerBound();
	
	//void setPosy(int y) { posy = y; }
};

#endif