#pragma once

#include <Animation.hpp>

#include <SFML/Window/Keyboard.hpp>

#include <sstream>


namespace sf
{
	class Sprite;
	class Text;
}

std::string keyToString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);