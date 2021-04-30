#include "pch.h"
#include "Animation.h"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>


int nSpritesheetCol = 5;

Animation::Animation()
: mSprite()
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
{
}

Animation::Animation(const sf::Texture& texture)
: mSprite(texture)
, mFrameSize()
, mNumFrames(0)
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
{
}

Animation::Animation(const sf::Texture& texture, 
					 const std::vector<int>& frameIDs, 
					 const std::vector<int>& durations, 
					 const sf::Vector2i& rect)
: mSprite(texture)
, mFrameSize()
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
{
	mNumFrames = durations.size();
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
		mFrameVector.push_back(std::move(frame));
	}
}

Animation::Animation(const sf::Texture& texture,
					 const std::vector<sf::IntRect>& frameRects,
					 const std::vector<int>& durations)
: mSprite(texture)
, mFrameSize()
, mCurrentFrame(0)
, mDuration(sf::Time::Zero)
, mElapsedTime(sf::Time::Zero)
, mRepeat(false)
{
	mNumFrames = durations.size();

	for (int i = 0; i != durations.size(); i++)
	{
		Frame frame = { frameRects[i], durations[i] };
		mFrameVector.push_back(std::move(frame));
	}
}

void Animation::setTexture(const sf::Texture& texture)
{
	mSprite.setTexture(texture);
}

void Animation::setFrames(const std::vector<sf::IntRect>& frameRects, // sprite sheet bounding boxes
						  const std::vector<int>& durations)		  // corresponding durations in frames
{
	mNumFrames = durations.size();
	assert(frameRects.size() == durations.size()); // make sure the two vectors are the same size

	for (int i = 0; i != durations.size(); i++)
	{
		Frame frame = { frameRects[i], durations[i] };
		mFrameVector.push_back(std::move(frame));
	}
}

void Animation::setFrames(const std::vector<int>& frameIDs,  // spritesheet indices (start at 0)
						  const std::vector<int>& durations, // corresponding durations in frames
						  const sf::Vector2i& rect)			 // bounding box width/height
{
	mNumFrames = durations.size();
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
		mFrameVector.push_back(std::move(frame));
	}
}


const sf::Texture* Animation::getTexture() const
{
	return mSprite.getTexture();
}

void Animation::setFrameSize(sf::Vector2i frameSize)
{
	mFrameSize = frameSize;
}

sf::Vector2i Animation::getFrameSize() const
{
	return mFrameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return mNumFrames;
}

void Animation::setDuration(sf::Time duration)
{
	mDuration = duration;
}

sf::Time Animation::getDuration() const
{
	return mDuration;
}

void Animation::setRepeating(bool flag)
{
	mRepeat = flag;
}

bool Animation::isRepeating() const
{
	return mRepeat;
}

void Animation::restart()
{
	mCurrentFrame = 0;
}

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}

sf::FloatRect Animation::getLocalBounds() const
{
	return sf::FloatRect(getOrigin(), static_cast<sf::Vector2f>(getFrameSize()));
}

sf::FloatRect Animation::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

void Animation::update(sf::Time dt)
{
	mElapsedTime += dt;

	sf::Time timePerImageFrame = sf::seconds(mFrameVector[mCurrentFrame].duration / 60.f);
	while (mElapsedTime >= timePerImageFrame && (mCurrentFrame != mFrameVector.size() || mRepeat))
	{
		mElapsedTime -= timePerImageFrame;

		mSprite.setTextureRect(mFrameVector[mCurrentFrame].rect);

		if (mRepeat && mCurrentFrame == mFrameVector.size() - 1)
		{
			mCurrentFrame = 0;
		}
		else
		{
		mCurrentFrame++;
		}
	}
}

void Animation::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(mSprite, states);
}
