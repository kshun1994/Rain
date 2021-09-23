#include "rnpch.h"
#include "ActionState.h"
#include "Character.h"

void ActionState::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& spriteDims)
{
	animationFrameIDs_ = frameIDs;
	animationFrameDurations_ = durations;
	animationSpriteDims_ = spriteDims;
}

void ActionState::setAnimationRepeat(bool flag)
{
	animationIsRepeating_ = flag;
}

void ActionState::setBoxes(std::vector<std::shared_ptr<Box>> boxes)
{
	boxes_ = std::move(boxes);
}

void ActionState::appendBox(std::shared_ptr<Box> box)
{
	boxes_.push_back(std::move(box));
}

void ActionState::setFrameData(const int& startup, const int& active, const int& recovery)
{
	startup_	= startup;
	active_		= active;
	recovery_	= recovery;
}

void ActionState::setAnimation(Character& character)
{
	character.setAnimationFrames(animationFrameIDs_, animationFrameDurations_, animationSpriteDims_);
}

int ActionState::handleInput(Character& character, std::map<int, bool> stateMap)
{
	for (auto it = stateMap.rbegin(); it != stateMap.rend(); ++it)
	{
		if (it->second && (character.getCurrentActionStateID() != it->first))
		{
			// Iterate through all child nodes of Character and detach boxes; move back to ActionState
			//for (SceneNode* ptr : character.getChildren())
			//{
			//	if (ptr->getCategory() == Category::Box)
			//	{
			//		boxes_.push_back(std::static_pointer_cast<Box>(character.detachChild(*ptr)));
			//	}

			//}

			for (Box* box : boxPtrs_)
			{
				auto detachedBox = character.detachChild(*box);
				boxes_.push_back(std::static_pointer_cast<Box>(detachedBox));

				//if (box != nullptr)
				//{
				//	boxes_.push_back(static_cast_ptr<Box>(character.detachChild(*box)));
				//}
			}
			boxPtrs_.clear();

			//character.detachBoxes();

			character.setCurrentActionStateID(it->first);
			return it->first;
		}
	}

	return NULL_ACTION;
}

void ActionState::enter(Character& character)
{
	for (std::shared_ptr<Box>& box : boxes_)
	{
		boxPtrs_.push_back(box.get());
		character.attachChild(std::move(box));
	}
	boxes_.clear();
}

void StandState::enter(Character& character)
{
	ActionState::enter(character);
	setAnimation(character);
	RN_DEBUG("Entered StandState.");
}

void CrouchState::enter(Character& character)
{
	ActionState::enter(character);
	setAnimation(character);
	RN_DEBUG("Entered CrouchState.");
}

void FWalkState::update(Character& character)
{
	character.move(speed_ * character.getFacingSign(), 0.f);
}

void FWalkState::enter(Character& character)
{
	ActionState::enter(character);
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
	ActionState::enter(character);
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
