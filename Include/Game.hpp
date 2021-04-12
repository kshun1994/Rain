#pragma once

#include <World.hpp>
#include <Player.hpp>

#include <SFML/Graphics.hpp> 
//#include <Animation.hpp>

class Game
{
public:
											Game();
	void									run();

private:
	void									processInput();
	void									update(sf::Time dt);
	void									render();
	void									updateStatistics(sf::Time elapsedTime);

private:
	static const float						PlayerSpeed;
	static const sf::Time					TimePerFrame;
	static const bool						bPauseOnLostFocus;

	sf::RenderWindow						mWindow;
	World									mWorld;
	sf::Texture								mTexture;
	Player									mPlayer;
	sf::Font								mFont;
	sf::Text								mStatsText;
	sf::Time								mStatsUpdateTime;

	std::size_t								mStatsNumFrames;
	bool									mIsPaused;
};