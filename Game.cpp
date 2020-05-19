#include <cstdlib>
#include <string>

#include "Game.h"


Game::~Game() {
	for (std::vector<Missile*>::iterator it = missiles.begin(); it != missiles.end(); ++it) {
		delete *it;
	}
	for (std::vector<Obstacle*>::iterator it = obstacles.begin(); it != obstacles.end(); ++it) {
		delete *it;
	}
	missiles.clear();
	obstacles.clear();
}

void Game::init(PinName seed_pin) {
	AnalogIn seed(seed_pin);
	srand(seed.read_u16());
	
	clearBoard();
}

void Game::clearBoard() {
	for (auto it = std::begin(board); it != std::end(board); ++it) { 
		std::fill(std::begin(*it), std::end(*it), static_cast<int>(BoardToken::empty));
	}
}

void Game::printBoard() {
	printf("-------------------------------------\r\n");
	for (int x = 0; x < Game::NUM_ROWS; x++) {
		std::string row;
		for (int y = 0; y < Game::NUM_COLS; ++y) {
			row += (" " + std::to_string(board[x][y]));
		}
		printf("%s\r\n", row.c_str());
	}
}

void Game::placeToken(int x, int y, BoardToken token) {
	board[x][y] = static_cast<int>(token);
}

void Game::loop() {
	clearBoard();
	
	for (auto it = missiles.begin(); it != missiles.end(); ++it) {
		Missile *m = *it;
		
		m->move();
		placeToken(m->getPosx(), m->getPosy(), BoardToken::missile);
	}
	
	//remove any obstacles that the missile collides with
	for (auto it = missiles.begin(); it != missiles.end(); ++it) {
		Missile *m = *it;
		
		for (auto it2 = obstacles.begin(); it2 != obstacles.end(); ++it2) {
			Obstacle *o = *it2;
			
			if (hasCollided(m, o)) {
				delete o;
				obstacles.erase(it2);
				
				//assumption: obstacles don't overlap each other
				break;
			}
		}
	}
	
	for (Obstacle* o : obstacles) {
		o->move();
		placeToken(o->getPosx(), o->getPosy(), BoardToken::obstacle);
	}
}

void Game::spawnObstacles() {
	int numSpawned = 0;
	
	for (int y = 0; y < Game::NUM_COLS; y++) {
		if (numSpawned == Game::MAX_OBSTACLES_PER_SPAWN) break;
		
		if (rand() % 2) {
			numSpawned++;
			
			placeToken(0, y, BoardToken::obstacle);
			
			obstacles.push_back(new Obstacle(0, y));
		}
	}
}

bool Game::hasCollided(Projectile* a, Projectile* b) {
	return a->getPosx() == b->getPosx() && a->getPosy() == b->getPosy();
}

void Game::handleShoot() {
	
}

void Game::spawnMissiles() {
	for (int y = 0; y < Game::NUM_COLS; ++y) {
		missiles.push_back(new Missile(Game::NUM_ROWS-1, y));
		placeToken(Game::NUM_ROWS-1, y, BoardToken::missile);
	}
}

void Game::adjustPlayerBound() {
	if (player.posy < 0) player.posy = 0;
	if (player.posy >= Game::NUM_COLS) player.posy = Game::NUM_COLS-1;
}
