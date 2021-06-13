#include "rnpch.h"
#include "CharState.h"
#include "Character.h"


void StandState::enter(Character& character)
{
	character.setPosture(Character::Posture::Standing);
	RN_DEBUG("Entered StandState");
}

void StandState::update(Character& character)
{
}

std::unique_ptr<CharState> StandState::handleInput(Character& character, int input)
{
	if (input & Input::Down)
	{
		std::unique_ptr<CharState> state = std::make_unique<CharState>(CrouchState());
		return state;
	}
	if (input & Input::Up)
	{
		std::unique_ptr<CharState> state = std::make_unique<CharState>(AirState());
		return state;
	}

	return nullptr;
}

void CrouchState::enter(Character& character)
{
	character.setPosture(Character::Posture::Crouching);
	RN_DEBUG("Entered CrouchState");
}

void CrouchState::update(Character& character)
{
}

std::unique_ptr<CharState> CrouchState::handleInput(Character& character, int input)
{
	if (!(input & Input::Down))
	{
		std::unique_ptr<CharState> state = std::make_unique<CharState>(StandState());
		return state;
	}
	if (input & Input::Up)
	{
		std::unique_ptr<CharState> state = std::make_unique<CharState>(AirState());
		return state;
	}

	return nullptr;
}

void AirState::enter(Character& character)
{
}

void AirState::update(Character& character)
{
}

std::unique_ptr<CharState> AirState::handleInput(Character& character, int input)
{
	return std::unique_ptr<CharState>();
}
