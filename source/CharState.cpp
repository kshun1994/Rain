#include "rnpch.h"
#include "CharState.h"
#include "Character.h"


//void StandState::enter(Character& character)
//{
//	character.setPosture(Character::Posture::Standing);
//	RN_DEBUG("Entered StandState");
//}
//
//void StandState::update(Character& character)
//{
//}
//
//std::unique_ptr<CharState> StandState::handleInput(Character& character, int input)
//{
//	if (input & Input::Down)
//	{
//		std::unique_ptr<CharState> state = std::make_unique<CharState>(CrouchState());
//		return state;
//	}
//	if (input & Input::Up)
//	{
//		std::unique_ptr<CharState> state = std::make_unique<CharState>(AirState());
//		return state;
//	}
//
//	return nullptr;
//}
//
//void CrouchState::enter(Character& character)
//{
//	character.setPosture(Character::Posture::Crouching);
//	RN_DEBUG("Entered CrouchState");
//}
//
//void CrouchState::update(Character& character)
//{
//}
//
//std::unique_ptr<CharState> CrouchState::handleInput(Character& character, int input)
//{
//	if (!(input & Input::Down))
//	{
//		std::unique_ptr<CharState> state = std::make_unique<CharState>(StandState());
//		return state;
//	}
//	if (input & Input::Up)
//	{
//		std::unique_ptr<CharState> state = std::make_unique<CharState>(AirState());
//		return state;
//	}
//
//	return nullptr;
//}
//
//void AirState::enter(Character& character)
//{
//}
//
//void AirState::update(Character& character)
//{
//}
//
//std::unique_ptr<CharState> AirState::handleInput(Character& character, int input)
//{
//	return std::unique_ptr<CharState>();
//}

void AnimatedState::update(Character& character)
{
	// Every update, increment state progress
	progress_++;

	if (progress_ >= duration_)
	{
		progress_ = 0;

		// If progress exceeds duration and state doesn't repeat then exit the state
		if (!isRepeating_)
		{
			exit(character);
		}
	}
}

std::unique_ptr<CharState> AnimatedState::handleInput(Character& character, int input)
{
	return nullptr;
}

void AnimatedState::enter(Character& character)
{
	// Make sure the sum of animation sprite durations matches the state's total duration
	assert(std::accumulate(spriteDurations_.begin(), spriteDurations_.end(), 0) == duration_);

	// Set animation using parameters defined during state setup
	character.setAnimation(spriteIds_, spriteDurations_, spriteDims_, isRepeating_);

	// Set progress to zero
	progress_ = 0;
}

void AnimatedState::exit(Character& character)
{
	character.exitCharState();
}

void AnimatedState::setAnimationParameters(const std::vector<int>& spriteIds, const std::vector<int>& spriteDurations, const sf::Vector2i& spriteDims)
{
	// To be done in state setup; set parameters for animation switching upon state entry
	spriteIds_ = spriteIds;
	spriteDurations_ = spriteDurations;
	spriteDims_ = spriteDims;
}

void AnimatedState::setStateDuration(int duration)
{
	// To set the total duration of the state
	duration_ = duration;
}

void AnimatedState::setRepeating(bool flag)
{
	isRepeating_ = flag;
}
