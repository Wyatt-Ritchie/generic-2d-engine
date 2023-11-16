#include "Button.h"
#include "Font.h"

Button::Button(const std::string& name, Font* font, std::function<void()> onClick, const Vector2& pos, const Vector2& dim) : mName("")
																															,mFont(nullptr)
																															,mNameTexture(nullptr)
																															,mPosition(Vector2::Zero)
																															,mDimension(Vector2::Zero)
																															,mHighlighted(false)
{
}

Button::~Button()
{
}

void Button::SetName(const std::string& name)
{
	{
		mName = name;

		if (mNameTexture)
		{
			SDL_DestroyTexture(mNameTexture);
			delete mNameTexture;
			mNameTexture = nullptr;
		}
		mNameTexture = mFont->RenderText(mName);
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
