#include "include/Game.h"
#include "include/Player.h"
#include "include/Missile.h"

const std::string Player::LABEL = "Player";

//const int Player::hitbox = { {0, 0}, {-1, 0}, {0, -1}, {0, 1} };

Missile* Player::shoot() {
	return new Missile(posx-1, posy);
}
