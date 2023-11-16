#pragma once
#include "UIScreen.h"
#include <vector>
class TextBox : public UIScreen
{
public:
	TextBox(class Game* game);
	~TextBox();

	// Load the text from the file, and add each word into 
	// the mWords vector by a white space delimeter.
	// Keep formating characters such as \n and \t
	void LoadText(const std::string& filePath);

	// Load the font from the intended file into the mFont object
	void LoadFont(const std::string& filePath);

	// Draw
	void Draw(SDL_Renderer* renderer) override;

	// Get the width(x) and height(y) of the box as a Vector2
	Vector2 GetBoxDimensions();

	// Getters/Setters
	int GetFontSize() const { return mFontSize; }
	int GetBoxHeight() const { return mBoxHeight; }
	int GetBoxWidth() const { return mBoxWidth; }
	Vector2 GetBoxPosistion() const { return mBoxPosition; }
	void SetFontSize(int size) { mFontSize = size; }
	void SetBoxHeight(int height) { mBoxHeight = height; }
	void SetBoxWidth(int width) { mBoxWidth = width; }
	void SetBoxPosition(Vector2 position) { mBoxPosition = position; }
	void SetTextFilePath(const std::string filePath) { mTextFilePath = filePath; }
	void SetFontFilePath(const std::string filePath) { mFontFilePath = filePath; }

private:
	// File paths for the Text file, and the font file
	std::string mTextFilePath;
	std::string mFontFilePath;

	std::vector<std::string> mWords;

	// Texture generated from the font
	SDL_Texture* mTexture;

	// Font size
	int mFontSize;

	// Texture height and width derived from SDL_QueryTexture
	int mTexHeight;
	int mTexWidth;

	// The dimensions of the box
	int mBoxHeight;
	int mBoxWidth;
	Vector2 mBoxPosition;
};

