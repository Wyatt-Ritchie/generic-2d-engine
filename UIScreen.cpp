#include "UIScreen.h"
#include "Game.h"

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
}

void UIScreen::Update(float deltaTime)
{
}

void UIScreen::Draw(SDL_Renderer* renderer)
{
}

void UIScreen::ProcessInput(const uint8_t* keys)
{
}

void UIScreen::HandleKeyPress(int key)
{
}


void UIScreen::Close()
{
}

void UIScreen::SetTitle(const std::string& text, const Vector3& color, int pointSize)
{
}

void UIScreen::DrawTexture(Shader* shader, Texture* texture, const Vector2& offset, float scale)
{
}
