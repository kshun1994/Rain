#include "rnpch.h"
#include "ActionState.h"
#include "Character.h"

ActionState::ActionState()
: animationDoesLoop_(false)
, animationIsLooping_(false)
, progress_(0)
, properties_()
{
}

void ActionState::update(Character& character)
{
	// If there is a part of the action that loops, check if progress_ has reached that part and if so set the animation frames to just the looping part
	if (animationDoesLoop_ && !animationIsLooping_ && (std::find(animationLoopFrames_.begin(), animationLoopFrames_.end(), progress_) != animationLoopFrames_.end()))
	{
		animationIsLooping_ = true;
		// Use range constructor to slice original Frame ID vector
		std::vector<int> newFrames(animationFrameIDs_.begin() + animationLoopFrames_.front(), 
								   animationFrameIDs_.begin() + animationLoopFrames_.back());
		std::vector<int> newDurations(animationFrameDurations_.begin() + animationLoopFrames_.front(), 
									  animationFrameDurations_.begin() + animationLoopFrames_.back());
		character.setAnimationFrames(newFrames, newDurations, animationSpriteDims_);
		character.setAnimationRepeat(true);
		RN_DEBUG("Animation loop set!");
	}

	++progress_;
}

void ActionState::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& spriteDims)
{
	animationFrameIDs_ = frameIDs;
	animationFrameDurations_ = durations;
	animationSpriteDims_ = spriteDims;
}

void ActionState::setAnimationLoop(const bool& flag)
{
	// In case of no arguments to function set entire animation as loop range
	animationDoesLoop_ = flag;
	animationLoopFrames_.resize(animationFrameIDs_.size());
	std::iota(animationLoopFrames_.begin(), animationLoopFrames_.end(), 0);
}

void ActionState::setAnimationLoop(const std::vector<int>& loopFrames)
{
	animationDoesLoop_ = true;
	animationLoopFrames_ = loopFrames;
}

void ActionState::setBoxes(std::vector<std::shared_ptr<Box>> boxes)
{
	boxes_ = std::move(boxes);
}

void ActionState::appendBox(std::shared_ptr<Box> box)
{
	boxes_.push_back(std::move(box));
}

void ActionState::setAnimation(Character& character)
{
	character.setAnimationFrames(animationFrameIDs_, animationFrameDurations_, animationSpriteDims_);
}

void ActionState::setAnimation(Character& character, const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& spriteDims)
{
	character.setAnimationFrames(frameIDs, durations, spriteDims);
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

			progress_ = 0;
			animationIsLooping_ = false;

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
	ActionState::update(character);
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
	ActionState::update(character);
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
