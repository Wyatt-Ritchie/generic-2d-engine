#include "UIScreen.h"
#include "Game.h"
#include "Font.h"
#include "Button.h"


UIScreen::UIScreen(Game* game) : mGame(game)
								,mFont(nullptr)
								,mTitle(nullptr)
								,mTitlePos(Vector2::Zero)
								,mState(EActive)
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
}

void UIScreen::Update(float deltaTime)
{
}

void UIScreen::Draw(SDL_Renderer* renderer)
{
}

void UIScreen::ProcessInput(const uint8_t* keys)
{
	if (!mButtons.empty())
	{
		// Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetScreenHeight() * 0.5f - mousePos.y;

		// Highlight any buttons
		for (auto b : mButtons)
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
	Vector2 dims(0, 0);
	int a, b;
	SDL_QueryTexture(mButtonSelected, NULL, NULL, &a, &b);
	dims.x = static_cast<float>(a);
	dims.y = static_cast<float>(b);

	Button* button = new Button(name, mFont, onClick, mNextButtonPos, dims);
	mNextButtonPos.y -= b + 20;
	mButtons.emplace_back(button);
}

void UIScreen::DrawTexture(Shader* shader, Texture* texture, const Vector2& offset, float scale)
{
}
