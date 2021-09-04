#include "rnpch.h"
#include "Action.h"

Action::~Action()
{
}

void Action::update(Character& character)
{
	// Iterate through Properties and call each one's update function on Character
	for (Property property : properties_)
	{
		property.update(character);
	}
}

void Action::handleInput(Character& character)
{
}

void Action::setDuration(int duration)
{
	duration_ = duration;
}

void Action::addProperty(Property property)
{
}
