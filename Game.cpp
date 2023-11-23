#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"
#include "TileMapComponent.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "Math.h"
#include "Font.h"
#include "BGSpriteComponent.h"
#include "AudioSystem.h"
#include "UIScreen.h"
#include "TitleText.h"
#include "TextBox.h"
#include "Button.h"
#include <iostream>
#include <algorithm>
#include <vector>
Game::Game() : mWindow(nullptr),
			   mRenderer(nullptr), 
			   mIsRunning(true),
			   mUpdatingActors(false),
			   mAudioSystem(nullptr),
			   mScreenHeight(768),
	           mScreenWidth(1024)
{
	DrawCalled = false;
}

bool Game::Initialize()
{
	// int is 0 for success, non zero for fail
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		// reutrn false and a c-style string error message to the console
		SDL_Log("Unable to initialize the SDL: %s", SDL_GetError());
		return false;
	}

	// Initialize SDL TTF
	int ttfResult = TTF_Init();
	if (ttfResult != 0)
	{
		SDL_Log("Unable to initialize TTD: %s", TTF_GetError());
	}

	// create a window
	mWindow = SDL_CreateWindow(
		"2D-Game Engine", // Window Title
		100, // Top left x-coordinate of window
		100, // top left y-coordinate of window
		mScreenWidth, // width of window
		mScreenHeight, // height of window
		0 // flags (0 for no flags set)
	);

	// check that window was created. Otherwise send error message
	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	// initialize the SDL renderer for mWindow
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create the renderer for
		-1, // usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	// return false if rendered fails to initialize
	if (!mRenderer)
	{
		SDL_Log("Failed to initialize renderer: %s", SDL_GetError());
		return false;
	}

	int flags = IMG_INIT_PNG;
	int imgResult = IMG_Init(flags);

	// Initialize audio system
	mAudioSystem = new AudioSystem(this);
	if (!mAudioSystem->Initialize())
	{
		SDL_Log("No audio system to initialize.");
		mAudioSystem->ShutDown();
		delete mAudioSystem;
		mAudioSystem = nullptr;
		return false;
	}
	

	// Call the Load data function
	LoadData();

	mTicksCount = SDL_GetTicks();
	// set mIsRunning to true

	return true;
}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (!mUIStack.empty())
			{
				mUIStack.back()->HandleKeyPress(event.button.button);
			}
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		std::cout << "Escape pressed, begin shutdown" << std::endl;
		mIsRunning = false;
	}

	// Process input for actors
	mUpdatingActors = true;
	for (auto actor : mActors)
	{
		actor->ProcessInput(state);
	}
	mUpdatingActors = false;

	// Process input for UI screens
	for (auto& ui : mUIStack)
	{
		ui->ProcessInput(state);
	}

}

// implementation of the game loop function. 
void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();

		UpdateGame();

		GenerateOutput();
	}

}

