#include "rnpch.h"
#include "Action.h"
#include "Character.h"


Action::Action()
: animationDoesLoop_(false)
, animationIsLooping_(false)
, loopBounds_(0, 0)
, currentFrame_(0)
, properties_()
, velocityPerFrame_()
{
}

void Action::update(Character& character)
{
	if (character.getType() == Character::Type::Enkidu)
	{
		//RN_DEBUG("CurrentFrame - {}", currentFrame_);
	}

	// Calculate current frame velocity and move the Entity that amount this frame
	sf::Vector2f currentVelocity = calculateVelocity(character.getGravity());
	character.move(character.getFacingSign() * currentVelocity.x, -1 * currentVelocity.y);

	// Check to see if Action needs to be looped; this has to go at the very end before incrementing currentFrame_
	if ((loopBounds_.first != loopBounds_.second) && (currentFrame_ == loopBounds_.second))
	{
		// Assumes Animation updates after Action in Character::update()
		character.setCurrentAnimationTick(loopBounds_.first);
		currentFrame_ = loopBounds_.first - 1; // Since it's going to be incremented at the end of this anyway
	}

	++currentFrame_;
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
}

void Action::setLoopBounds(const int& start, const int& end)
{
	loopBounds_.first = start;
	loopBounds_.second = end;
}

void Action::setLoopBounds(const std::pair<int, int>& bounds)
{
	loopBounds_ = bounds;
}

void Action::setBoxes(std::vector<std::shared_ptr<Box>> boxes)
{
	boxes_ = std::move(boxes);
}

void Action::appendBox(std::shared_ptr<Box> box)
{
	boxes_.push_back(std::move(box));
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
	for (auto it = stateMap.rbegin(); it != stateMap.rend(); ++it)
	{
		if (it->second && (character.getCurrentActionID() != it->first))
		{
			for (Box* box : boxPtrs_)
			{
				auto detachedBox = character.detachChild(*box);
				boxes_.push_back(std::static_pointer_cast<Box>(detachedBox));
			}

			boxPtrs_.clear();

			character.setCurrentActionID(it->first);

			currentFrame_ = 0;
			animationIsLooping_ = false;

			return it->first;
		}
	}

	return CONTINUE_ACTION;
}

void Action::enter(Character& character)
{
	currentFrame_ = 0;
	character.setCurrentAnimationTick(0);

	for (std::shared_ptr<Box>& box : boxes_)
	{
		boxPtrs_.push_back(box.get());
		character.attachChild(std::move(box));
	}
	boxes_.clear();

	setAnimation(character);
}

AirborneAction::~AirborneAction()
{
}

void AirborneAction::update(Character& character)
{
	// AirborneActions are continuous and have no defined end (they end when the Entity hits the ground)
	// Thus all currentFrame_ is used for is to advance animations until a looping portion and to calculate velocity
	sf::Vector2f currentVelocity = calculateVelocity(character.getGravity());
	character.move(character.getFacingSign() * currentVelocity.x, -1 * currentVelocity.y);

	// Use a second variable for progressing through loop so acceleration due to gravity isn't reset by loop resets
	if ((loopBounds_.first != loopBounds_.second) && (animationLoopProgress_ == loopBounds_.second))
	{
		// Find the animation frame corresponding to the loop's beginning
		int total = 0;
		int loopAnimationFrame = 0;
		for (int i = 0; i < animationFrameDurations_.size(); ++i)
		{
			total += animationFrameDurations_[i];
			if (total >= loopBounds_.first)
			{
				loopAnimationFrame = i;
				break;
			}
		}

		// Assumes Animation updates after Action in Character::update()
		character.setCurrentAnimationTick(loopBounds_.first);
		character.setCurrentAnimationFrame(loopAnimationFrame - 1);
		animationLoopProgress_ = loopBounds_.first - 1; // Since it's going to be incremented at the end of this anyway
	}

	++animationLoopProgress_;
	++currentFrame_;
}

int AirborneAction::handleInput(Character& character, std::map<int, bool> stateMap)
{
	// Check if character has landed every frame
	//RN_DEBUG("Current position: ({}, {})", character.getPosition().x, character.getPosition().y);
	RN_DEBUG("Current frame: {}", currentFrame_);
	if ((currentFrame_ > startup_) && (character.getPosition().y >= 0.f))
	{
		// Make sure character doesn't end up buried in the ground
		character.setPosition(sf::Vector2f(character.getPosition().x, 0.f));

		for (Box* box : boxPtrs_)
		{
			auto detachedBox = character.detachChild(*box);
			boxes_.push_back(std::static_pointer_cast<Box>(detachedBox));
		}

		boxPtrs_.clear();
		ballistics_.clear();

		character.setCurrentActionID(COMMON_ACTION_STAND);
		character.setPosture(Character::Posture::Standing);

		currentFrame_ = 0;
		animationLoopProgress_ = 0;
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
