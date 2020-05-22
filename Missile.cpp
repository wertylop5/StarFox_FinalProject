#include "include/Missile.h"

const std::string Missile::LABEL = "Missile";

Missile::Missile(int x, int y, int d, int dx, int dy)
		: Projectile(x, y, 1, dx, dy), durability{ d } {
	hitbox.push_back(std::pair<int, int>(0, 0));
}
