#include "src/Game.h"

int main() {
	Game game;
	game.Setup();
	game.SetupTempWorldState();
	game.Start();
	return 0;
}

