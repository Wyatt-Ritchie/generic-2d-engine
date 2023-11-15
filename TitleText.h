#pragma once
#include "UIScreen.h"
#include <string>
#include "Math.h"
class TitleText : public UIScreen
{
public:
	TitleText(Game* game);
	~TitleText();
	void SetPosition(Vector2 pos) { mPosition = pos; }
	Vector2 GetPosition() const { return mPosition; }
	void SetDrawBox(bool flag) { mDrawBox = flag; }

	void Draw(SDL_Renderer* renderer) override;
private:
	const std::string mText;
	bool mDrawBox;
	Vector2 mPosition;
	SDL_Texture* mTexture;
	int mTexHeight;
	int mTexWidth;
};

