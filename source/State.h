#pragma once

#include "StateIdentifiers.h"
#include "ResourceIdentifiers.h"
#include "Player.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>


namespace sf
{
	class RenderWindow;
}

class StateStack;
class Player;

class State
{
public:
	typedef std::unique_ptr<State> Ptr;

	struct Context // holds objects shared between all game states so as not to waste memory storing the same resource multiple times
	{
												Context(sf::RenderWindow& window, 
														TextureHolder& textures, 
														FontHolder& fonts, 
														Player& player1,
														Player& player2);

		sf::RenderWindow*						window;
		TextureHolder*							textures;
		FontHolder*								fonts;
		Player*									player1;
		Player*									player2;
	};

public:
												State(StateStack& stack, Context context);
	virtual										~State();

	virtual void								draw() = 0;
	virtual bool								update() = 0;
	virtual bool								handleEvent(const sf::Event& event) = 0;

protected:
	void										requestStackPush(States::ID states);
	void										requestStackPop();
	void										requestStateClear();

	Context										getContext() const;

private:
	StateStack*									stack_;
	Context										context_;
};