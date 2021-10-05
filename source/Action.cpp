#include "rnpch.h"
#include "Action.h"
#include "Character.h"


Action::Action()
: animationDoesLoop_(false)
, animationIsLooping_(false)
, loopBounds_(0, 0)
, currentFrame_(0)
, properties_()
, movePerFrame_()
{
}

void Action::update(Character& character)
{
	if (character.getType() == Character::Type::Enkidu)
	{
		//RN_DEBUG("CurrentFrame - {}", currentFrame_);
	}

	// Move the character a specified amount for the given frame
	character.move(character.getFacingSign() * movePerFrame_[currentFrame_].x, -1 * movePerFrame_[currentFrame_].y);

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
	movePerFrame_.resize(actionDuration, sf::Vector2f(0.f, 0.f));
}

void Action::setAnimationLoop(const bool& flag)
{
	// In case of no arguments to function set entire animation as loop range
	animationDoesLoop_ = flag;
	animationLoopFrames_.resize(animationFrameIDs_.size());
	std::iota(animationLoopFrames_.begin(), animationLoopFrames_.end(), 0);
}

void Action::setAnimationLoop(const std::vector<int>& loopFrames)
{
	animationDoesLoop_ = true;
	animationLoopFrames_ = loopFrames;
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
	movePerFrame_ = movePerFrame;
}

void Action::applyBallisticVector(const float& gravity, const float& launchSpeed, const float& launchAngle)
{
	// Airborne states need to track y-height every frame and leave state as soon as y = 0

	// Add to movePerFrame_ elements based on ballistic trajectory calculation per unit time (i.e. per frame)
	for (sf::Vector2f velocity : movePerFrame_)
	{

	}
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

void StandAction::enter(Character& character)
{
	Action::enter(character);
	RN_DEBUG("Entered StandAction.");
}

void CrouchAction::enter(Character& character)
{
	Action::enter(character);
	RN_DEBUG("Entered CrouchAction.");
}

void FWalkAction::update(Character& character)
{
	Action::update(character);
	
}

void FWalkAction::enter(Character& character)
{
	Action::enter(character);
	RN_DEBUG("Entered FWalkAction.");
}

void FWalkAction::setSpeed(float speed)
{
	speed_ = speed;
}

void BWalkAction::update(Character& character)
{
	Action::update(character);
}

void BWalkAction::enter(Character& character)
{
	Action::enter(character);
	RN_DEBUG("Entered BWalkAction.");
}

void BWalkAction::setSpeed(float speed)
{
	speed_ = speed;
}

int JumpAction::handleInput(Character& character, std::map<int, bool> stateMap)
{
	if (currentFrame_ == std::accumulate(animationFrameDurations_.begin(), animationFrameDurations_.end(), 0))
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
	}

	return CONTINUE_ACTION;
}

void JumpAction::setJumpArcViaInitialY(const float& ySpeedInitial,
									   const float& peakHeight, 
									   const float& range, 
									   const int& jumpDuration)
{
	// Calculate xSpeed
	float xSpeed = range / jumpDuration;

	// Calculate gravity for jump arc
	//float gravity = pow(ySpeedInitial, 2) / (2.f * double(peakHeight));
	float gravity = (2 * ySpeedInitial) / jumpDuration;
	//float gravity = ((4 * double(ySpeedInitial) * double(jumpDuration)) - (8 * double(peakHeight))) / pow(double(jumpDuration), 2);
	//float gravity = ((pow(xSpeed, 2) + pow(ySpeedInitial, 2)) * pow(sinf(tanf(ySpeedInitial / xSpeed)), 2)) / (2 * double(peakHeight));

	// Check to make sure jump active range is the same as input duration of travel
	int activeFrames = 0;
	for (int property : properties_)
	{
		if (property & Action::Property::Active)
		{
			++activeFrames;
		}
	}
	assert(activeFrames == jumpDuration);

	for (int i = 0; i < movePerFrame_.size(); ++i)
	{
		if (properties_[i] & Action::Property::Active)
		{
			movePerFrame_[i].x = xSpeed;
			movePerFrame_[i].y = ySpeedInitial - (i * gravity);
		}
	}

	RN_DEBUG("");
}

// TODO 
// Characters have specific gravity numbers (can be modified by proration)
// Character airborne state just checks position.y every update and once it's 0 leave airborne
// Have some kind of applyMoveVector() function to be used throughout game?

// Can calculate gravity or ySpeedInitial given the other and total duration
//     gravity = (2 * ySpeedInitial) / duration
//     ySpeedInitial = (gravity * duration) / 2
// Can calculate ySpeedInitial given peakHeight and gravity
//     ySpeedInitial = sqrt(2 * gravity * peakHeight)
// Can calculate gravity given ySpeedInitial and peakHeight
//     gravity = (ySpeedInitial ^ 2) / (2 * peakHeight)
// Can calculate xSpeed given gravity, ySpeedInitial, and range (total horizontal distance travelled)
//     xSpeed = (gravity * range) / (2 * ySpeedInitial)
//
// Once above has been calculated, the number of frames to display each jump animation frame can be calculated

void JumpAction::setJumpArcViaGravity(const float& gravity, 
									  const float& peakHeight, 
									  const float& range, 
									  const int& jumpDuration)
{
	// Calculate ySpeedInital for jump arc
	float ySpeedInitial = sqrt(2 * gravity * peakHeight);

	// Calculate xSpeed
	float xSpeed = (gravity * range) / (2 * ySpeedInitial);

	// Check to make sure jump active range is the same as input jumpDuration of travel
	int activeFrames = 0;
	for (int property : properties_)
	{
		if (property & Action::Property::Active)
		{
			++activeFrames;
		}
	}
	assert(activeFrames == jumpDuration);

	for (int i = 0; i < jumpDuration; ++i)
	{
		movePerFrame_[i].x = xSpeed;
		movePerFrame_[i].y = ySpeedInitial - (i * gravity);
	}
}

//void JumpAction::setAnimationFrames(const std::vector<int>& frameIDs, const sf::Vector2i& spriteDims, const int& jumpStartup, const int& jumpDuration)
//{
//	int remainingDuration = jumpDuration;
//
//	// Check to make sure jump active range is the same as input jumpDuration of travel
//	// This means properties have to be set BEFORE animation is set
//	int activeFrames = 0;
//	for (int property : properties_)
//	{
//		if (property & Action::Property::Active)
//		{
//			++activeFrames;
//		}
//	}
//	assert(activeFrames == jumpDuration);
//
//	// Want to split the 
//}

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
