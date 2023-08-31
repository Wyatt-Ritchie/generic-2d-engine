#pragma once

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

	//FMOD::System* getLowLevelSystem() const { return mLowLevelSystem; }
private:
	class Game* mGame;

	//FMOD 
	FMOD::Studio::System* mSystem;
	//FMOD low level 
	FMOD::System* mLowLevelSystem;
};

