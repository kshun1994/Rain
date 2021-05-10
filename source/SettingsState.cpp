#include "rnpch.h"
#include "SettingsState.h"
#include "Utility.h"
#include "ResourceHolder.h"

#include <SFML/Graphics/RenderWindow.hpp>


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(Textures::ID::MainMenu));
	centerOrigin(mBackgroundSprite);
	mBackgroundSprite.setPosition(context.window->getView().getSize() / 2.f);

	float windowWidth = context.window->getSize().x;
	float windowHeight = context.window->getSize().y;

	// Build key binding buttons and labels
	addButtonLabel(Player::Left,   100.f, "Left",	context);
	addButtonLabel(Player::Right,  150.f, "Right",	context);
	addButtonLabel(Player::Up,     200.f, "Up",		context);
	addButtonLabel(Player::Down,   250.f, "Down",   context);
	addButtonLabel(Player::A,      300.f, "A",      context);
	addButtonLabel(Player::B,	   350.f, "B",      context);
	addButtonLabel(Player::C,      400.f, "C",      context);
	addButtonLabel(Player::D,      450.f, "D",      context);
	addButtonLabel(Player::Start,  500.f, "Start",  context);
	addButtonLabel(Player::Select, 550.f, "Select", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(80.f, 625.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update()
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	for (std::size_t i = 0; i < magic_enum::enum_count<Player::Action>(); ++i)
	{
		Player::Action action = magic_enum::enum_value<Player::Action>(i);
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<Player::Action>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	if (isKeyBinding)
	{
		updateLabels();
	}
	else
	{
		mGUIContainer.handleEvent(event);
	}
	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;

	for (std::size_t i = 0; i < magic_enum::enum_count<Player::Action>(); ++i)
	{
		Player::Action action = magic_enum::enum_value<Player::Action>(i);
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<Player::Action>(action));
		mBindingLabels[action]->setText(keyToString(key));
	}
}

void SettingsState::addButtonLabel(Player::Action action, float y, const std::string& text, Context context)
{
	//std::size_t ind = magic_enum::enum_value<Player::Action>
	mBindingButtons[action] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[action]->setPosition(80.f, y);
	mBindingButtons[action]->setText(text);
	mBindingButtons[action]->setToggle(true);

	mBindingLabels[action] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[action]->setPosition(300.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[action]);
	mGUIContainer.pack(mBindingLabels[action]);
}
