#include <cstdlib>
#include <string>

#include "Game.h"

void print_memory_info() {
    // allocate enough room for every thread's stack statistics
    int cnt = osThreadGetCount();
    mbed_stats_stack_t *stats = (mbed_stats_stack_t*) malloc(cnt * sizeof(mbed_stats_stack_t));

	
    cnt = mbed_stats_stack_get_each(stats, cnt);
    for (int i = 0; i < cnt; i++) {
        printf("Thread: 0x%lX, Stack size: %lu / %lu\r\n", stats[i].thread_id, stats[i].max_size, stats[i].reserved_size);
    }
    free(stats);
	
    // Grab the heap statistics
    mbed_stats_heap_t heap_stats;
    mbed_stats_heap_get(&heap_stats);
    printf("Heap size: %lu / %lu bytes\r\n", heap_stats.current_size, heap_stats.reserved_size);
}


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
	
	refreshSpeeds.emplace(Player::LABEL, 1);
	refreshSpeeds.emplace(Projectile::LABEL, 1);
	refreshSpeeds.emplace(Missile::LABEL, 1);
	refreshSpeeds.emplace(Obstacle::LABEL, 3);
	
	refreshCounters.emplace(Player::LABEL, refreshSpeeds[Player::LABEL]);
	refreshCounters.emplace(Projectile::LABEL, refreshSpeeds[Projectile::LABEL]);
	refreshCounters.emplace(Missile::LABEL, refreshSpeeds[Missile::LABEL]);
	refreshCounters.emplace(Obstacle::LABEL, refreshSpeeds[Obstacle::LABEL]);
}

void Game::clearBoard() {
	for (auto it = std::begin(board); it != std::end(board); ++it) { 
		std::fill(std::begin(*it), std::end(*it), static_cast<int>(BoardToken::empty));
	}
}

void Game::updateBoard() {
	for (auto it = missiles.begin(); it != missiles.end(); ++it) {
		Missile *m = *it;
		
		placeToken(m->getPosx(), m->getPosy(), BoardToken::missile);
	}
	
	for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {
		Obstacle *o = *it;
		
		placeToken(o->getPosx(), o->getPosy(), BoardToken::obstacle);
	}
	
	placeToken(player.getPosx(), player.getPosy(), BoardToken::player);
}

void Game::printBoard() {
	printf("-------------------------------------\r\n");
	updateBoard();
	for (int x = 0; x < Game::NUM_ROWS; x++) {
		std::string row;
		for (int y = 0; y < Game::NUM_COLS; ++y) {
			row += (" " + std::to_string(board[x][y]));
		}
		printf("%s\r\n", row.c_str());
	}
}

void Game::clampBoard() {
	for (int x = 0; x < Game::NUM_ROWS; x++) {
		for (int y = 0; y < Game::NUM_COLS; ++y) {
			clampedBoard[x][y] = board[x][y] == 0 ? 0 : std::min(board[x][y], 1);
		}
	}
}

void Game::placeToken(int x, int y, BoardToken token) {
	board[x][y] = static_cast<int>(token);
}

bool Game::isInBounds(Entity *e) {
	return e->getPosx() >= 0 && e->getPosx() < Game::NUM_ROWS &&
			e->getPosy() >= 0 && e->getPosy() < Game::NUM_COLS;
}

void Game::decrementCounters() {
	for (auto it = std::begin(refreshCounters); it != std::end(refreshCounters); ++it) {
		refreshCounters[it->first] -= 1;
	}
	
	//print values for debugging
	for (auto it = std::begin(refreshCounters); it != std::end(refreshCounters); ++it) {
		printf("%s: %d\n", (it->first).c_str(), it->second);
	}
}

void Game::moveMissiles() {
	for (auto it = missiles.begin(); it != missiles.end(); ++it) {
		(*it)->move();
	}
}

void Game::moveObstacles() {
	for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {	
		(*it)->move();
	}
}

