#include "include/Game.h"
#include "include/Player.h"
#include "include/Missile.h"

const std::string Player::LABEL = "Player";

Missile* Player::shoot() {
	return new Missile(posx-1, posy);
}
