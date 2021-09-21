#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "Player.h"
#include "StateStack.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

class Application
{
public:
								Application();
	void						run();


private:
	void						processInput();
	void						update();
	void						render();

	void						updateStatistics(float ft, int numUpdates);
	void						registerStates();


private:
	static const sf::Time		TimePerFrame;

	sf::RenderWindow			window_;
	TextureHolder				textures_;
	FontHolder					fonts_;

	Player						player1_;
	Player						player2_;

	StateStack					stateStack_;

	float						currentSlice_;
	float						lastFT_;

	float						statsTimer_;
	int							totalUpdates_;
	int							updatesPerSec_;
	sf::Text					statsText_;
};