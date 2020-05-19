#include "include/Game.h"
#include "include/Player.h"
#include "include/Missile.h"

Missile* Player::shoot() {
	return new Missile(posx, posy);
}
