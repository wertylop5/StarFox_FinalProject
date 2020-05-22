#include "include/BossPlayer.h"
#include "include/Game.h"

const std::string BossPlayer::LABEL = "BossPlayer";

BossPlayer::BossPlayer(int x, int y, int s, int h, int f)
		: Player(x, y, s, h), shootCounter{ 0 }, shootFrequency{ f } {
	if (posy <= 0) direction = 1;
	else if (posy >= Game::NUM_COLS-1) direction = -1;
	
	//boss is a T shape
	hitbox.push_back(std::pair<int, int>(0, 0));
	hitbox.push_back(std::pair<int, int>(1, 0));
	hitbox.push_back(std::pair<int, int>(0, -1));
	hitbox.push_back(std::pair<int, int>(0, 1));
}

void BossPlayer::move() {
	posy += speed*direction;
	
	if (posy <= 0) direction = 1;
	else if (posy >= Game::NUM_COLS-1) direction = -1;
	
	shootCounter++;
}

bool BossPlayer::shouldShoot() {
	if (shootCounter >= shootFrequency) {
		shootCounter = 0;
		return true;
	}
	return false;
}

Missile* BossPlayer::shoot() {
	return new Missile(posx+2, posy, 1, 1, 0);
}
