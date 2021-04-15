#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>


class Animation : public sf::Drawable, public sf::Transformable
{
public:
	Animation();
	Animation(const sf::Texture& texture);

	void								setTexture(const sf::Texture& texture);
	void								setFrameSize(sf::Vector2i frameSize);
	void								setNumFrames(std::size_t numFrames);
	void								setDuration(sf::Time duration);
	void								setRepeating(bool flag);

	const sf::Texture*					getTexture() const;
	sf::Vector2i						getFrameSize() const;
	std::size_t							getNumFrames() const;
	sf::Time							getDuration() const;
	bool								isRepeating() const;

	void								restart();
	bool								isFinished() const;

	sf::FloatRect						getLocalBounds() const;
	sf::FloatRect						getGlobalBounds() const;

	void								update(sf::Time dt);
	void								draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite							mSprite;
	sf::Vector2i						mFrameSize; // size of one frame rect
	std::size_t							mNumFrames;
	std::size_t							mCurrentFrame;
	sf::Time							mDuration;
	sf::Time							mElapsedTime;
	bool								mRepeat;
};