#include "rnpch.h"
#include "Action.h"
#include "Character.h"


Action::Action()
: animationDoesLoop_(false)
, animationIsLooping_(false)
, loopBounds_(0, 0)
, nextLoopBound_()
, currentFrame_(-1) // Initialized to -1 because update() begins by incrementing this by 1
, properties_()
, velocityPerFrame_()
, currentBoxesInd_()
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
	int actionDuration = std::accumulate(durations.begin(), durations.end(), 0);
	properties_.resize(actionDuration, Action::Property::None);
	velocityPerFrame_.resize(actionDuration, sf::Vector2f(0.f, 0.f));
	boxes_.resize(actionDuration);
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

int Action::getCurrentProperty() const
{
	return properties_[currentFrame_];
}

void Action::setMovePerFrame(const std::vector<sf::Vector2f>& movePerFrame)
{
	velocityPerFrame_ = movePerFrame;
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

int Action::handleInput(Character& character, std::map<int, bool> stateMap)
{
	if (properties_[currentFrame_] & Action::Property::Cancellable)
	{
		for (auto it = stateMap.rbegin(); it != stateMap.rend(); ++it)
		{
			if (it->second && (character.getCurrentActionID() != it->first))
			{
				// Detach old boxes
				for (Box* boxPtr : boxPtrs_)
				{
					std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
					boxes_[currentBoxesInd_].push_back(detachedBox);
				}

				boxPtrs_.clear();

				character.setCurrentActionID(it->first);

				currentFrame_ = 0;
				currentBoxesInd_ = 0;
				animationIsLooping_ = false;

				return it->first;
			}
		}
	}

	return CONTINUE_ACTION;
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

	if ((loopBounds_.first != loopBounds_.second) && (currentFrame_ == nextLoopBound_))
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

int AirborneAction::handleInput(Character& character, std::map<int, bool> stateMap)
{
	// Check if character has landed every frame
	if ((currentFrame_ > startup_) && (character.getPosition().y >= 0.f))
	{
		// Make sure character doesn't end up buried in the ground
		character.setPosition(sf::Vector2f(character.getPosition().x, 0.f));

		// Detach old boxes
		for (Box* boxPtr : boxPtrs_)
		{
			std::shared_ptr<Box> detachedBox = std::static_pointer_cast<Box>(character.detachChild(*boxPtr));
			boxes_[currentBoxesInd_].push_back(detachedBox);
		}

		boxPtrs_.clear();
		ballistics_.clear();

		character.setCurrentActionID(COMMON_ACTION_STAND);
		character.setPosture(Character::Posture::Standing);

		currentFrame_ = 0;
		currentBoxesInd_ = 0;
		nextLoopBound_ = loopBounds_.second;
		animationIsLooping_ = false;

		return COMMON_ACTION_STAND;
	}

	return CONTINUE_ACTION;
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
