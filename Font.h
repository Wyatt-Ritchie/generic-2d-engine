#pragma once
#include <string>
#include <unordered_map>
#include <SDL_ttf.h>
#include "Math.h"
#include "SDL.h"

class Font
{
public:
	Font(class Game* game);
	~Font();

	// Load/unload from a file
	bool Load(const std::string& filename);
	void Unload();

	// Given string and this font, draw to a texture
	SDL_Texture* RenderText(const std::string& text,
		const Vector3& color = Color::White,
		int pointSize = 30);
private:

	// Map of point sizes to font data
	std::unordered_map<int, TTF_Font*> mFontData;
	class Game* mGame;
};

