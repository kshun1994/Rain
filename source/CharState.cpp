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

std::shared_ptr<CharState> CharState::handleInput(Character& character, std::map<int, bool> stateMap)
{
	for (auto it = stateMap.rbegin(); it != stateMap.rend(); ++it)
	{
		if (it->second && (character.getCurrentCharState() != character.getCharStates()[it->first]))
		{
			return character.getCharStates()[it->first];
		}
	}

	return nullptr;
}

void StandState::enter(Character& character)
{
	setAnimation(character);
	RN_DEBUG("Entered StandState.");
}

void CrouchState::enter(Character& character)
{
	setAnimation(character);
	RN_DEBUG("Entered CrouchState.");
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
