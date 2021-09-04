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
		return character.getCharStates()[1];
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
	if ((input & 15) > 3) // If no downward input
	{
		return character.getCharStates()[0]; // Return standing state to Character
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
