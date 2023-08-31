#pragma once
#include <unordered_map>
#include <string>
namespace FMOD
{
	class System;
	namespace Studio
	{
		class Bank;
		class EventDescription;
		class EventInstance;
		class System;
		class Bus;
	}
}

class AudioSystem
{
public:
	AudioSystem(class Game* game);
	~AudioSystem();

	bool Initialize();
	void ShutDown();

	void Update(float deltaTime);

	// Function for loading the bank, and sample data
	void LoadBank(const std::string& name);
	void UnloadBank(const std::string& name);
	void UnloadAllBanks();

	void PlayEvent(const std::string& name);

	//FMOD::System* getLowLevelSystem() const { return mLowLevelSystem; }
private:
	class Game* mGame;

	//FMOD 
	FMOD::Studio::System* mSystem;
	//FMOD low level 
	FMOD::System* mLowLevelSystem;

	// Banks
	// Map of loaded banks. The string is the filename of the bank
	std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
	// Map of event name to event description. The string is the name assigned
	// by FMOD for the event. 
	std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
};

