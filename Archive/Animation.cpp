#include <Animation.hpp>

Animation::Animation(sf::Sprite& target)
	: mTarget(target)
{
	mProgress = mTotalLength = 0.0;
}

void Animation::addFrame(Frame&& frame)
{
	mTotalLength += frame.duration / 60.0;
	mFrames.push_back(std::move(frame));
}

void Animation::update(double elapsed)
{
	mProgress += elapsed;
	double p = mProgress;

	for (size_t i = 0; i < mFrames.size(); i++)
	{
		p -= mFrames[i].duration / 60.0;

		if (p <= 0.0 || &(mFrames[i]) == &mFrames.back())
		{
			mTarget.setTextureRect(mFrames[i].rect);
			break;
		}
	}
}