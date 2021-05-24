#include "rnpch.h"
#include "Label.h"
#include "Utility.h"

#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{

	Label::Label(const std::string& text, const FontHolder& fonts)
	:text_(text, fonts.get(Fonts::ID::Main), 16)
	{
	}

	bool Label::isSelectable() const
	{
		// why would you be able to select a bunch of text
		return false;
	}

	void Label::handleEvent(const sf::Event&)
	{
	}

	void Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();
		target.draw(text_, states);
	}

	void Label::setText(const std::string& text)
	{
		text_.setString(text);
	}
}