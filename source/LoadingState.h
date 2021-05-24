#pragma once

#include "State.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "ParallelTask.h"


class LoadingState : public State
{
public:
	LoadingState(StateStack& stack, Context context);

	virtual void			draw();
	virtual bool			update();
	virtual bool			handleEvent(const sf::Event& event);

	void					setCompletion(float percent);

private:
	sf::Text				loadingText_;
	sf::RectangleShape		progressBarBackground_;
	sf::RectangleShape		progressBar_;

	ParallelTask			loadingTask_;
};