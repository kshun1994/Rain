#pragma once

#include "Category.h"

#include <SFML/System/Time.hpp>

class SceneNode;

struct Command
{
																Command();
	std::function<void(SceneNode&)>								action;
	unsigned int												category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&)> derivedAction(Function fn)
{
	return [=](SceneNode& node)
	{
		// Check if cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		// Downcast node and invoke function on it
		fn(static_cast<GameObject&>(node));
	};
}
