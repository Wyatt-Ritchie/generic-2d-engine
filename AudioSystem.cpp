#include "AudioSystem.h"
#include "SDL_log.h"
#include <fmod_studio.hpp>
#include <fmod_errors.h>
#include <vector>
AudioSystem::AudioSystem(Game* game) : mGame(game)
									  ,mSystem(nullptr)
									  ,mLowLevelSystem(nullptr)
{}

AudioSystem::~AudioSystem()
{
	ShutDown();
}

bool AudioSystem::Initialize()
{
	FMOD::Debug_Initialize(
		FMOD_DEBUG_LEVEL_ERROR, // log errors
		FMOD_DEBUG_MODE_TTY // output to stdout
	);

	FMOD_RESULT result;

	result = FMOD::Studio::System::create(&mSystem);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to create FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}

	result = mSystem->initialize(
		512,						// max number of channels
		FMOD_STUDIO_INIT_NORMAL,	// use default settings
		FMOD_INIT_NORMAL,			// use default settings
		nullptr
	);

	if (result != FMOD_OK)
	{
		SDL_Log("Failed to initialize FMOD system: %s", FMOD_ErrorString(result));
		return false;
	}
	mSystem->getCoreSystem(&mLowLevelSystem);

	return true;
}

void AudioSystem::Update(float deltaTime)
{
	if (mSystem)
	{
		mSystem->update();
	}
	
}

void AudioSystem::ShutDown()
{
	if (mSystem)
	{
		mSystem->release();
	}
	
}