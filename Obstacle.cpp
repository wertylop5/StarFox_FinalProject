#include "include/Obstacle.h"

const std::string Obstacle::LABEL = "Obstacle";

Obstacle::Obstacle(int x, int y): Projectile(x, y, 1, 1, 0) {
	hitbox.push_back(std::pair<int, int>(0, 0));
}
