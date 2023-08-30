#pragma once
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>

class Game
{
public:
	// constructor
	Game();

	// initialize
	bool Initialize();

	// Run Loop
	void RunLoop();

	// add/remove actor functions
	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);
	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

	SDL_Texture* GetTexture(const std::string& fileName);

	//shutdown game loop
	void ShutDown();
private:

	// helper functions for the game loop
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	void LoadData();

	void UnloadData();

	// Window created by SDL
	SDL_Window* mWindow;

	// game running bool
	bool mIsRunning;

	// SDL renderer
	SDL_Renderer* mRenderer;

	// vectors containing active actors and pending actors
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;

	int mTicksCount;
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	
	bool mUpdatingActors;

};

