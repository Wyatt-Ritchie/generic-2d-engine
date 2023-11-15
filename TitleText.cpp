#include "TitleText.h"
#include "Font.h"
#include <memory>

TitleText::TitleText(Game* game) : UIScreen(game)
								  ,mText("Hello World")
								  ,mPosition(Vector2::Zero)
								  ,mTexture(nullptr)
								  ,mTexHeight(0)
								  ,mTexWidth(0)
								  ,mDrawBox(true)
{
	mFont = new Font(mGame);
	const std::string path = "Assets/CaviarDreams.ttf";
	mFont->Load(path);
	SetPosition(Vector2(500.0f, 300.0f));
	mTexture = mFont->RenderText(mText, Color::Red, 20);
	SDL_QueryTexture(mTexture, nullptr, nullptr, &mTexWidth, &mTexHeight);
}

TitleText::~TitleText()
{
}

void TitleText::Draw(SDL_Renderer* renderer)
{

	if (mTexture)
	{
		SDL_Rect r;

		// Scale the width and height by owner scale
		r.w = static_cast<int>(mTexWidth);
		r.h = static_cast<int>(mTexHeight);
		// Center the rectangle around the position of the owner
		r.x = static_cast<int>(GetPosition().x - r.w / 2);
		r.y = static_cast<int>(GetPosition().y - r.h / 2);

		// Draw
		SDL_RenderCopyEx(renderer,
			mTexture,
			nullptr,
			&r,
			0.0,
			nullptr,
			SDL_FLIP_NONE);

		if (!mDrawBox)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderDrawRect(renderer, &r);
		}
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(renderer, r.x, r.y + 35, r.x + 50, r.y + 35);
	}
}
