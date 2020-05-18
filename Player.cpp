#include "include/Game.h"
#include "include/Player.h"

void Player::moveRight() {
	posy += speed;
}

void Player::moveLeft() {
	posy -= speed;
}
