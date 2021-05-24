#include "rnpch.h"
#include "SettingsState.h"
#include "Utility.h"
#include "ResourceHolder.h"


SettingsState::SettingsState(StateStack& stack, Context context)
: State(stack, context)
, gUIContainer_()
{
	backgroundSprite_.setTexture(context.textures->get(Textures::ID::MainMenu));
	centerOrigin(backgroundSprite_);
	backgroundSprite_.setPosition(context.window->getView().getSize() / 2.f);

	float windowWidth = context.window->getSize().x;
	float windowHeight = context.window->getSize().y;

	// Build key binding buttons and labels
	addButtonLabel(Input::Left,   100.f, "Left",	context);
	addButtonLabel(Input::Right,  150.f, "Right",	context);
	addButtonLabel(Input::Up,     200.f, "Up",		context);
	addButtonLabel(Input::Down,   250.f, "Down",   context);
	addButtonLabel(Input::A,      300.f, "A",      context);
	addButtonLabel(Input::B,	   350.f, "B",      context);
	addButtonLabel(Input::C,      400.f, "C",      context);
	addButtonLabel(Input::D,      450.f, "D",      context);
	addButtonLabel(Input::Start,  500.f, "Start",  context);
	addButtonLabel(Input::Select, 550.f, "Select", context);

	updateLabels();

	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(80.f, 625.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	gUIContainer_.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;

	window.draw(backgroundSprite_);
	window.draw(gUIContainer_);
}

bool SettingsState::update()
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	for (std::size_t i = 0; i < magic_enum::enum_count<Input>(); ++i)
	{
		Input input = magic_enum::enum_value<Input>(i);
		if (bindingButtons_[input]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player1->assignKey(static_cast<Input>(input), event.key.code);
				bindingButtons_[input]->deactivate();
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
		gUIContainer_.handleEvent(event);
	}
	return false;
}

void SettingsState::updateLabels()
{
	Player& player1 = *getContext().player1;

	for (std::size_t i = 0; i < magic_enum::enum_count<Input>(); ++i)
	{
		Input input = magic_enum::enum_value<Input>(i);
		sf::Keyboard::Key key = player1.getAssignedKey(static_cast<Input>(input));
		bindingLabels_[input]->setText(keyToString(key));
	}
}

void SettingsState::addButtonLabel(Input input, float y, const std::string& text, Context context)
{
	//std::size_t ind = magic_enum::enum_value<Player::Input>
	bindingButtons_[input] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	bindingButtons_[input]->setPosition(80.f, y);
	bindingButtons_[input]->setText(text);
	bindingButtons_[input]->setToggle(true);

	bindingLabels_[input] = std::make_shared<GUI::Label>("", *context.fonts);
	bindingLabels_[input]->setPosition(300.f, y + 15.f);

	gUIContainer_.pack(bindingButtons_[input]);
	gUIContainer_.pack(bindingLabels_[input]);
}
