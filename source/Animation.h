#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Sprite.hpp>


struct Frame
{
	sf::IntRect							rect; // bounding box of animation frame in sprite sheet
	int									duration; // num render frames to display animation frame for
};

class Animation : public sf::Sprite // public sf::Drawable, public sf::Transformable
{
public:
										Animation();
										Animation(const sf::Texture& texture);
										Animation(const sf::Texture& texture,
												  const std::vector<int>& frameIDs,
												  const std::vector<int>& durations,
												  const sf::Vector2i& rect);

										Animation(const sf::Texture& texture,
												  const std::vector<sf::IntRect>& frameRects,
												  const std::vector<int>& durations);

	void								setTexture(const sf::Texture& texture);
	void								setFrames(const std::vector<sf::IntRect>& frameRects,
												  const std::vector<int>& durations);
	void								setFrames(const std::vector<int>& frameIDs,
												  const std::vector<int>& durations,
												  const sf::Vector2i& rect);

	void								setFrameSize(sf::Vector2i frameSize);
	void								setNumFrames(std::size_t numFrames);
	void								setRepeating(bool flag);

	const sf::Texture*					getTexture() const;
	sf::Vector2i						getFrameSize() const;
	std::size_t							getNumFrames() const;
	bool								isRepeating() const;

	sf::FloatRect						getLocalBounds() const;
	sf::FloatRect						getGlobalBounds() const;

	void								update();
	void								draw(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Sprite							sprite_;
	sf::Vector2i						frameSize_; // size of one frame rect
	std::size_t							numFrames_;
	std::size_t							currentFrame_;
	std::vector<Frame>					frameVector_;
	sf::Time							duration_;
	sf::Time							elapsedTime_;
	int									elapsedTicks_;
	bool								repeat_;
};