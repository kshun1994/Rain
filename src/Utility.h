#pragma once

#include "Animation.h"

#include <SFML/Window/Keyboard.hpp>


namespace sf
{
	class Sprite;
	class Text;
}

std::string keyToString(sf::Keyboard::Key key);

void centerOrigin(sf::Sprite& sprite);
void centerOrigin(sf::Text& text);
void centerOrigin(Animation& animation);