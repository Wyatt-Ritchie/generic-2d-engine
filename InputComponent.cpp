#include "InputComponent.h"
#include "Math.h"
#include <iostream>
#include "Actor.h"
InputComponent::InputComponent(Actor* actor) : MoveComponent(actor)
											  ,mMaxAngularSpeed(Math::Pi)
											  
											  
{
}

void InputComponent::ProcessInput(const uint8_t* keyState)
{
	// calculate the forward speed of movecomponent

	if (keyState[mForwardKey])
	{
		if (GetVelocity().Length() < (mOwner->GetForward()*GetMaxVelocity()).Length())
		{
			float mag = 80.0f;
			Vector2 forward = mOwner->GetForward();
			AddForce(forward * mag);
		}
	}

	float angularSpeed = 0.0f;
	if (keyState[mClockwiseKey])
	{
		angularSpeed += mMaxAngularSpeed;
	}
	if (keyState[mCounterClockwiseKey])
	{
		angularSpeed -= mMaxAngularSpeed;
	}

	SetAngularSpeed(angularSpeed);
}