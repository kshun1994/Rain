#include "rnpch.h"
#include "Animation.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


int nSpritesheetCol = 5;

Animation::Animation()
: sprite_()
, frameSize_()
, numFrames_(0)
, currentFrame_(0)
, duration_(sf::Time::Zero)
, elapsedTime_(sf::Time::Zero)
, elapsedTicks_(0)
, repeat_(false)
{
}

Animation::Animation(const sf::Texture& texture)
: sprite_(texture)
, frameSize_()
, numFrames_(0)
, currentFrame_(0)
, duration_(sf::Time::Zero)
, elapsedTime_(sf::Time::Zero)
, elapsedTicks_(0)
, repeat_(false)
{
}

Animation::Animation(const sf::Texture& texture, 
					 const std::vector<int>& frameIDs, 
					 const std::vector<int>& durations, 
					 const sf::Vector2i& rect)
: sprite_(texture)
, frameSize_()
, currentFrame_(0)
, duration_(sf::Time::Zero)
, elapsedTime_(sf::Time::Zero)
, elapsedTicks_(0)
, repeat_(false)
{
	numFrames_ = durations.size();
	assert(frameIDs.size() == durations.size()); // make sure the two vectors are the same size

	sf::IntRect currentRect;
	currentRect.width  = rect.x;
	currentRect.height = rect.y;

	for (int i = 0; i != durations.size(); i++)
	{
		int currentFrameID = frameIDs[i];
		currentRect.left   = rect.x * (currentFrameID % nSpritesheetCol);
		currentRect.top	   = rect.y * (currentFrameID / nSpritesheetCol);

		Frame frame = { currentRect, durations[i] };
		frameVector_.push_back(std::move(frame));
	}
}

Animation::Animation(const sf::Texture& texture,
					 const std::vector<sf::IntRect>& frameRects,
					 const std::vector<int>& durations)
: sprite_(texture)
, frameSize_()
, currentFrame_(0)
, duration_(sf::Time::Zero)
, elapsedTime_(sf::Time::Zero)
, elapsedTicks_(0)
, repeat_(false)
{
	numFrames_ = durations.size();

	for (int i = 0; i != durations.size(); i++)
	{
		Frame frame = { frameRects[i], durations[i] };
		frameVector_.push_back(std::move(frame));
	}
}

void Animation::setTexture(const sf::Texture& texture)
{
	sprite_.setTexture(texture);
}

void Animation::setFrames(const std::vector<sf::IntRect>& frameRects, // sprite sheet bounding boxes
						  const std::vector<int>& durations)		  // corresponding durations in frames
{
	// Clear any stuff from previous animation
	currentFrame_ = 0;
	elapsedTicks_ = 0;
	frameVector_.clear();

	numFrames_ = durations.size();
	assert(frameRects.size() == durations.size()); // make sure the two vectors are the same size

	for (int i = 0; i != durations.size(); i++)
	{
		Frame frame = { frameRects[i], durations[i] };
		frameVector_.push_back(std::move(frame));
	}

	sprite_.setTextureRect(frameVector_[currentFrame_].rect);
}

void Animation::setFrames(const std::vector<int>& frameIDs,  // spritesheet indices (start at 0)
						  const std::vector<int>& durations, // corresponding durations in frames
						  const sf::Vector2i& rect)			 // bounding box width/height
{
	// Clear any stuff from previous animation
	currentFrame_ = 0;
	elapsedTicks_ = 0;
	frameVector_.clear();

	numFrames_ = durations.size();
	assert(frameIDs.size() == durations.size()); // make sure the two vectors are the same size

	for (int i = 0; i != durations.size(); i++)
	{
		int currentFrameID = frameIDs[i];

		sf::IntRect currentRect;
		currentRect.width  = rect.x;
		currentRect.height = rect.y;
		currentRect.left   = rect.x * (currentFrameID % nSpritesheetCol);
		currentRect.top	   = rect.y * (currentFrameID / nSpritesheetCol);

		Frame frame = { currentRect, durations[i] };
		frameVector_.push_back(std::move(frame));
	}

	sprite_.setTextureRect(frameVector_[currentFrame_].rect);
}


const sf::Texture* Animation::getTexture() const
{
	return sprite_.getTexture();
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	frameSize_ = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return frameSize_;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	numFrames_ = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return numFrames_;
}

void Animation::setRepeating(bool flag)
{
	repeat_ = flag;
}

bool Animation::isRepeating() const
{
	return repeat_;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::update()
{
	elapsedTicks_++;

	while (elapsedTicks_ >= frameVector_[currentFrame_].duration && (currentFrame_ != frameVector_.size() || repeat_))
	{
		if (repeat_ && (currentFrame_ == frameVector_.size() - 1))
		{
			currentFrame_ = 0;
		}
		else
		{
			currentFrame_++;
		}

		sprite_.setTextureRect(frameVector_[currentFrame_].rect);

		elapsedTicks_ = 0;
	}
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(sprite_, states);
}
