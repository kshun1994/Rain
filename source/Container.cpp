#include "rnpch.h"
#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{
	Container::Container()
	: children_()
	, selectedChild_(-1)
	{
	}

	void Container::pack(Component::Ptr component)
	{
		children_.push_back(component); // append incoming container to children
		if (!hasSelection() && component->isSelectable()) // check if there's a currently selected child and if not check if appended component is selectable
		{
			select(children_.size() - 1); // select appended component
		}
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event& event)
	{
		if (hasSelection() && children_[selectedChild_]->isActive()) // check if valid selection and whether selected component is active
		{
			children_[selectedChild_]->handleEvent(event); // if so then pass events to component instead of container
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			if (event.key.code == sf::Keyboard::W || 
				event.key.code == sf::Keyboard::Up)
			{
				selectPrevious();
			}
			else if (event.key.code == sf::Keyboard::S || 
				 	 event.key.code == sf::Keyboard::Down)
 			{
				selectNext();
			}
			else if (event.key.code == sf::Keyboard::J || 
					 event.key.code == sf::Keyboard::Return)
			{
				if (hasSelection())
				{
					children_[selectedChild_]->activate();
				}
			}
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (const Component::Ptr& child : children_)
		{
			target.draw(*child, states);
		}
	}

	bool Container::hasSelection() const
	{
		return selectedChild_ >= 0;
	}

	void Container::select(std::size_t index)
	{
		if (children_[index]->isSelectable())
		{
			if (hasSelection())
			{
				children_[selectedChild_]->deselect();
			}

			children_[index]->select();
			selectedChild_ = index;
		}
	}

	void Container::selectNext()
	{
		if (!hasSelection())
		{
			return;
		}

		// Search next selectable component
		int next = selectedChild_;
		do
			next = (next + 1) % children_.size();
		while (!children_[next]->isSelectable());

		// Select that component
		select(next);
	}

	void Container::selectPrevious()
	{
		if (!hasSelection())
		{
			return;
		}

		// Search previous selectable component
		int prev = selectedChild_;
		do 
			prev = (prev + children_.size() - 1) % children_.size();
		while (!children_[prev]->isSelectable());

		// Select that component
		select(prev);
	}
}

