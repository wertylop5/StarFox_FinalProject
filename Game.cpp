#include <cstdlib>
#include <string>

#include "Game.h"


Game::~Game() {
	
}

void Game::init() {
	srand(0);
}

void Game::printBoard() {
	printf("-------------------------------------\r\n");
	for (int x = 0; x < Game::NUM_ROWS; x++) {
		std::string row;
		for (int y = 0; y < Game::NUM_COLS; y++) {
			//printf("%d ", board[x][y]);
			row += (" " + std::to_string(board[x][y]));
		}
		//printf("\r\n");
		printf("%s\r\n", row.c_str());
	}
}

void Game::loop() {
	
}

void Game::spawnObstacles() {
	int numSpawned = 0;
	
	for (int y = 0; y < Game::NUM_COLS; y++) {
		if (numSpawned == Game::MAX_OBSTACLES_PER_SPAWN) break;
		
		if (rand() % 2) {
			numSpawned--;
			
			board[0][y] = static_cast<int>(BoardToken::obstacle);
			
			projectiles.push_back(new Obstacle);
		}
	}
}

void Game::adjustPlayerBound() {
	if (player.posy < 0) player.posy = 0;
	if (player.posy >= Game::NUM_COLS) player.posy = Game::NUM_COLS-1;
}
