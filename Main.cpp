#include "Game.h"
#include <iostream>
#include <ctime>
int main(int argc, char* argv[])
{
	std::srand(std::time(nullptr));
	Game game;
	bool success = game.Initialize();
	std::cout << "Game initialized" << std::endl;
	if (success)
	{
		std::cout << "Running Game" << std::endl;
		game.RunLoop();
	}
	std::cout << "Game Loop Exited" << std::endl;
	game.ShutDown();
	return 0;
}
