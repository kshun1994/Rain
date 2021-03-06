#include "rnpch.h"
#include "Action.h"
#include "Character.h"


Action::Action()
: animationDoesLoop_(false)
, loopBounds_(0, 0)
, nextLoopBound_()
, currentFrame_(-1) // Initialized to -1 because update() begins by incrementing this by 1
, properties_()
, cancelType_(0)
, cancels_(0)
, velocityPerFrame_()
, currentBoxesInd_()
, destinationActionID_(0)
{
}

void Action::update(Character& character)
{
	++currentFrame_;

	if (character.getType() == Character::Type::Enkidu)
	{
		//RN_DEBUG("CurrentFrame - {}", currentFrame_);
	}

	// Check to see if Action needs to be looped; this has to go at the very end before incrementing currentFrame_
	if ((loopBounds_.first != loopBounds_.second) && (currentFrame_ == loopBounds_.second))
	{
		// Assumes Animation updates after Action in Character::update()
		character.setCurrentAnimationTick(loopBounds_.first);
		currentFrame_ = loopBounds_.first; // Since it's going to be incremented at the end of this anyway
	}

	// Calculate current frame velocity and move the Entity that amount this frame
	sf::Vector2f currentVelocity = calculateVelocity(character.getGravity());
	character.move(character.getFacingSign() * currentVelocity.x, -1 * currentVelocity.y);

	updateBoxes(character);
}

void Action::setAnimationFrames(const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& spriteDims)
{
	animationFrameIDs_ = frameIDs;
	animationFrameDurations_ = durations;
	animationSpriteDims_ = spriteDims;

	// Initialize vectors
	int actionDuration = sum_vector(durations);
	properties_.resize(actionDuration, Action::Property::None);
	velocityPerFrame_.resize(actionDuration, sf::Vector2f(0.f, 0.f));
	boxes_.resize(actionDuration);
	cancels_.resize(actionDuration);
}

// Set loop bounds for Action's animation. Arguments are loop start frame (inclusive) and end frame (exclusive).
void Action::setLoopBounds(const int& start, const int& end)
{
	loopBounds_.first = start;
	loopBounds_.second = end;
	nextLoopBound_ = end;
}

void Action::setLoopBounds(const std::pair<int, int>& bounds)
{
	loopBounds_ = bounds;
}

void Action::updateBoxes(Character& character)
{
	if (currentFrame_ >= boxes_.size())
	{
		return;
	}

	// If the current frame's element in boxes_ is not empty, then switch those in
	if (!boxes_[currentFrame_].empty())
	{
		// Detach old boxes
		for (Box* boxPtr : boxPtrs_)
		{
			std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
			boxes_[currentBoxesInd_].push_back(detachedBox);
		}

		boxPtrs_.clear();

		// Attach new boxes
		for (std::shared_ptr<Box>& box : boxes_[currentFrame_])
		{
			boxPtrs_.push_back(box.get());
			character.attachChild(std::move(box));
		}

		boxes_[currentFrame_].clear();

		currentBoxesInd_ = currentFrame_;
	}
}

void Action::setBoxes(const int& frame, Boxes boxes)
{
	boxes_[frame] = std::move(boxes);
}

void Action::addProperty(Property property, std::vector<int> frameInds)
{
	// Make sure input frame inds are within action duration
	assert(*std::max_element(frameInds.begin(), frameInds.end()) <= properties_.size());

	for (int ind : frameInds)
	{
		properties_[ind] |= property;
	}
}

void Action::addProperty(Property property, boost::integer_range<int> frameInds)
{
	// Make sure input frame inds are within action duration
	assert(*std::max_element(frameInds.begin(), frameInds.end()) <= properties_.size());

	for (int ind : frameInds)
	{
		properties_[ind] |= property;
	}
}

int Action::getCurrentProperty() const
{
	return properties_[currentFrame_];
}

void Action::setMoveID(const int& moveID)
{
	moveID_ = moveID;
}

int Action::getMoveID() const
{
	return moveID_;
}

void Action::setCancelType(const int& cancelType)
{
	cancelType_ |= cancelType;
}

int Action::getCancelType() const
{
	return cancelType_;
}

