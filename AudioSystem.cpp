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

	// Load the master banks (strings first)
	LoadBank("Assets/Master Bank.string.bank");
	LoadBank("Assets/Master Bank.bank");

	return true;
}

void AudioSystem::Update(float deltaTime)
{
	if (mSystem)
	{
		mSystem->update();
	}
	
}

void AudioSystem::LoadBank(const std::string& name)
{
	// Prevent double-loading
	if (mBanks.find(name) != mBanks.end())
	{
		return;
	}

	// Try to load bank
	FMOD::Studio::Bank* bank = nullptr;

	FMOD_RESULT result = mSystem->loadBankFile(
		name.c_str(),
		FMOD_STUDIO_LOAD_BANK_NORMAL, // Normal load
		&bank //save pointer to bank
	);

	const int maxPathLength = 512;	// same as the number of channels
	if (result == FMOD_OK)
	{
		// Add bank to the map
		mBanks.emplace(name, bank);

		// Load all non-streaming data
		bank->loadSampleData();

		// Get number of events in this bank
		int numEvents = 0;
		bank->getEventCount(&numEvents);

		if (numEvents > 0)
		{
			// Get list of event descriptions in this bank
			std::vector<FMOD::Studio::EventDescription*> events(numEvents);
			bank->getEventList(events.data(), numEvents, &numEvents);
			char eventName[maxPathLength];
			for (int i = 0; i < numEvents; i++)
			{
				FMOD::Studio::EventDescription* e = events[i];
				// Get the path of this event
				e->getPath(eventName, maxPathLength, nullptr);
				// Add the event to the map
				mEvents.emplace(eventName, e);
			}
		}
	}
}

void AudioSystem::UnloadBank(const std::string& name)
{
	auto iter = mBanks.find(name);
	if (iter == mBanks.end())
	{
		return;
	}

	// First remove all the events from this bank
	FMOD::Studio::Bank* bank = iter->second;
	int numEvents = 0;
	bank->getEventCount(&numEvents);

	if (numEvents > 0)
	{
		// Get event description for this bank
		std::vector<FMOD::Studio::EventDescription*> events(numEvents);
		//Get list of events
		bank->getEventList(events.data(), numEvents, &numEvents);
		char eventName[512];

		for (int i = 0; i < numEvents; i++)
		{
			FMOD::Studio::EventDescription* e = events[i];
			// get path of this event
			e->getPath(eventName, 512, nullptr);
			auto eventi = mEvents.find(eventName);
			if (eventi != mEvents.end())
			{
				mEvents.erase(eventi);
			}
		}
	}

	// Unload sample data and bank
	bank->unloadSampleData();
	bank->unload();
	// Remove from banks map
	mBanks.erase(iter);
}

void AudioSystem::UnloadAllBanks()
{
	for (auto& iter : mBanks)
	{
		iter.second->unloadSampleData();
		iter.second->unload();
	}
	mBanks.clear();
	mEvents.clear();
}

void AudioSystem::PlayEvent(const std::string& name)
{
	// Make sure event exists
	auto iter = mEvents.find(name);
	if (iter != mEvents.end())
	{
		//Create instance of the event
		FMOD::Studio::EventInstance* event = nullptr;
		iter->second->createInstance(&event);
		if (event)
		{
			// start the event instance
			event->start();
			// release schedules destruction of the event
			// instance when it stops.
			// (non looping events automatically stops)
			event->release();
		}
	}
}

void AudioSystem::ShutDown()
{
	if (mSystem)
	{
		mSystem->release();
	}
	
}