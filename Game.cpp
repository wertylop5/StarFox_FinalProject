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
	
	if (boss != 0) delete boss;
	boss = 0;
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
	
	if (bossSpawnFlag) {
		placeToken(boss->getPosx(), boss->getPosy(), BoardToken::boss);
	}
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
	switch (token) {
		case BoardToken::missile: case BoardToken::obstacle:
		{
			board[x][y] = static_cast<int>(token);
		}
		break;
		
		case BoardToken::player:
		{
			auto it = player.getHitboxIterator();
			while (!player.isHitboxIteratorAtEnd(it)) {
				auto pair = (*it);
				
				if (isInBounds(x+pair.first, y+pair.second)) {
					board[x+pair.first][y+pair.second] = static_cast<int>(token);
				}
				
				++it;
			}
		}
		break;
		
		case BoardToken::boss:
		{
			if (bossSpawnFlag) {
				auto it = boss->getHitboxIterator();
				while (!boss->isHitboxIteratorAtEnd(it)) {
					auto pair = (*it);
					
					if (isInBounds(x+pair.first, y+pair.second)) {
						board[x+pair.first][y+pair.second] = static_cast<int>(token);
					}
					
					++it;
				}
			}
		}
		break;
		
		case BoardToken::empty: default:
		break;
	}
	
}

bool Game::isInBounds(Entity *e) {
	return e->getPosx() >= 0 && e->getPosx() < Game::NUM_ROWS &&
			e->getPosy() >= 0 && e->getPosy() < Game::NUM_COLS;
}

bool Game::isInBounds(int x, int y) {
	return x >= 0 && x < Game::NUM_ROWS &&
			y >= 0 && y < Game::NUM_COLS;
}

