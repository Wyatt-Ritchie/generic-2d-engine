#include "TextBox.h"
#include "Font.h"
#include "Game.h"
#include <iostream>
#include <fstream>

TextBox::TextBox(Game* game) : UIScreen(game)
							  ,mTexture(nullptr)
							  ,mBoxHeight(300)
							  ,mBoxWidth(300)
							  ,mTexHeight(0)
							  ,mTexWidth(0)
							  ,mFontSize(12)
							  ,mTextFilePath("")
							  ,mFontFilePath("")
							  ,mBoxPosition(0, 0)
{
	mFont = new Font(game);
}

TextBox::~TextBox()
{
}

void TextBox::LoadText(const std::string& filePath)
{
	// Open the file and add each word into the words vector
	std::ifstream file(filePath);
	if (file)
	{
		std::string word;
		while (file >> word)
		{
			mWords.push_back(word);
		}
	}
	else
	{
		SDL_Log("Error opening file at file path %s", filePath.c_str());
		return;
	}
	file.close();

	return;
}

void TextBox::LoadFont(const std::string& filePath)
{
	// Call the Get Font from the game class
	// this puts the font in the font map for 
	// later use/checks to see if it is already loaded
	try {
		mFont = mGame->GetFont(filePath);
		if (!mFont)
		{
			throw;
		}
	}
	catch (...)
	{
		SDL_Log("Font was unable to load");
	}
}

Vector2 TextBox::GetBoxDimensions()
{
	return Vector2(static_cast<float>(mBoxWidth), static_cast<float>(mBoxHeight));
}

void TextBox::Draw(SDL_Renderer* renderer)
{

	Vector2 dim = GetBoxDimensions();
	Vector2 pos = GetBoxPosistion();

	SDL_Rect r;
	r.w = (int)dim.x;
	r.h = (int)dim.y;
	r.x = (int)pos.x;
	r.y = (int)pos.y;

	int x = r.x + r.w;
	int y = 0;

	for (const auto& word : mWords) {
		//if (y >= mBoxHeight) return;
		// Render each word
		mTexture = mFont->RenderText(word, Color::White, mFontSize);

		// Get the width and height of the text
		SDL_QueryTexture(mTexture, NULL, NULL, &mTexWidth, &mTexHeight);

		// Check if the word goes beyond the box width
		if (x + mTexWidth > mBoxWidth) {
			// Move to the next line
			x = 20;
			y += mTexHeight + 5;  // Add some spacing between lines
		}
		
		// Render the text
		SDL_Rect dstRect = { x, y, mTexWidth, mTexHeight };
		SDL_RenderCopyEx(renderer,
			mTexture,
			nullptr,
			&dstRect,
			0.0,
			nullptr,
			SDL_FLIP_NONE);

		// Update horizontal position for the next word
		x += mTexWidth + 5;  // Add some spacing between words
		
		// Free resources
		SDL_DestroyTexture(mTexture);
	}
}

