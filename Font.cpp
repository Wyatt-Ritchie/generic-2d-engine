#include "Font.h"
#include "Game.h"

Font::Font(Game* game) : mGame(game)
{

}

Font::~Font()
{
	SDL_Log("Font deleted!");
}

bool Font::Load(const std::string& filename)
{
	// Support these font sizes
	std::vector<int> fontSizes = {
		8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28,
		30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 52, 56, 60,
		64, 68, 72
	};

	//call TTF_OpenFont once per every font size
	for (auto& size : fontSizes)
	{
		TTF_Font* font = TTF_OpenFont(filename.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Error from TTF_Font: %s\n", TTF_GetError());
			SDL_Log("Failed to load font %s in size %d", filename.c_str(), size);
			return false;
		}
		mFontData.emplace(size, font);
	}
	return true;
}

void Font::Unload()
{
	for (auto& font : mFontData)
	{
		TTF_CloseFont(font.second);
	}
}

SDL_Texture* Font::RenderText(const std::string& text, const Vector3& color, int pointSize)
{
	SDL_Texture* texture = nullptr;

	SDL_Color sdlColor;
	sdlColor.r = static_cast<Uint8>(color.x * 255);
	sdlColor.g = static_cast<Uint8>(color.y * 255);
	sdlColor.b = static_cast<Uint8>(color.z * 255);
	sdlColor.a = 255;

	// Find the point data for this point size
	auto iter = mFontData.find(pointSize);
	if (iter != mFontData.end())
	{
		TTF_Font* font = iter->second;

		// draw this to a surface
		SDL_Surface* surf = TTF_RenderText_Blended(font, text.c_str(), sdlColor);

		if (surf != nullptr)
		{
			// Convert from surface to texture
			texture = SDL_CreateTextureFromSurface(mGame->GetRenderer(), surf);
			SDL_FreeSurface(surf);
		}
	}
	else
	{
		SDL_Log("Point size %d is unsuported.", pointSize);
	}

	return texture;
}
