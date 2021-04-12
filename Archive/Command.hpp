#pragma once

#include <functional>

#include <SceneNode.hpp>

// Command can be called to apply user input to any game object represented by SceneNode

struct Command
{
															Command();
	std::function<void(SceneNode&, sf::Time)>				action;
	unsigned int											category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn) // convert functions on derived classes into functions onto SceneNode base class
{
	return [=](SceneNode& node, sf::Time dt)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node), dt);
	};
}