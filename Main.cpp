#include "Game.h"
#include <iostream>
int main(int argc, char* argv[])
{
	time_t srand(time(NULL));
	Game game;
	bool success = game.Initialize();
	std::cout << "Game initialized" << std::endl;
	if(success)
	{
		std::cout << "Running Game" << std::endl;
		game.RunLoop();
	}
	std::cout << "Game Loop Exited" << std::endl;
	game.ShutDown();
	return 0;
}
