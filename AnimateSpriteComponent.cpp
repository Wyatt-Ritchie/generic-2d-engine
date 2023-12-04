#include "AnimateSpriteComponent.h"
#include <iostream>
#include "Actor.h"

AnimateSpriteComponent::AnimateSpriteComponent(Actor* actor, int drawOrder) : SpriteComponent(actor, drawOrder)
, mCurrentFrame(0.0f)
, mAnimFPS(10.0f)
, mCurrentAnimation()
, state(SDL_GetKeyboardState(NULL))
, flip(SDL_FLIP_NONE)
{
	std::vector<Animation> mAnimations;
	Animation tmp;
	tmp.mAnimName = "tmp";
	mCurrentAnimation = tmp;
}

void AnimateSpriteComponent::Update(float deltaTime)
{
	//SpriteComponent::Update(deltaTime);

	if (mAnimations.size() > 0)
	{
		// if the animation is non looping and has reached then last frame, do nothing
		if (mCurrentFrame >= mCurrentAnimation.mEndPos && !mCurrentAnimation.mLoop)
		{
			return;
		}
		else {
			// update current frame based on frame rate
			// and delta time
			mCurrentFrame += mAnimFPS * deltaTime;
		}

		// if the animation is looping and the last frame is reached, return to the first
		// frame of the animation
		if (static_cast<int>(mCurrentFrame) >= mCurrentAnimation.mEndPos && mCurrentAnimation.mLoop)
		{
			mCurrentFrame = static_cast<float>(mCurrentAnimation.mStartPos);
		}

		// set the current texture
		// setTexture(mAnimSpriteSheets[static_cast<int>(mCurrentFrame)]);
	}
}

void AnimateSpriteComponent::SetAnimationTextures(SDL_Texture* spriteSheet
	, const std::string animName
	, const bool looping
	, const Vector2 frameSize)
{
	// creating a vector of animations to draw from
	int sheetWidth, sheetHeight;
	Animation anim;
	SDL_QueryTexture(spriteSheet, nullptr, nullptr, &sheetWidth, &sheetHeight);

	anim.mAnimName = animName;
	anim.mLoop = looping;
	anim.flip = false;
	anim.mStartPos = 1;
	anim.mEndPos = sheetWidth / static_cast<int>(frameSize.x);
	anim.mSpriteSheet = spriteSheet;
	anim.mFrameSize = frameSize;
	mAnimations.push_back(anim);
	std::cout << "Added " << animName << " to the animation vector" << std::endl;

}

void AnimateSpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mCurrentAnimation.mAnimName == "tmp")
	{
		return;
	}
	SDL_Rect rDest;
	SDL_Rect rSrc;
	// Scale the width and height of the tile on the screen
	rDest.w = 90;
	rDest.h = 90;

	rDest.x = static_cast<int>(mOwner->GetPosition().x - rDest.w / 2);
	rDest.y = static_cast<int>(mOwner->GetPosition().y - rDest.h / 2);

	// The width and height of the tile on the tile set
	rSrc.w = static_cast<int>(mCurrentAnimation.mFrameSize.x);
	rSrc.h = static_cast<int>(mCurrentAnimation.mFrameSize.y);

	rSrc.x = static_cast<int>(mCurrentFrame) % static_cast<int>(mCurrentAnimation.mFrameSize.x) * mCurrentAnimation.mFrameSize.x;
	rSrc.y = 0;

	// Draw
	SDL_RenderCopyEx(renderer,
		mCurrentAnimation.mSpriteSheet,
		&rSrc,
		&rDest,
		-Math::ToDegrees(mOwner->GetRotation()),
		nullptr,
		flip);
}

void AnimateSpriteComponent::SetCurrentAnimation(const std::string animationName)
{
	std::cout << "Setting current animation to " << animationName << "\n";


	for (const auto& a : mAnimations)
	{
		if (a.mAnimName == animationName)
		{
			mCurrentAnimation = a;
			mCurrentFrame = 1.0f;
			return;
		}
	}
}