#pragma once
#include "Math.h"
#include <string>
#include "SDL.h"
class UIScreen
{
public:
	UIScreen(class Game* game);
	virtual ~UIScreen();
	virtual void Update(float deltaTime);
	virtual void Draw(SDL_Renderer* renderer);
	virtual void ProcessInput(const uint8_t* keys);
	virtual void HandleKeyPress(int key);

	// Tracks if the UI is active or closing
	enum UIState{EActive, EClosing};
	// Set state to closing
	void Close();

	UIState GetState() const { return mState; }

	// Change the title text
	void SetTitle(const std::string& text,
		const Vector3& color = Color::White,
		int pointSize = 30);
protected:
	// Helper to drraw a texture
	void DrawTexture(class Shader* shader, class Texture* texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f);
	class Game* mGame;
	// For the UI screen's title text
	class Font* mFont;
	class Texture* mTitle;
	Vector2 mTitlePos;
	// State
	UIState mState;
private:

};

