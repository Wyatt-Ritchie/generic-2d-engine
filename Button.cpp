#include "Button.h"
#include "Font.h"

Button::Button(const std::string& name, Font* font, std::function<void()> onClick, const Vector2& pos, const Vector2& dim) : mName(name)
																															,mFont(font)
																															,mNameTexture(nullptr)
																															,mPosition(pos)
																															,mDimension(dim)
																															,mHighlighted(false)
																															,mOnClick(onClick)
																															,mNameTexHeight(0)
																															,mNameTexWidth(0)
{
	SetName(name);
	SDL_Log("Button named: [%s] has been created.", name.c_str());
}

Button::~Button()
{
	SDL_DestroyTexture(mNameTexture);
}

void Button::SetName(const std::string& name)
{
	if (!mFont)
	{
		SDL_Log("Font is null");
		return;
	}
	{
		mName = name;

		if (mNameTexture)
		{
			SDL_DestroyTexture(mNameTexture);
			mNameTexture = nullptr;
		}
		mNameTexture = mFont->RenderText(mName, Color::White, 30);
		SDL_QueryTexture(mNameTexture, NULL, NULL, &mNameTexWidth, &mNameTexHeight);
	}
}

bool Button::ContainsPoint(const Vector2& pt) const
{
	bool no = pt.x < (mPosition.x - mDimension.x / 2.0f) ||
		pt.x > (mPosition.x + mDimension.x / 2.0f) ||
		pt.y < (mPosition.y - mDimension.y / 2.0f) ||
		pt.y > (mPosition.y + mDimension.y / 2.0f);
	return !no;
}

void Button::OnClick()
{
	if (mOnClick)
	{
		mOnClick();
	}
}

void Button::Draw(SDL_Texture* tex, SDL_Renderer* renderer)
{
	Vector2 dim = GetDimension();
	Vector2 pos = GetPosition();

	SDL_Rect r;
	r.w = (int)dim.x;
	r.h = (int)dim.y;
	r.x = (int)pos.x - dim.x/2;
	r.y = (int)pos.y - dim.y/2;

	SDL_RenderCopyEx(renderer,
		tex,
		nullptr,
		&r,
		0.0,
		nullptr,
		SDL_FLIP_NONE);

	int x = 0;
	int y = 0;

	if (!mNameTexture)
	{
		SetName(mName);
	}

	SDL_Rect dstRect;
	dstRect.w = mNameTexWidth;
	dstRect.h = mNameTexHeight;
	dstRect.x = r.x + (r.w - dstRect.w)/2;
	dstRect.y = r.y + (r.h - dstRect.h)/2;

	SDL_RenderCopyEx(renderer,
		mNameTexture,
		nullptr,
		&dstRect,
		0.0,
		nullptr,
		SDL_FLIP_NONE);

}
