#ifndef PLAYER_H
#define PLAYER_H

class Missile;

class Player {
private:
	int posx;
	int posy;
	int speed;

public:
	Player(int x, int y, int s)
		: posx{ x }, posy{ y }, speed{ s }  {};
	
	/**
	increments/decrements posy
	*/
	void moveRight() { posy += 1; }
	void moveLeft() { posy -= 1; }
	
	/**
	spawns a new missile
	*/
	Missile* shoot();
	
	friend void Game::adjustPlayerBound();
	
	//void setPosy(int y) { posy = y; }
};

#endif