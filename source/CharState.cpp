#include "rnpch.h"
#include "CharState.h"
#include "Character.h"

void CharState::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& spriteDims)
{
	animationFrameIDs_ = frameIDs;
	animationFrameDurations_ = durations;
	animationSpriteDims_ = spriteDims;
}

void CharState::setAnimationRepeat(bool flag)
{
	animationIsRepeating_ = flag;
}

void CharState::setAnimation(Character& character)
{
	character.setAnimationFrames(animationFrameIDs_, animationFrameDurations_, animationSpriteDims_);
}

CharState* StandState::handleInput(Character& character, int input)
{
	if ((input & 15) <= 3) // If downward input
	{
		return character.getCharStates()[COMMON_ACTION_CROUCH]; // TODO do not hardcode the indices for character states
	}

	if ((input & 15) == 6) // To forward walk 
	{
		return character.getCharStates()[COMMON_ACTION_F_WALK];
	}

	if ((input & 15) == 4) // To back walk 
	{
		return character.getCharStates()[COMMON_ACTION_B_WALK];
	}

	return nullptr;
}

void StandState::update(Character& character)
{
}

void StandState::enter(Character& character)
{
	setAnimation(character);
	RN_DEBUG("Entered StandState.");
}

CharState* CrouchState::handleInput(Character& character, int input)
{
	if ((input & 15) == 5) // If no downward input, to stand
	{
		RN_DEBUG("Returned standing from crouch.");
		return character.getCharStates()[COMMON_ACTION_STAND]; // Return standing state to Character
	}

	if ((input & 15) == 6) // To forward walk 
	{
		return character.getCharStates()[COMMON_ACTION_F_WALK];
	}

	if ((input & 15) == 4) // To back walk 
	{
		return character.getCharStates()[COMMON_ACTION_B_WALK];
	}

	return nullptr;
}

void CrouchState::update(Character& character)
{
}

void CrouchState::enter(Character& character)
{
	setAnimation(character);
	RN_DEBUG("Entered CrouchState.");
}

CharState* FWalkState::handleInput(Character& character, int input)
{
	if ((input & 15) == 5) // To stand
	{
		return character.getCharStates()[COMMON_ACTION_STAND];
	}

	if ((input & 15) <= 3) // To crouch
	{
		return character.getCharStates()[COMMON_ACTION_CROUCH]; 
	}

	if ((input & 15) == 4) // To back walk 
	{
		return character.getCharStates()[COMMON_ACTION_B_WALK];
	}

	return nullptr;
}

void FWalkState::update(Character& character)
{
	character.move((character.getFacing() == Character::Facing::Right) ? speed_ : -speed_, 0.f);
}

void FWalkState::enter(Character& character)
{
	setAnimation(character);
	RN_DEBUG("Entered FWalkState.");
}

void FWalkState::setSpeed(float speed)
{
	speed_ = speed;
}

CharState* BWalkState::handleInput(Character& character, int input)
{
	if ((input & 15) == 5) // To stand
	{
		return character.getCharStates()[COMMON_ACTION_STAND];
	}

	if ((input & 15) <= 3) // To crouch
	{
		return character.getCharStates()[COMMON_ACTION_CROUCH]; 
	}

	if ((input & 15) == 6) // To forward walk
	{
		return character.getCharStates()[COMMON_ACTION_F_WALK]; 
	}

	return nullptr;
}

void BWalkState::update(Character& character)
{
	character.move((character.getFacing() == Character::Facing::Right) ? -speed_ : speed_, 0.f);
}

void BWalkState::enter(Character& character)
{
	setAnimation(character);
	RN_DEBUG("Entered BWalkState.");
}

void BWalkState::setSpeed(float speed)
{
	speed_ = speed;
}

/*

StandState
CrouchState
FWalkState
BWalkState
JumpState
5AState

StandState -> 5AState
5AState !-> FwalkState
JumpState !->CrouchState

Categorize

StandState & CrouchState & Fwalk & Bwalk ---> ActionableStates
ActionableState -> 5A

5AState & JumpState & FDashState ---> NonActionableState
	These states need Durations
	They need timepoints at which they are or are not cancellable into specific other states

Actionable ---> NormalAttacks

if CurrentState == Actionable
	look thru states to see which are available
	from that filtered list, see if any of the player's inputs match
	if so, return matching state

NonActionable !---> NormalAttacks

States own member variable Property

Head Invul
Throw Invul
Counterhittable

JumpState has property Throw Invul

Throw hitboxes check opponent current state to see if state has Throw Invul

*/