void Game::checkProjectileCollision() {
	//remove any obstacles that the missile collides with
	auto it = missiles.begin();
	while (it != missiles.end()) {
		Missile *m = *it;
		
		//check every obstacle
		auto it2 = obstacles.begin();
		while (it2 != obstacles.end()) {
			Obstacle *o = *it2;
			
			if (hasCollided(m, o) || isObstacleBehind(m, o)) {
				printf("obstacle hit\r\n");
				
				delete o;
				obstacles.erase(it2);
				
				m->lowerDurability();
			}
			else { ++it2; }
			
			//delete the missile that's destroyed
			if (!(m->getDurability())) break;
		}
		
		if (!(m->getDurability())) {
			printf("missile destroyed\n");
			
			delete m;
			missiles.erase(it);
		}
		else { ++it; }
	}
}

void Game::removeOutOfBoundsProjectiles() {
	auto it = missiles.begin();
	while ( it != missiles.end()) {
		Missile *m = *it;
		
		if (!isInBounds(m)) {
			printf("removing oob missile\n");
			
			delete m;
			missiles.erase(it);
		}
		else { ++it; }
	}
	
	auto it2 = obstacles.begin();
	while ( it2 != obstacles.end()) {
		Obstacle *o = *it2;
		
		if (!isInBounds(o)) {
			printf("removing oob obstacle\n");
			
			delete o;
			obstacles.erase(it2);
		}
		else { ++it2; }
	}
}

void Game::checkPlayerCollision() {
	auto it2 = obstacles.begin();
	while ( it2 != obstacles.end()) {
		Obstacle *o = *it2;
		
		if (hasCollided(&player, o)) {
			printf("player hit\n");
			
			delete o;
			obstacles.erase(it2);
			
			player.lowerHealth();
			
			if (!player.isAlive()) break;
		}
		else { ++it2; }
	}
	
	if (!player.isAlive()) endGameFlag = true;
}

bool Game::loop() {
	printf("looping\r\n");
	clearBoard();
	
	decrementCounters();
	
	if (refreshCounters[Missile::LABEL] == 0) {
		refreshCounters[Missile::LABEL] = refreshSpeeds[Missile::LABEL];
		
		moveMissiles();
	}
	if (refreshCounters[Obstacle::LABEL] == 0) {
		refreshCounters[Obstacle::LABEL] = refreshSpeeds[Obstacle::LABEL];
		
		moveObstacles();
		spawnObstacles();
	}
	
	removeOutOfBoundsProjectiles();
	checkProjectileCollision();
	checkPlayerCollision();
	adjustPlayerBound();
	updateBoard();
	
	return !endGameFlag;
}

bool Game::hasCollided(Entity* a, Entity* b) {
	return a->getPosx() == b->getPosx() && a->getPosy() == b->getPosy();
}

bool Game::isObstacleBehind(Missile* m, Obstacle* o) {
	return m->getPosx() < o->getPosx() && m->getPosy() == o->getPosy();
}

void Game::handleShoot() {
	Missile *newMissile = player.shoot();
	
	missiles.push_back(newMissile);
}

void Game::spawnObstacles() {
	int numSpawned = 0;
	
	for (int y = 0; y < Game::NUM_COLS; y++) {
		if (numSpawned == Game::MAX_OBSTACLES_PER_SPAWN) break;
		
		if (rand() % 2) {
			numSpawned++;
			
			//placeToken(0, y, BoardToken::obstacle);
			
			obstacles.push_back(new Obstacle(0, y));
		}
	}
}

void Game::spawnMissiles() {
	for (int y = 0; y < Game::NUM_COLS; ++y) {
		missiles.push_back(new Missile(Game::NUM_ROWS-1, y));
		//placeToken(Game::NUM_ROWS-1, y, BoardToken::missile);
	}
}

void Game::adjustPlayerBound() {
	if (player.posy < 0) player.posy = 0;
	if (player.posy >= Game::NUM_COLS) player.posy = Game::NUM_COLS-1;
}
