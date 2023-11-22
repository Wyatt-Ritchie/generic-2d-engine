#pragma once
#include "Math.h"
#include <functional>
#include <string>
#include "SDL.h"
#include <vector>
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

	// Function for adding buttons
	void AddButton(const std::string& name,
		std::function<void()> onClick);

	void SetFont(class Font* font) { mFont = font; }

	void LoadSelectedTex(const std::string path);
	void LoadUnSelectedTex(const std::string path);
protected:
	// Helper to drraw a texture
	void DrawTexture(SDL_Texture* texture,
		const Vector2& offset = Vector2::Zero,
		float scale = 1.0f);
	class Game* mGame;
	// For the UI screen's title text
	class Font* mFont;
	class Texture* mTitle;
	Vector2 mTitlePos;
	Vector2 mNextButtonPos;
	// State
	UIState mState;

	// Vector of buttons
	std::vector<class Button*> mButtons;
	SDL_Texture* mButtonSelected;
	SDL_Texture* mButtonUnSelected;
private:

};

