#include "include/Game.h"
#include "include/Player.h"
#include "include/Missile.h"

const std::string Player::LABEL = "Player";

//const int Player::hitbox = { {0, 0}, {-1, 0}, {0, -1}, {0, 1} };

Player::Player(int x, int y, int s, int h): Entity(x, y, s), health{ h } {
	
	//player is a flipped T shape
	hitbox.push_back(std::pair<int, int>(0, 0));
	hitbox.push_back(std::pair<int, int>(-1, 0));
	hitbox.push_back(std::pair<int, int>(0, -1));
	hitbox.push_back(std::pair<int, int>(0, 1));
}

Missile* Player::shoot() {
	return new Missile(posx-2, posy);
}

void Player::adjustPlayerBound(int yBoundLo, int yBoundHi) {
	if (posy < yBoundLo) posy = yBoundLo;
	if (posy >= yBoundHi) posy = yBoundHi-1;
}