void Game::UpdateGame() 
{
	// ensure there is a 16ms elapsed time between frames
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

	// DeltaTime is thge difference in ticks from last
	// converted to seconds
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
	// update ticks count

	// clamp maximum delta time value
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();

	// Update the audio system
	mAudioSystem->Update(deltaTime);

	// update all actors

	mUpdatingActors = true;

	for (auto actor : mActors)
	{
		actor->Update(deltaTime);
	}

	mUpdatingActors = false;

	// move any pending actors to mActors
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// add any dead actors to a temp vector
	std::vector<Actor*> deadActors;
	for (auto actor : mActors)
	{
		if (actor->getState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	// delete dead actors (remove them from mActors
	for (auto actor : deadActors)
	{
		delete actor;
	}

	for (auto ui : mUIStack)
	{
		if (ui->GetState() == UIScreen::EActive)
		{
			ui->Update(deltaTime);
		}
	}
	for(auto ui : mUIStack)
		if (ui->GetState() == UIScreen::EClosing)
		{
			delete ui;
		}

}

void Game::GenerateOutput() 
{
	SDL_SetRenderDrawColor(
		mRenderer,
		0,
		0,
		255,
		255
	);

	SDL_RenderClear(mRenderer);

	// Render the game sprites
	for (auto sprite : mSprites)
	{
		sprite->Draw(mRenderer);
	}

	// Render the games active UI screens
	for (auto ui : mUIStack)
	{
		ui->Draw(mRenderer);
	}
	SDL_RenderPresent(mRenderer);

	
}

void Game::AddActor(Actor* actor)
{
	// if updating actors, need to add to pending
	if (mUpdatingActors)
	{
		mPendingActors.emplace_back(actor);
	}
	else
	{
		mActors.emplace_back(actor);
	}
}

void Game::AddSprite(SpriteComponent* sprite)
{
	// find the insertion point in the sorted vector

	// (the first element with a higher draw order than me)

	int myDrawOrder = sprite->GetDrawOrder();
	auto iter = mSprites.begin();
	for (;iter != mSprites.end();++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}
	//insert element before position of the iterator
	mSprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

SDL_Texture* Game::GetTexture(const std::string& fileName)
{
	SDL_Texture* tex = nullptr;
	auto iter = mTextures.find(fileName);

	if (iter != mTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		// Load from file
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return nullptr;
		}

		

		// Create texture from surface
		tex = SDL_CreateTextureFromSurface(mRenderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return nullptr;
		}

		mTextures.emplace(fileName.c_str(), tex);
	}
	return tex;
}

Font* Game::GetFont(const std::string& fileName)
{
	
	auto iter = mFonts.find(fileName);

	if (iter != mFonts.end())
	{
		return iter->second;
	}
	else
	{
		Font* font = new Font(this);
		if (font->Load(fileName))
		{
			mFonts.emplace(fileName, font);
		}
		else
		{
			font->Unload();
			delete font;
			font = nullptr;
		}
		return font;
	}

}

void Game::LoadData()
{
	// Set temp background 
	/*
	* Steps:
	* 1. create a temp Actor instance, and set position to center of screen
	* 2. Create a new BGSpriteComponent and set temp as the input
	* 3. Set Screen Size to desired size
	* 4. Get textures, then set textures.
	* 5. Optional if there are more than one background repeat and set scroll speeds if desired.
	*/

	

	Font* font = new Font(this);
	font->Load("Assets/Caviar_Dreams_Bold.ttf");
	UIScreen* ui = new UIScreen(this);
	ui->SetFont(font);
	const std::string name = "Button";
	
	ui->LoadSelectedTex("Assets/blue_brick.png");
	ui->LoadUnSelectedTex("Assets/red_brick.png");
	ui->AddButton(name, &foo);

	Actor* actor = new Actor(this);
	TileMapComponent* tc = new TileMapComponent(actor, 100);
}

void Game::UnloadData()
{
	// Delete actors
	// Because ~Actor calls RemoveActor, have to use a different style loop
	std::cout << "Unload Data Called" << std::endl;
	while (!mActors.empty())
	{
		delete mActors.back();
	}
	std::cout << "Actors Removed" << std::endl;
	// Destroy textures
	for (auto i : mTextures)
	{
		SDL_DestroyTexture(i.second);
	}
	mTextures.clear();
	std::cout << "Textures cleared" << std::endl;

	while (!mUIStack.empty())
	{
		delete mUIStack.back();
		mUIStack.pop_back();
	}
	mFonts.clear();
	
}

void Game::RemoveActor(Actor* actor)
{
	auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
	if (iter != mPendingActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mPendingActors.end() - 1);
		mPendingActors.pop_back();
	}

	// Is it in actors?
	iter = std::find(mActors.begin(), mActors.end(), actor);
	if (iter != mActors.end())
	{
		// Swap to end of vector and pop off (avoid erase copies)
		std::iter_swap(iter, mActors.end() - 1);
		mActors.pop_back();
	}
}

void Game::PushUI(UIScreen* screen)
{
	mUIStack.emplace_back(screen);
}

void Game::ShutDown()
{
	std::cout << "Shutdown Called" << std::endl;
	UnloadData();
	std::cout << "Data Unloaded" << std::endl;
	IMG_Quit();
	std::cout << "Audio system shutdown called" << std::endl;
	mAudioSystem->ShutDown();
	// shut down renderer
	SDL_DestroyRenderer(mRenderer);
	std::cout << "Renderer Destroyed" << std::endl;
	// destroy the window
	SDL_DestroyWindow(mWindow);
	std::cout << "Window Destroyed" << std::endl;
	// Shut down the TTF
	TTF_Quit();
	std::cout << "TTF closed" << std::endl;
	// close the sdl with quit
	SDL_Quit();
	std::cout << "SDL closed" << std::endl;
}


void foo()
{
	SDL_Log("Clicked");
}