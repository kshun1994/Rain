#pragma once
#include <SFML/Graphics.hpp>

#include "Property.h"

// State-based system for Character Actions
// Takes Character (Entity?) objects as reference and applies whatever 
//     Properties to them
// Need a mapping of inputs to cancel endpoints
//     Pointers to InputTriggers mapped to pointers to Actions?
//	   if Trigger->isTriggered() then Action->enter()

class Character;

class Action
{
public:
								~Action();
	virtual void				update(Character& character);
	virtual void				handleInput(Character& character);

	virtual void				setDuration(int duration);

	virtual void				addProperty(Property property);

protected:
	int							duration_;

	std::vector<Property>		properties_;
};
