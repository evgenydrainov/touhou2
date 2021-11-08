#include "Game.h"

int main(int argc, char* argv[])
{
	auto& game = Game::getInstance();
	game.run();
}