void Action::setCancel(const int& cancelType, const std::vector<int>& frames)
{
	for (int frame : frames)
	{
		cancels_[frame] |= cancelType;
	}
}

void Action::setCancel(const int& cancelType, const boost::integer_range<int>& frames)
{
	for (auto frame : frames)
	{
		cancels_[frame] |= cancelType;
	}
}

void Action::setCancel(const int& cancelType, const int& startFrameInclusive, const int& endFrameExclusive)
{
	for (auto frame : boost::irange(startFrameInclusive, endFrameExclusive))
	{
		cancels_[frame] |= cancelType;
	}
}

void Action::setMovePerFrame(const std::vector<sf::Vector2f>& movePerFrame)
{
	velocityPerFrame_ = movePerFrame;
}

void Action::setDestinationActionID(const int& id)
{
	destinationActionID_ = id;
}

void Action::applyBallisticVector(const float& launchVelocity, const float& launchAngle)
{
	// Airborne states need to track y-height every frame and leave state as soon as y = 0

	Ballistics ballistics;
	ballistics.LaunchVelocity = launchVelocity;
	ballistics.LaunchAngle = launchAngle;
	ballistics.FrameApplied = currentFrame_;

	// ballistics_ is a vector so multiple vectors can act on the same Entity at once
	ballistics_.push_back(ballistics);
}

sf::Vector2f Action::calculateVelocity(const float& gravity)
{
	// Get base action velocity for this frame
	sf::Vector2f velocity = velocityPerFrame_[currentFrame_];

	// Iterate through ballistics
	for (Ballistics ballisticVector : ballistics_)
	{
		velocity.x += ballisticVector.LaunchVelocity * cos(ballisticVector.LaunchAngle * (M_PI / 180.f));
		velocity.y += ballisticVector.LaunchVelocity * sin(ballisticVector.LaunchAngle * (M_PI / 180.f)) - (gravity * double(currentFrame_ - ballisticVector.FrameApplied));
	}

	return velocity;
}

void Action::setAnimation(Character& character)
{
	character.setAnimationFrames(animationFrameIDs_, animationFrameDurations_, animationSpriteDims_);
}

void Action::setAnimation(Character& character, const std::vector<int>& frameIDs, const std::vector<int>& durations, const sf::Vector2i& spriteDims)
{
	character.setAnimationFrames(frameIDs, durations, spriteDims);
}

Action* Action::handleInput(Character& character, std::vector<Character::ActionPair>& actions)
{
	// Check to see if current frame is cancellable
	if (cancels_[currentFrame_] != Action::CancelType::NoCancel)
	{
		// Iterate through possible destination states
		// Currently the only way priority is determined is by the order of the states in this step
		//   Since it currently iterates backward (via .rbegin()/.rend()), states with greater ID values have greater priority
		for (auto it = actions.rbegin(); it != actions.rend(); ++it)
		{
			// If destination state is same as current state and current frame is self-cancellable
			if (it->second && (it->first.get() == this) && (cancels_[currentFrame_] & Action::CancelType::Self)) 
			{
				// Detach old boxes
				for (Box* boxPtr : boxPtrs_)
				{
					std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
					boxes_[currentBoxesInd_].push_back(detachedBox);
				}

				boxPtrs_.clear();

				currentFrame_ = 0;
				currentBoxesInd_ = 0;

				this->enter(character);

				return nullptr;
			}

			// If destination state isn't current state and it matches the possible cancel paths for the current state
			if	(it->second && (character.getCurrentAction() != it->first.get()) && (it->first->getCancelType() & cancels_[currentFrame_]))
			{
				// Detach old boxes
				for (Box* boxPtr : boxPtrs_)
				{
					std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
					boxes_[currentBoxesInd_].push_back(detachedBox);
				}

				boxPtrs_.clear();

				for (Character::ActionPair& actionPair : actions)
				{
					if (actionPair.first.get() == character.getCurrentAction())
					{
						actionPair.second = false;
					}
				}
				it->second = false;

				currentFrame_ = 0;
				currentBoxesInd_ = 0;

				return it->first.get();
			}
		}
	}

	// If current frame is the LAST frame of the Action's duration and that the current frame isn't the END BOUND of a potential loop
	if ((currentFrame_ == sum_vector(animationFrameDurations_) - 1) && (currentFrame_ != loopBounds_.second - 1))
	{
		// Iterate through possible destination states
		// Currently the only way priority is determined is by the order of the states in this step
		//   Since it currently iterates backward (via .rbegin()/.rend()), states with greater ID values have greater priority
		for (auto it = actions.rbegin(); it != actions.rend(); ++it)
		{
			// Make sure destination state isn't the current state
			if (it->second && ((character.getCurrentAction() != it->first.get())))
			{
				// Detach old boxes
				for (Box* boxPtr : boxPtrs_)
				{
					std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
					boxes_[currentBoxesInd_].push_back(detachedBox);
				}

				boxPtrs_.clear();

				//character.setCurrentActionID(it->first);
				for (Character::ActionPair& actionPair : actions)
				{
					if (actionPair.first.get() == character.getCurrentAction())
					{
						actionPair.second = false;
					}
				}
				it->second = false;

				currentFrame_ = 0;
				currentBoxesInd_ = 0;

				return it->first.get();
			}
		}
	}

	//for (Character::ActionPair& actionPair : actions)
	//{
	//	if (actionPair.first.get() == character.getCurrentAction())
	//	{
	//		actionPair.second = false;
	//		break;
	//	}
	//}
	
	return nullptr;
}

