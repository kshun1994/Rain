#include "rnpch.h"
#include "MenuState.h"
#include "Button.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/View.hpp>


MenuState::MenuState(StateStack& stack, Context context)
	: State(stack, context)
	, gUIContainer_()
{
	sf::Texture& texture = context.textures->get(Textures::ID::MainMenu);
	backgroundSprite_.setTexture(texture);
	centerOrigin(backgroundSprite_);
	backgroundSprite_.setPosition(context.window->getView().getSize() / 2.f);

	auto playButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	playButton->setPosition(100, 250);
	playButton->setText("Play");
	playButton->setCallback([this]()
		{
			requestStackPop();
			requestStackPush(States::ID::Game);
		});
	
	auto settingsButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	settingsButton->setPosition(100, 300);
	settingsButton->setText("Settings");
	settingsButton->setCallback([this] ()
	{
		requestStackPush(States::ID::Settings);
	});

	auto exitButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	exitButton->setPosition(100, 350);
	exitButton->setText("Exit");
	exitButton->setCallback([this] ()
	{
		requestStackPop();
	});

	gUIContainer_.pack(playButton);
	gUIContainer_.pack(settingsButton);
	gUIContainer_.pack(exitButton);
}

void MenuState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.setView(window.getDefaultView());

	window.draw(backgroundSprite_);
	window.draw(gUIContainer_);
}

bool MenuState::update()
{
	return true;
}

bool MenuState::handleEvent(const sf::Event& event)
{
	gUIContainer_.handleEvent(event);
	return false;
}
