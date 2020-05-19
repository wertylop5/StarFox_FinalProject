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
	printf("looping\r\n");
	clearBoard();
	
	for (auto it = missiles.begin(); it != missiles.end(); ++it) {
		(*it)->move();
	}
	
	for (auto it = obstacles.begin(); it != obstacles.end(); ++it) {	
		(*it)->move();
	}
	
	//remove any obstacles that the missile collides with
	for (auto it = missiles.begin(); it != missiles.end(); ++it) {
		Missile *m = *it;
		
		auto it2 = obstacles.begin();
		while (it2 != obstacles.end()) {
			Obstacle *o = *it2;
			
			if (hasCollided(m, o) || isObstacleBehind(m, o)) {
				printf("collision detected\r\n");
				
				delete o;
				obstacles.erase(it2);
			}
			else { ++it2; }
		}
	}
	
	updateBoard();
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

bool Game::isObstacleBehind(Missile* m, Obstacle* o) {
	return m->getPosx() < o->getPosx() && m->getPosy() == o->getPosy();
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
