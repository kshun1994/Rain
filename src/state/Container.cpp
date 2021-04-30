#include "pch.h"
#include "Container.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/RenderTarget.hpp>


namespace GUI
{
	Container::Container()
	: mChildren()
	, mSelectedChild(-1)
	{
	}

	void Container::pack(Component::Ptr component)
	{
		mChildren.push_back(component); // append incoming container to children
		if (!hasSelection() && component->isSelectable()) // check if there's a currently selected child and if not check if appended component is selectable
		{
			select(mChildren.size() - 1); // select appended component
		}
	}

	bool Container::isSelectable() const
	{
		return false;
	}

	void Container::handleEvent(const sf::Event& event)
	{
		if (hasSelection() && mChildren[mSelectedChild]->isActive()) // check if valid selection and whether selected component is active
		{
			mChildren[mSelectedChild]->handleEvent(event); // if so then pass events to component instead of container
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
					mChildren[mSelectedChild]->activate();
				}
			}
		}
	}

	void Container::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.transform *= getTransform();

		for (const Component::Ptr& child : mChildren)
		{
			target.draw(*child, states);
		}
	}

	bool Container::hasSelection() const
	{
		return mSelectedChild >= 0;
	}

	void Container::select(std::size_t index)
	{
		if (mChildren[index]->isSelectable())
		{
			if (hasSelection())
			{
				mChildren[mSelectedChild]->deselect();
			}

			mChildren[index]->select();
			mSelectedChild = index;
		}
	}

	void Container::selectNext()
	{
		if (!hasSelection())
		{
			return;
		}

		// Search next selectable component
		int next = mSelectedChild;
		do
			next = (next + 1) % mChildren.size();
		while (!mChildren[next]->isSelectable());

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
		int prev = mSelectedChild;
		do 
			prev = (prev + mChildren.size() - 1) % mChildren.size();
		while (!mChildren[prev]->isSelectable());

		// Select that component
		select(prev);
	}
}

