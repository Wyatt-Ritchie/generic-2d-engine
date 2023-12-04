#pragma once
#include "SpriteComponent.h"
#include <vector>
#include "Math.h"
#include <string>
class AnimateSpriteComponent : public SpriteComponent
{
	// structure for representing a single animation
	struct Animation
	{
		std::string mAnimName = "";
		SDL_Texture* mSpriteSheet;
		int mStartPos, mEndPos;
		Vector2 mFrameSize;
		bool mLoop = true;
		bool flip = false;
	};

public:
	AnimateSpriteComponent(class Actor* owner, int drawOrder = 100);

	void Update(float deltaTime) override;

	void Draw(SDL_Renderer* renderer) override;

	void SetCurrentAnimation(const std::string animationName);

	Animation GetCurrentAnimation() const { return mCurrentAnimation; }

	void SetAnimationTextures(SDL_Texture* spriteSheet
		, const std::string animName
		, const bool looping
		, const Vector2 frameSize);

	float GetAnimFPS() const { return mAnimFPS; };
	void SetAnimFPS(const float fps) { mAnimFPS = fps; };

private:
	// All textures in the animation
	//std::vector<SDL_Texture*> mAnimSpriteSheets;

	// all the animations
	std::vector<Animation> mAnimations;

	Animation mCurrentAnimation;

	const Uint8* state;

	SDL_RendererFlip flip;

	// Current fram displayed
	float mCurrentFrame;

	// Animation FPS
	float mAnimFPS;
};