void Action::enter(Character& character)
{
	currentFrame_ = 0;
	character.setCurrentAnimationTick(0);

	for (std::shared_ptr<Box>& box : boxes_[currentFrame_])
	{
		boxPtrs_.push_back(box.get());
		character.attachChild(std::move(box));
	}
	boxes_[currentFrame_].clear();

	setAnimation(character);
}

HeldAction::~HeldAction()
{
}

Action* HeldAction::handleInput(Character& character, std::vector<Character::ActionPair>& actions)
{
	if (!(properties_[currentFrame_] & Action::Property::Recovery))
	{
		// Check to see if the current action is still being inputted for or not
		for (Character::ActionPair& actionPair : actions)
		{
			if ((actionPair.first.get() == this) && !actionPair.second)
			{
				// Find first recovery, if any (this assumes there's only one set of recovery frames and that set is continuous)		
				for (int i = 0; i < properties_.size(); ++i)
				{
					if (properties_[i] & Action::Property::Recovery)
					{
						currentFrame_ = i;
						character.setCurrentAnimationTick(currentFrame_);
						break;
					}
				}
				break;
			}
		}
	}

	Action* action = Action::handleInput(character, actions);
	return action;
}

AirborneAction::AirborneAction()
: startup_(0)
{
}

AirborneAction::~AirborneAction()
{
}

void AirborneAction::update(Character& character)
{
	++currentFrame_;

	if ((loopBounds_.first != loopBounds_.second) && (currentFrame_ == nextLoopBound_) && !(properties_[currentFrame_] & Action::Property::Recovery))
	{
		// Assumes Animation updates after Action in Character::update()
		character.setCurrentAnimationTick(loopBounds_.first);
		// Don't loop again until the same point in the next loop
		nextLoopBound_ += (loopBounds_.second - loopBounds_.first + 1);
	}

	// AirborneActions are continuous and have no defined end (they end when the Entity hits the ground)
	// Thus all currentFrame_ is used for is to advance animations until a looping portion and to calculate velocity
	sf::Vector2f currentVelocity = calculateVelocity(character.getGravity());
	character.move(character.getFacingSign() * currentVelocity.x, -1 * currentVelocity.y);

	updateBoxes(character);
}

