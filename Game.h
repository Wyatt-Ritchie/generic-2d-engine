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
	// Function for retrieving textures
	SDL_Texture* GetTexture(const std::string& fileName);
	// Function for retrieving fonts
	class Font* GetFont(const std::string& fileName);
	// Function for returning the renderer
	SDL_Renderer* GetRenderer() const { return mRenderer; }
	class AudioSystem* GetAudioSystem() { return mAudioSystem; }

	// Return entire UI stack by reference
	const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; };

	// Push specified UIScreen onto stack
	void PushUI(class UIScreen* screen);

	int GetScreenHeight() const { return mScreenHeight; }
	int GetScreenWidth() const { return mScreenWidth; }

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

	int mScreenWidth;

	int mScreenHeight;

	// SDL renderer
	SDL_Renderer* mRenderer;

	// FMOD audio system
	class AudioSystem* mAudioSystem;

	// vectors containing active actors and pending actors
	std::vector<class Actor*> mActors;
	std::vector<class Actor*> mPendingActors;
	std::vector<class SpriteComponent*> mSprites;

	int mTicksCount;
	// Map of textures
	std::unordered_map<std::string, SDL_Texture*> mTextures;
	// Map of fonts
	std::unordered_map<std::string, Font*> mFonts;

	// UI screen stack for the game
	std::vector<class UIScreen*> mUIStack;
	
	bool mUpdatingActors;
	bool DrawCalled;
};

void foo();