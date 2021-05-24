#pragma once

#include "Entity.h"
#include "Event.h"

class Observer
{
public:
	virtual void onNotify(const Entity& entity, Event event) = 0;
};