Action* AirborneAction::handleInput(Character& character, std::vector<Character::ActionPair>& actions)
{
	//if (!(properties_[currentFrame_] & Action::Property::Recovery))
	//{
	//	// Check to see if the current action is still being inputted for or not
	//	for (Character::ActionPair& actionPair : actions)
	//	{
	//		if (actionPair.first.get() == this && !actionPair.second)
	//		{
	//			// Find recovery frames, if any			
	//			for (int i = 0; i < properties_.size(); ++i)
	//			{
	//				if (properties_[i] & Action::Property::Recovery)
	//				{
	//					currentFrame_ = i;
	//					character.setCurrentAnimationTick(currentFrame_);
	//					break;
	//				}
	//			}
	//			break;
	//		}
	//	}
	//}

	//Action* action = Action::handleInput(character, actions);
	//return action;

	if (properties_[currentFrame_] & Action::Property::Recovery)
	{
		//Action* action = Action::handleInput(character, actions);
		return nullptr;
	}

	// Check if character has landed every frame
	if ((currentFrame_ > startup_) && (character.getPosition().y >= 0.f))
	{
		// Make sure character doesn't end up buried in the ground
		character.setPosition(sf::Vector2f(character.getPosition().x, 0.f));

		//// Check to see if this Action has recovery after landing
		//bool hasRecovery = false;
		//for (int i = 0; i < properties_.size(); ++i)
		//{
		//	if (properties_[i] & Action::Property::Recovery)
		//	{
		//		hasRecovery = true;
		//		currentFrame_ = i;
		//		character.setCurrentAnimationTick(currentFrame_);
		//		nextLoopBound_ = 0;
		//		ballistics_.clear();
		//		break;
		//	}
		//}

		//if (!hasRecovery)
		//{
			// Detach old boxes
			for (Box* boxPtr : boxPtrs_)
			{
				std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
				boxes_[currentBoxesInd_].push_back(detachedBox);
			}

			boxPtrs_.clear();
			ballistics_.clear();

			for (Character::ActionPair& actionPair : actions)
			{
				if (actionPair.first.get() == this)
				{
					actionPair.second = false;
				}
			}

			//character.setCurrentActionID(COMMON_ACTION_STAND);
			character.setPosture(Character::Posture::Standing);

			currentFrame_ = 0;
			currentBoxesInd_ = 0;
			nextLoopBound_ = loopBounds_.second;

			return actions[destinationActionID_].first.get();
		//}
	}

	return nullptr;
}

void AirborneAction::enter(Character& character)
{
	Action::enter(character);

	character.setPosture(Character::Posture::Airborne);
}

void AirborneAction::setBoxes(const int& frame, Boxes boxes)
{
	if (frame >= boxes_.size())
	{
		boxes_.resize(frame + 1);
	}

	boxes_[frame] = std::move(boxes);
}

sf::Vector2f AirborneAction::calculateVelocity(const float& gravity)
{
	// Get base action velocity for this frame
	sf::Vector2f velocity(0.f, 0.f);

	// Iterate through ballistics and add component velocities
	for (Ballistics ballisticVector : ballistics_)
	{
		velocity.x += ballisticVector.LaunchVelocity * cos(ballisticVector.LaunchAngle * (M_PI / 180.f));
		velocity.y += ballisticVector.LaunchVelocity * sin(ballisticVector.LaunchAngle * (M_PI / 180.f)) - (gravity * (double(currentFrame_) - double(ballisticVector.FrameApplied)));
	}

	return velocity;
}

void JumpAction::update(Character& character)
{
	if (currentFrame_ == startup_)
	{
		this->applyBallisticVector(launchVelocity_, launchAngle_);
	}

	AirborneAction::update(character);
}

void JumpAction::setJumpStartup(const int& startup)
{
	startup_ = startup;
}

void JumpAction::setJumpBallistics(const float& launchVelocity, const float& launchAngle)
{
	launchVelocity_ = launchVelocity;
	launchAngle_ = launchAngle;
}

/*

StandAction
CrouchAction
FWalkAction
BWalkAction
JumpAction
5AAction

StandAction -> 5AAction
5AAction !-> FwalkAction
JumpAction !->CrouchAction

Categorize

StandAction & CrouchAction & Fwalk & Bwalk ---> ActionableActions
ActionableAction -> 5A

5AAction & JumpAction & FDashAction ---> NonActionableAction
	These states need Durations
	They need timepoints at which they are or are not cancellable into specific other states

Actionable ---> NormalAttacks

if CurrentAction == Actionable
	look thru states to see which are available
	from that filtered list, see if any of the player's inputs match
	if so, return matching state

NonActionable !---> NormalAttacks

Actions own member variable Property

Head Invul
Throw Invul
Counterhittable

JumpAction has property Throw Invul

Throw hitboxes check opponent current state to see if state has Throw Invul

*/
