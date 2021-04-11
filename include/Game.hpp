#include <SFML/Graphics.hpp>

class Game
{
public:
											Game();
	void									run();

private:
	void									processEvents();
	void									update(sf::Time dt);
	void									render();
	void									handlePlayerInput(sf::Keyboard::Key key, bool bPressed);
	void									updateStatistics(sf::Time elapsedTime);

private:
	static const float						PlayerSpeed;
	static const sf::Time					TimePerFrame;

	sf::RenderWindow						mWindow;
	sf::Texture								mTexture;
	sf::Sprite								mPlayer;
	sf::Font								mFont;
	sf::Text								mStatsText;
	sf::Time								mStatsUpdateTime;

	std::size_t								mStatsNumFrames;
	bool									mIsMovingUp;
	bool									mIsMovingDown;
	bool									mIsMovingLeft;
	bool									mIsMovingRight;
};