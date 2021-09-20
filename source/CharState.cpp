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

void CharState::setBoxes(std::vector<std::shared_ptr<Box>> boxes)
{
	boxes_ = boxes;
}

void CharState::appendBox(std::shared_ptr<Box> box)
{
	boxes_.push_back(box);
}

void CharState::setAnimation(Character& character)
{
	character.setAnimationFrames(animationFrameIDs_, animationFrameDurations_, animationSpriteDims_);
}

//std::shared_ptr<CharState> CharState::handleInput(Character& character, std::map<int, bool> stateMap)
//{
//	for (auto it = stateMap.rbegin(); it != stateMap.rend(); ++it)
//	{
//		if (it->second && (character.getCurrentCharState() != character.getCharStates()[it->first]))
//		{
//			return character.getCharStates()[it->first];
//		}
//	}
//
//	return nullptr;
//}

int CharState::handleInput(Character& character, std::map<int, bool> stateMap)
{
	for (auto it = stateMap.rbegin(); it != stateMap.rend(); ++it)
	{
		if (it->second && (character.getCurrentCharStateID() != it->first))
		{
			// Iterate through all child nodes of Character and detach boxes; move back to CharState
			//for (SceneNode* ptr : character.getChildren())
			//{
			//	if (ptr->getCategory() == Category::Box)
			//	{
			//		boxes_.push_back(std::static_pointer_cast<Box>(character.detachChild(*ptr)));
			//	}

			//}

			//for (auto box : boxes_)
			//{
			//	//character.detachChild(*box.get());
			//}

			//character.detachBoxes();

			character.setCurrentCharStateID(it->first);
			return it->first;
		}
	}

	return NULL_ACTION;
}

void CharState::enter(Character& character)
{
	for (std::shared_ptr<Box>& box : boxes_)
	{
		character.attachChild(box);
	}
}

void StandState::enter(Character& character)
{
	CharState::enter(character);
	setAnimation(character);
	RN_DEBUG("Entered StandState.");
}

void CrouchState::enter(Character& character)
{
	CharState::enter(character);
	setAnimation(character);
	RN_DEBUG("Entered CrouchState.");
}

void FWalkState::update(Character& character)
{
	character.move(speed_ * character.getFacingSign(), 0.f);
}

void FWalkState::enter(Character& character)
{
	CharState::enter(character);
	setAnimation(character);
	RN_DEBUG("Entered FWalkState.");
}

void FWalkState::setSpeed(float speed)
{
	speed_ = speed;
}

void BWalkState::update(Character& character)
{
	character.move(-speed_ * character.getFacingSign(), 0.f);
}

void BWalkState::enter(Character& character)
{
	CharState::enter(character);
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