void Game::decrementCounters() {
	for (auto it = std::begin(refreshCounters); it != std::end(refreshCounters); ++it) {
		refreshCounters[it->first] -= 1;
	}
	
	//print values for debugging
	/*
	for (auto it = std::begin(refreshCounters); it != std::end(refreshCounters); ++it) {
		printf("%s: %d\n", (it->first).c_str(), it->second);
	}
	*/
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

void Game::addMissiles() {
	int x, y;
	while (missileBufferPos > 0) {
		//the location on the end is odd
		x = missileBuffer[missileBufferPos-1][0];
		y = missileBuffer[missileBufferPos-1][1];
		
		missileBufferPos--;
		
		if (isInBounds(x, y)) {
			printf("adding missile at %d, %d\n", x, y);
			
			int durability = (rand() % 100 < Game::CRIT_MISSILE_CHANCE) ? 2 : 1;
			
			missiles.push_back(new Missile(x, y, durability));
		}
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
				score++;
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

void Game::checkPlayerCollision(Player* p) {
	auto it2 = obstacles.begin();
	while ( it2 != obstacles.end()) {
		Obstacle *o = *it2;
		
		if (hasCollided(p, o)) {
			printf("player hit\n");
			
			delete o;
			obstacles.erase(it2);
			
			p->lowerHealth();
			
			if (!p->isAlive()) break;
		}
		else { ++it2; }
	}
	
	auto it = missiles.begin();
	while ( it != missiles.end()) {
		Missile *m = *it;
		
		if (hasCollided(p, m)) {
			printf("player hit\n");
			
			delete m;
			missiles.erase(it);
			
			p->lowerHealth();
			
			if (!p->isAlive()) break;
		}
		else { ++it; }
	}
	
	if (!p->isAlive()) {
		if (p == &player) endGameFlag = true;
		else if (p == boss) {
			bossDestroyedFlag = true;
		}
	}
}

bool Game::loop() {
	printf("looping\r\n");
	clearBoard();
	
	decrementCounters();
	//adjustPlayerBound(&player);
	player.adjustPlayerBound(0, Game::NUM_COLS);
	
	if (bossSpawnFlag) boss->adjustPlayerBound(0, Game::NUM_COLS);
	
	printf("bossSpawn: %d, bossDestroyed: %d\n", bossSpawnFlag, bossDestroyedFlag);
	
	if (bossDestroyedFlag) {
		printf("removing boss\n");
		
		delete boss;
		boss = 0;
		
		bossSpawnFlag = false;
		bossDestroyedFlag = false;
	}
	
	if (bossSpawnFlag && !bossDestroyedFlag) {
		if (boss->shouldShoot()) {
			missiles.push_back(boss->shoot());
		}
		
		boss->move();
	}
	
	//0 means ready to refresh
	if (refreshCounters[Missile::LABEL] == 0) {
		refreshCounters[Missile::LABEL] = refreshSpeeds[Missile::LABEL];
		
		moveMissiles();
	}
	if (refreshCounters[Obstacle::LABEL] == 0) {
		refreshCounters[Obstacle::LABEL] = refreshSpeeds[Obstacle::LABEL];
		
		moveObstacles();
		
		if (!bossSpawnFlag) spawnObstacles();
	}
	
	removeOutOfBoundsProjectiles();
	checkProjectileCollision();
	checkPlayerCollision(&player);
	if (bossSpawnFlag) checkPlayerCollision(boss);
	
	addMissiles();
	
	if (score != 0 && score % BOSS_SPAWN_CONDITION == 0 && !bossSpawnFlag) {
		//spawn the boss
		printf("spawning boss\n");
		
		bossSpawnFlag = true;
		bossDestroyedFlag = false;
		
		boss = new BossPlayer(0, 0, 1, 5, 1);
	}
	
	updateBoard();
	
	return !endGameFlag;
}

bool Game::hasCollided(Entity* a, Entity* b) {
	auto ita = a->getHitboxIterator();
	while (!a->isHitboxIteratorAtEnd(ita)) {
		auto paira = (*ita);
		
		auto itb = b->getHitboxIterator();
		while (!b->isHitboxIteratorAtEnd(itb)) {
			auto pairb = (*itb);
			
			//remember that hitbox gives an offset from center
			if (a->getPosx()+paira.first == b->getPosx()+pairb.first &&
					a->getPosy()+paira.second == b->getPosy()+pairb.second) {
				return true;
			}
			
			++itb;
		}
		
		++ita;
	}
	
	return false;
}

bool Game::isObstacleBehind(Missile* m, Obstacle* o) {
	auto itm = m->getHitboxIterator();
	while (!m->isHitboxIteratorAtEnd(itm)) {
		auto pairm = (*itm);
		
		auto ito = o->getHitboxIterator();
		while (!o->isHitboxIteratorAtEnd(ito)) {
			auto pairo = (*ito);
			
			//remember that hitbox gives an offset from center
			if (m->getPosx()+pairm.first < o->getPosx()+pairo.first &&
					m->getPosy()+pairm.second == o->getPosy()+pairo.second) {
				return true;
			}
			
			++ito;
		}
		
		++itm;
	}
	
	return false;
}

void Game::handleShoot() {
	/*
	this function is necessary because using the "new" keyword is
	not allowed in an ISR
	*/
	if (missileBufferPos < MISSILE_BUFFER_SIZE) {
		missileBuffer[missileBufferPos][0] = player.getPosx()-1;
		missileBuffer[missileBufferPos][1] = player.getPosy();
		
		missileBufferPos++;
	}
}

void Game::spawnObstacles() {
	if(rand() % Game::NUM_ROWS <= 1){	
		for (int x = 0; x < Game::MAX_OBSTACLES_PER_SPAWN; ++x) {
			obstacles.push_back(new Obstacle(0, rand() % Game::NUM_COLS));
		}
	}
}

void Game::spawnMissiles() {
	for (int y = 0; y < Game::NUM_COLS; ++y) {
		missiles.push_back(new Missile(Game::NUM_ROWS-1, y));
	}
}

/*
void Game::adjustPlayerBound(Player* p) {
	if (p->posy < 0) p->posy = 0;
	if (p->posy >= Game::NUM_COLS) p->posy = Game::NUM_COLS-1;
}
*/
