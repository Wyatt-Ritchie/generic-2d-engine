#include "UIScreen.h"
#include "Game.h"
#include "Font.h"
#include "Button.h"


UIScreen::UIScreen(Game* game) : mGame(game)
								,mFont(nullptr)
								,mTitle(nullptr)
								,mTitlePos(Vector2::Zero)
								,mState(EActive)
								,mNextButtonPos(Vector2(334, 334))
{
	game->PushUI(this);
}

UIScreen::~UIScreen()
{
	if (mFont)
	{
		mFont->Unload();
		delete mFont;
	}

	if (mButtonSelected)
	{
		SDL_DestroyTexture(mButtonSelected);
	}

	if (mButtonUnSelected)
	{
		SDL_DestroyTexture(mButtonUnSelected);
	}
}

void UIScreen::Update(float deltaTime)
{
}

void UIScreen::Draw(SDL_Renderer* renderer)
{

	// Iterate through mButtons and draw each button
	if (!mButtons.empty())
	{
		for (auto const & b : mButtons)
		{
			if (b->GetHighlighted())
			{
				b->Draw(mButtonSelected, renderer);
			}
			else
			{
				b->Draw(mButtonUnSelected, renderer);
			}
		}
	}

}

void UIScreen::ProcessInput(const uint8_t* keys)
{
	if (!mButtons.empty())
	{
		// Get mouse position
		int x, y;
		uint32_t state = SDL_GetMouseState(&x, &y);
		
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		// Highlight any buttons
		for (auto& b : mButtons)
		{
			if (b->ContainsPoint(mousePos))
			{
				b->SetHighlighted(true);
				
			}
			else
			{
				b->SetHighlighted(false);
			}
		}

		
	}
}

void UIScreen::HandleKeyPress(int key)
{
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty())
		{
			for (auto& b : mButtons)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
		break;
	default:
		break;
	}
}


void UIScreen::Close()
{
	mState = EClosing;
}

void UIScreen::SetTitle(const std::string& text, const Vector3& color, int pointSize)
{
}

void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(64, 64);
	int a, b;
	SDL_QueryTexture(mButtonSelected, NULL, NULL, &a, &b);
	dims.x = static_cast<float>(a);
	dims.y = static_cast<float>(b);
	

	Button* button = new Button(name, mFont, onClick, mNextButtonPos, dims);
	mNextButtonPos.y -= b + 20;
	mButtons.emplace_back(button);
}

void UIScreen::LoadSelectedTex(const std::string path)
{
	mButtonSelected = mGame->GetTexture(path);
}

void UIScreen::LoadUnSelectedTex(const std::string path)
{
	mButtonUnSelected = mGame->GetTexture(path);
}

void UIScreen::DrawTexture(SDL_Texture* texture, const Vector2& offset, float scale)
{
}
