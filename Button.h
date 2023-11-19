#pragma once
#include <string>
#include <functional>
#include "Math.h"
#include "SDL.h"
class Button
{
public:
	// The constructor takes in a name, font,
	// callback function and the position/dimension of the button
	Button(const std::string& name, class Font* font,
		std::function<void()> onClick,
		const Vector2& pos, const Vector2& dim);
	~Button();

	// Set the name of the button and generate the name texture
	void SetName(const std::string& name);

	// Returns true if the point is within the button's bounds
	bool ContainsPoint(const Vector2& pt) const;
	
	// Called when button is clicked
	void OnClick();

	// Draw the button and associated text
	void Draw(SDL_Texture* tex, SDL_Renderer* renderer);

	// Getters/Setters
	SDL_Texture* GetNameTexture() const { return mNameTexture; }
	Font* GetFont() const { return mFont; }
	Vector2 GetDimension() const { return mDimension; }
	Vector2 GetPosition() const { return mPosition; }
	bool GetHighlighted() const { return mHighlighted; }
	std::string GetName() const { return mName; }
	std::function<void()> GetOnClick() const { return mOnClick; }
	void SetNameTexture(SDL_Texture* tex) { mNameTexture = tex; }
	void SetFont(Font* font) { mFont = font; }
	void SetPosition(Vector2 pos) { mPosition = pos; }
	void SetDimension(Vector2 dim) { mDimension = dim; }
	void SetHighlighted(bool highlight) { mHighlighted = highlight; }

private:
	std::function<void()> mOnClick;
	std::string mName;
	SDL_Texture* mNameTexture;
	int mNameTexWidth;
	int mNameTexHeight;
	class Font* mFont;
	Vector2 mPosition;
	Vector2 mDimension;
	bool mHighlighted;

};

